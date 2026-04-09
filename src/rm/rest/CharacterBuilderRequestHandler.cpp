#include <CharacterBuilderRequestHandler.h>
#include <CharacterStat.h>
#include <Dice.h>
#include <HttpPathParser.h>
#include <StringUtils.h>
#include <boost/json.hpp>
#include <optional>

// Helper function to safely extract a string value from a JSON object, returning std::nullopt if the key is not found or the value is not a string
std::optional<std::string> get_optional_string(boost::json::object const& obj, std::string_view key) {
	auto it = obj.find(key);
	if (it != obj.end() && it->value().is_string()) {
		return boost::json::value_to<std::string>(it->value());
	}
	return std::nullopt;
}

namespace rm::rest {

void CharacterBuilderRequestHandler::handleRequest(const http::request<http::string_body>& request, http::response<http::string_body>& response) {
	// Decode the request target to handle URL-encoded characters
	std::string request_string = archid::uriDecode(request.target());

	// Extract path and query parameters
	const HttpPathParser path(request_string);

	// Get the operation from the path to determine which specific character builder task to perform
	std::string_view operation = path.extractNextSegment("/rmce/operations/character/");

	if (request.method() == http::verb::post && operation == "primary-definition")
		requestPrimaryDefinition(response, request);
	else if (request.method() == http::verb::post && operation == "primary-choices")
		requestPrimaryChoices(response, request);
	else if (request.method() == http::verb::post && operation == "stat-rolls")
		requestStatRolls(response, request);
	else if (request.method() == http::verb::post && operation == "set-stats")
		requestSetStats(response, request);
	else if (request.method() == http::verb::get && operation == "hobby-choices" && path.params().contains("id"))
		requestHobbyChoices(response, request, path.params().at("id"));
	else if (request.method() == http::verb::post && operation == "set-hobby-choices")
		requestSetHobbyChoices(response, request);
	else if (request.method() == http::verb::post && operation == "set-background-choices")
		requestSetBackgroundChoices(response, request);
	else if (request.method() == http::verb::get && operation == "dump" && path.params().contains("id"))
		requestDump(response, request, path.params().at("id"));
	else {
		response.result(http::status::not_found);
		response.set(http::field::content_type, "application/json");
		response.body() = R"({"error": "Endpoint not found", "message": "The requested endpoint does not exist"})";
	}
}

void CharacterBuilderRequestHandler::requestPrimaryDefinition(http::response<http::string_body>& response, const http::request<http::string_body>& request) {
	using namespace rm::game::character;

	std::string id{};
	CharacterBuilder& builder = serial_manager_.objectManager().get<CharacterBuilder>();
	try {
		json::value json_body = json::parse(request.body());
		if (!json_body.is_object()) {
			response.result(http::status::bad_request);
			response.set(http::field::content_type, "application/json");
			response.body() = R"({"error": "Invalid request body", "message": "Expected a JSON object"})";
			return;
		}

		std::string name = json_body.as_object().at("name").as_string().c_str();
		std::string race_id = json_body.as_object().at("race").as_string().c_str();
		std::string culture_id = json_body.as_object().at("culture").as_string().c_str();
		std::string profession_id = json_body.as_object().at("profession").as_string().c_str();
		const auto& realms = json_body.as_object().at("realms").as_array();
		std::set<RealmType::Type> magical_realms;
		for (const auto& realm : realms) {
			magical_realms.insert(RealmType::fromString(realm.as_string()).value());
		}
		builder.setPrimaryDefinition(serial_manager_.objectManager(), name, race_id, culture_id, profession_id, magical_realms);

		builder.recalculateAggregatedState();

		response.result(http::status::ok);
		response.set(http::field::content_type, "application/json");
		response.body() = serial_manager_.serializeObject<CharacterBuilder>(builder);
	} catch (const std::exception& e) {
		response.result(http::status::internal_server_error);
		response.body() = R"({"error": "Failed to set primary choices", "message": ")" + archid::escapeJson(e.what()) + R"("})";
	}
}

void CharacterBuilderRequestHandler::requestPrimaryChoices(http::response<http::string_body>& response, const http::request<http::string_body>& request) {
	using namespace rm::game::character;

	try {
		std::string body = request.body();
		json::value json_body = json::parse(request.body());
		if (!json_body.is_object()) {
			response.result(http::status::bad_request);
			response.set(http::field::content_type, "application/json");
			response.body() = R"({"error": "Invalid request body", "message": "Expected a JSON object"})";
			return;
		}
		std::string id = json_body.as_object().at("id").as_string().c_str();

		// This returns a const object, but we need a non-const reference to update the builder with the choices, so we will deserialize it first to update the cache and then get a non-const reference to it to perform the updates.
		const CharacterBuilder& deserialized = serial_manager_.deserializeObject<CharacterBuilder>(json_body.as_object());
		CharacterBuilder& builder = serial_manager_.objectManager().get<CharacterBuilder>(id);

		builder.recalculateAggregatedState();

		response.result(http::status::ok);
		response.set(http::field::content_type, "application/json");
		response.body() = serial_manager_.serializeObject<CharacterBuilder>(builder);
		std::string serialized = response.body();
		int x{};
	} catch (const std::exception& e) {
		response.result(http::status::internal_server_error);
		response.body() = R"({"error": "Failed to generate primary choices", "message": ")" + archid::escapeJson(e.what()) + R"("})";
	}
}

void CharacterBuilderRequestHandler::requestStatRolls(http::response<http::string_body>& response, const http::request<http::string_body>& request) {
	try {
		json::value json_body = json::parse(request.body());
		if (!json_body.is_array()) {
			response.result(http::status::bad_request);
			response.set(http::field::content_type, "application/json");
			response.body() = R"({"error": "Invalid request body", "message": "Expected a JSON array"})";
			return;
		}

		archid::Dice d100(100);
		json::array result_array;

		for (const auto& element : json_body.as_array()) {
			if (!element.is_object()) {
				response.result(http::status::bad_request);
				response.set(http::field::content_type, "application/json");
				response.body() = R"({"error": "Invalid request body", "message": "Each element must be a JSON object"})";
				return;
			}

			const auto& obj = element.as_object();

			// Parse temporary: re-roll any result < 25
			int temporary = static_cast<int>(obj.at("temporary").as_int64());
			if (temporary < 25) {
				do {
					temporary = d100.roll(false).result();
				} while (temporary < 25);
			}

			// Calculate potential from temporary and pot_roll
			int potential = rm::game::character::stat::getInitialPotentialValue(temporary);

			result_array.push_back(json::object({{"temporary", temporary}, {"potential", potential}}));
		}

		response.result(http::status::ok);
		response.set(http::field::content_type, "application/json");
		response.body() = json::serialize(json::value(result_array));
	} catch (const std::exception& e) {
		response.result(http::status::internal_server_error);
		response.body() = R"({"error": "Failed to perform stat rolls", "message": ")" + archid::escapeJson(e.what()) + R"("})";
	}
}

void CharacterBuilderRequestHandler::requestSetStats(http::response<http::string_body>& response, const http::request<http::string_body>& request) {
	using namespace rm::game::character;

	try {
		json::value json_body = json::parse(request.body());
		if (!json_body.is_object()) {
			response.result(http::status::bad_request);
			response.set(http::field::content_type, "application/json");
			response.body() = R"({"error": "Invalid request body", "message": "Expected a JSON object"})";
			return;
		}
		std::string id = json_body.as_object().at("id").as_string().c_str();

		// This returns a const object, but we need a non-const reference to update the builder with the choices, so we will deserialize it first to update the cache and then get a non-const reference to it to perform the updates.
		const CharacterBuilder& deserialized = serial_manager_.deserializeObject<CharacterBuilder>(json_body.as_object());
		CharacterBuilder& builder = serial_manager_.objectManager().get<CharacterBuilder>(id);

		builder.recalculateAggregatedState();

		// After setting the stats, we can immediately return the hobby choices. This allows the client to update the UI with the new hobby choices without needing to make a separate request.
		http::request<http::string_body> hobby_request; // We need a dummy request object to pass to the hobby choices function, but it won't be used in that function so we can just create an empty one.
		requestHobbyChoices(response, hobby_request, id);

	} catch (const std::exception& e) {
		response.result(http::status::internal_server_error);
		response.body() = R"({"error": "Failed to set stats", "message": ")" + archid::escapeJson(e.what()) + R"("})";
	}
}

void CharacterBuilderRequestHandler::requestHobbyChoices(http::response<http::string_body>& response, const http::request<http::string_body>& request, std::string id) {
	using namespace rm::game::character;

	try {
		CharacterBuilder& builder = serial_manager_.objectManager().get<CharacterBuilder>(id);

		rm::serial::CharacterBuilderSerializer serializer(serial_manager_.objectManager());
		json::value serialized_hobby_choices = serializer.serializeHobbyChoices(builder);

		response.result(http::status::ok);
		response.set(http::field::content_type, "application/json");
		response.body() = json::serialize(serialized_hobby_choices);

	} catch (const std::exception& e) {
		response.result(http::status::internal_server_error);
		response.body() = R"({"error": "Failed to get hobby choices", "message": ")" + archid::escapeJson(e.what()) + R"("})";
	}
}

void CharacterBuilderRequestHandler::requestSetHobbyChoices(http::response<http::string_body>& response, const http::request<http::string_body>& request) {
	using namespace rm::game::character;

	try {
		json::value json_body = json::parse(request.body());
		if (!json_body.is_object()) {
			response.result(http::status::bad_request);
			response.set(http::field::content_type, "application/json");
			response.body() = R"({"error": "Invalid request body", "message": "Expected a JSON object"})";
			return;
		}
		std::string id = json_body.as_object().at("id").as_string().c_str();
		CharacterBuilder& builder = serial_manager_.objectManager().get<CharacterBuilder>(id);

		const auto& skills_json = json_body.as_object().at("hobbyRanks").as_array();
		for (const auto& skill_json : skills_json) {
			// Each stat is expected to be an object with "id", optional "subcategory" and "value"	fields
			std::string skill_id = skill_json.as_object().at("id").as_string().c_str();
			std::optional<std::string> subcategory = skill_json.as_object().contains("subcategory") ? std::optional<std::string>(skill_json.as_object().at("subcategory").as_string().c_str()) : std::nullopt;
			int value = static_cast<int>(skill_json.as_object().at("value").as_int64());

			SubcategoriedSkillData& skill_data = serial_manager_.objectManager().subcategoriedSkillData(skill_id, subcategory);
			builder.addHobbySkillRankChoice(skill_data, value);
		}

		const auto& categories_json = json_body.as_object().at("hobbyCategoryRanks").as_array();
		for (const auto& category_json : categories_json) {
			std::string category_id = category_json.as_object().at("id").as_string().c_str();
			int value = static_cast<int>(category_json.as_object().at("value").as_int64());

			SkillCategoryData& category_data = serial_manager_.objectManager().get<SkillCategoryData>(category_id);
			builder.addHobbyCategoryRankChoice(category_data, value);
		}

		const auto& languages_json = json_body.as_object().at("adolescentLanguages").as_array();
		for (const auto& language_json : languages_json) {
			std::string language_id = language_json.as_object().at("language").as_string().c_str();
			int somatic = static_cast<int>(language_json.as_object().at("somatic").as_int64());
			int spoken = static_cast<int>(language_json.as_object().at("spoken").as_int64());
			int written = static_cast<int>(language_json.as_object().at("written").as_int64());

			LanguageData& language_data = serial_manager_.objectManager().get<LanguageData>(language_id);
			LanguageAbility language_ability(language_data, spoken, written, somatic);
			builder.addAdolescentLanguageChoice(std::move(language_ability));
		}

		std::optional<std::string> adolescent_spell_list = get_optional_string(json_body.as_object(), "adolescentSpellList");
		if (adolescent_spell_list.has_value() && !adolescent_spell_list->empty()) {
			SpellListData& adolescent_spell_data = serial_manager_.objectManager().get<SpellListData>(adolescent_spell_list.value());
			builder.setAdolescentSpellListChoice(adolescent_spell_data);
		}

		// Apply all the existing choices
		builder.recalculateAggregatedState();

		response.result(http::status::ok);
		response.set(http::field::content_type, "application/json");
		response.body() = serial_manager_.serializeObject<CharacterBuilder>(builder);
	} catch (const std::exception& e) {
		response.result(http::status::internal_server_error);
		response.body() = R"({"error": "Failed to set hobby choices", "message": ")" + archid::escapeJson(e.what()) + R"("})";
	}
}

void CharacterBuilderRequestHandler::requestSetBackgroundChoices(http::response<http::string_body>& response, const http::request<http::string_body>& request) {
	using namespace rm::game::character;
	try {
		json::value json_body = json::parse(request.body());
		if (!json_body.is_object()) {
			response.result(http::status::bad_request);
			response.set(http::field::content_type, "application/json");
			response.body() = R"({"error": "Invalid request body", "message": "Expected a JSON object"})";
			return;
		}
		std::string id = json_body.as_object().at("id").as_string().c_str();
		CharacterBuilder& builder = serial_manager_.objectManager().get<CharacterBuilder>(id);

		// Stat gain rolls
		bool stat_gains = json_body.as_object().at("statGains").as_bool();
		if (stat_gains) {
			builder.makeBackgroundStatGainRolls();
		}

		// Extra money roll
		int extra_money = static_cast<int>(json_body.as_object().at("extraMoney").as_int64());
		if (extra_money > 1) {
			builder.backgroundMoneyRoll(100); // putting tow points into extar money gives the maximum result
		} else if (extra_money == 1) {
			builder.backgroundMoneyRoll(-1); // Make a random roll for extra money
		}

		// Background languages
		const auto& languages_json = json_body.as_object().at("backgroundLanguages").as_array();
		for (const auto& language_json : languages_json) {
			std::string language_id = language_json.as_object().at("language").as_string().c_str();
			int somatic = static_cast<int>(language_json.as_object().at("somatic").as_int64());
			int spoken = static_cast<int>(language_json.as_object().at("spoken").as_int64());
			int written = static_cast<int>(language_json.as_object().at("written").as_int64());

			LanguageData& language_data = serial_manager_.objectManager().get<LanguageData>(language_id);
			LanguageAbility language_ability(language_data, spoken, written, somatic);
			builder.addBackgroundLanguageChoice(std::move(language_ability));
		}

		// Skill special bonus
		const auto& skills_json = json_body.as_object().at("backgroundSkillBonus").as_array();
		for (const auto& skill_json : skills_json) {
			std::string skill_id = skill_json.as_object().at("id").as_string().c_str();
			int value = static_cast<int>(skill_json.as_object().at("value").as_int64());

			SubcategoriedSkillData& skill_data = serial_manager_.objectManager().subcategoriedSkillData(skill_id);
			builder.addBackgroundSkillSpecialBonus(&skill_data, value);
		}

		// Category special bonus
		const auto& categories_json = json_body.as_object().at("backgroundCategoryBonus").as_array();
		for (const auto& category_json : categories_json) {
			std::string category_id = category_json.as_object().at("id").as_string().c_str();
			int value = static_cast<int>(category_json.as_object().at("value").as_int64());

			SkillCategoryData& category_data = serial_manager_.objectManager().get<SkillCategoryData>(category_id);
			builder.addBackgroundCategorySpecialBonus(&category_data, value);
		}

		// Special items
		int item_count = static_cast<int>(json_body.as_object().at("backgroundItemCount").as_int64());
		builder.generateBackgroundItems(item_count);

		// Apply all the existing choices
		builder.recalculateAggregatedState();

		response.result(http::status::ok);
		response.set(http::field::content_type, "application/json");
		response.body() = serial_manager_.serializeObject<CharacterBuilder>(builder);
	} catch (const std::exception& e) {
		response.result(http::status::internal_server_error);
		response.body() = R"({"error": "Failed to set hobby choices", "message": ")" + archid::escapeJson(e.what()) + R"("})";
	}
}

void CharacterBuilderRequestHandler::requestDump(http::response<http::string_body>& response, const http::request<http::string_body>& request, std::string id) {
	using namespace rm::game::character;

	try {
		CharacterBuilder& builder = serial_manager_.objectManager().get<CharacterBuilder>(id);
		rm::serial::CharacterBuilderSerializer serializer(serial_manager_.objectManager());

		response.result(http::status::ok);
		response.set(http::field::content_type, "application/json");
		response.body() = serial_manager_.serializeObject<CharacterBuilder>(builder);

	} catch (const std::exception& e) {
		response.result(http::status::internal_server_error);
		response.body() = R"({"error": "Failed to dump object", "message": ")" + archid::escapeJson(e.what()) + R"("})";
	}
}

} // namespace rm::rest