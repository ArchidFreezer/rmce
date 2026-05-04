#include <CharacterOperationsRequestHandler.h>
#include <CharacterStat.h>
#include <Dice.h>
#include <HttpPathParser.h>
#include <JsonConverter.h>
#include <StringUtils.h>
#include <boost/json.hpp>
#include <CharacterLeveller.h>

namespace rm::rest {

void CharacterOperationsRequestHandler::handleRequest(const http::request<http::string_body>& request, http::response<http::string_body>& response) {
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
	else if (request.method() == http::verb::post && operation == "set-physique")
		requestPhysiqueChoices(response, request);
	else if (request.method() == http::verb::post && operation == "set-hobby-choices")
		requestSetHobbyChoices(response, request);
	else if (request.method() == http::verb::post && operation == "set-background-choices")
		requestSetBackgroundChoices(response, request);
	else if (request.method() == http::verb::post && operation == "levelup")
		requestLevelUp(response, request);
	else if (request.method() == http::verb::get && operation == "dump" && path.params().contains("id"))
		requestDump(response, request, path.params().at("id"));
	else {
		response.result(http::status::not_found);
		response.set(http::field::content_type, "application/json");
		response.body() = R"({"error": "Endpoint not found", "message": "The requested endpoint does not exist"})";
	}
}

void CharacterOperationsRequestHandler::requestPrimaryDefinition(http::response<http::string_body>& response, const http::request<http::string_body>& request) {
	using namespace rm::game::character;
	using namespace rm::serial;

	try {
		json::value json_body = json::parse(request.body());
		if (!json_body.is_object()) {
			response.result(http::status::bad_request);
			response.set(http::field::content_type, "application/json");
			response.body() = R"({"error": "Invalid request body", "message": "Expected a JSON object"})";
			return;
		}

		// This returns a const object, but we need a non-const reference to update the builder with the choices, so we will deserialize it first to update the cache and then get a non-const reference to it to perform the updates.
		const CharacterBuilder& deserialized = serial_manager_.deserializeObject<CharacterBuilder>(json_body.as_object());
		std::string id = deserialized.id();
		CharacterBuilder& builder = serial_manager_.objectManager().get<CharacterBuilder>(id);

		builder.recalculateAggregatedState();

		response.result(http::status::ok);
		response.set(http::field::content_type, "application/json");
		response.body() = serial_manager_.serializeObject<CharacterBuilder>(builder);
	} catch (const std::exception& e) {
		response.result(http::status::internal_server_error);
		response.body() = R"({"error": "Failed to set primary choices", "message": ")" + archid::escapeJson(e.what()) + R"("})";
	}
}

void CharacterOperationsRequestHandler::requestPhysiqueChoices(http::response<http::string_body>& response, const http::request<http::string_body>& request) {
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

		builder.generatePhysique();

		response.result(http::status::ok);
		response.set(http::field::content_type, "application/json");
		response.body() = serial_manager_.serializeObject<CharacterBuilder>(builder);
		int x{};
	} catch (const std::exception& e) {
		response.result(http::status::internal_server_error);
		response.body() = R"({"error": "Failed to generate physique", "message": ")" + archid::escapeJson(e.what()) + R"("})";
	}
}

void CharacterOperationsRequestHandler::requestPrimaryChoices(http::response<http::string_body>& response, const http::request<http::string_body>& request) {
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

		response.result(http::status::ok);
		response.set(http::field::content_type, "application/json");
		response.body() = serial_manager_.serializeObject<CharacterBuilder>(builder);
		int x{};
	} catch (const std::exception& e) {
		response.result(http::status::internal_server_error);
		response.body() = R"({"error": "Failed to generate primary choices", "message": ")" + archid::escapeJson(e.what()) + R"("})";
	}
}

void CharacterOperationsRequestHandler::requestStatRolls(http::response<http::string_body>& response, const http::request<http::string_body>& request) {
	try {
		json::value json_body = json::parse(request.body());
		if (!json_body.is_array()) {
			response.result(http::status::bad_request);
			response.set(http::field::content_type, "application/json");
			response.body() = R"({"error": "Invalid request body", "message": "Expected a JSON array"})";
			return;
		}

		// Extract the data from the requestbody into a vector of ints for easier processing. We will validate the data in the next step, but we need to extract it first to perform the validation.
		std::vector<int> data{};
		for (const auto& element : json_body.as_array()) {
			if (!element.is_object()) {
				response.result(http::status::bad_request);
				response.set(http::field::content_type, "application/json");
				response.body() = R"({"error": "Invalid request body", "message": "Each element must be a JSON object"})";
				return;
			}
			const auto& obj = element.as_object();
			int temporary = static_cast<int>(obj.at("temporary").as_int64());
			data.push_back(temporary);
		}

		// Make sure that we have no stats <25 and at least 2 >=90
		rm::game::character::ensureValidTemporaryStats(data, 25, 90, 2);

		// Now we know we have valid stats get the potentials and place them in the reults array.
		json::array result_array;
		for (int temporary : data) {
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

void CharacterOperationsRequestHandler::requestSetStats(http::response<http::string_body>& response, const http::request<http::string_body>& request) {
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
		bool auto_build = json_body.as_object().at("autoBuild").as_bool();

		// This returns a const object, but we need a non-const reference to update the builder with the choices, so we will deserialize it first to update the cache and then get a non-const reference to it to perform the updates.
		const CharacterBuilder& deserialized = serial_manager_.deserializeObject<CharacterBuilder>(json_body.as_object());
		CharacterBuilder& builder = serial_manager_.objectManager().get<CharacterBuilder>(id);

		if (auto_build) {
			builder.autoStats();
		}
		builder.recalculateAggregatedState();

		response.result(http::status::ok);
		response.set(http::field::content_type, "application/json");
		response.body() = serial_manager_.serializeObject<CharacterBuilder>(builder);
	} catch (const std::exception& e) {
		response.result(http::status::internal_server_error);
		response.body() = R"({"error": "Failed to set stats", "message": ")" + archid::escapeJson(e.what()) + R"("})";
	}
}

void CharacterOperationsRequestHandler::requestSetHobbyChoices(http::response<http::string_body>& response, const http::request<http::string_body>& request) {
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

		response.result(http::status::ok);
		response.set(http::field::content_type, "application/json");
		response.body() = serial_manager_.serializeObject<CharacterBuilder>(builder);
	} catch (const std::exception& e) {
		response.result(http::status::internal_server_error);
		response.body() = R"({"error": "Failed to set hobby choices", "message": ")" + archid::escapeJson(e.what()) + R"("})";
	}
}

void CharacterOperationsRequestHandler::requestSetBackgroundChoices(http::response<http::string_body>& response, const http::request<http::string_body>& request) {
	using namespace rm::game::character;
	using namespace rm::serial;
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
			int somatic = JsonConverter::getInt(language_json.as_object(), "somatic", 0);
			int spoken = JsonConverter::getInt(language_json.as_object(), "spoken", 0);
			int written = JsonConverter::getInt(language_json.as_object(), "written", 0);

			LanguageData& language_data = serial_manager_.objectManager().get<LanguageData>(language_id);
			LanguageRanks language_ability(language_data, spoken, written, somatic);
			builder.addBackgroundLanguageChoice(std::move(language_ability));
		}

		// Skill special bonus
		const auto& skills_json = json_body.as_object().at("backgroundSkillBonus").as_array();
		for (const auto& skill_json : skills_json) {
			std::string skill_id = skill_json.as_object().at("id").as_string().c_str();
			std::optional<std::string> subcategory = JsonConverter::getOptionalString(skill_json.as_object(), "subcategory");
			int value = JsonConverter::getInt(skill_json.as_object(), "value", 0);

			SubcategoriedSkillData& skill_data = serial_manager_.objectManager().subcategoriedSkillData(skill_id, subcategory);
			builder.addBackgroundSkillSpecialBonus(&skill_data, value);
		}

		// Category special bonus
		const auto& categories_json = json_body.as_object().at("backgroundCategoryBonus").as_array();
		for (const auto& category_json : categories_json) {
			std::string category_id = category_json.as_object().at("id").as_string().c_str();
			int value = JsonConverter::getInt(category_json.as_object(), "value", 0);

			SkillCategoryData& category_data = serial_manager_.objectManager().get<SkillCategoryData>(category_id);
			builder.addBackgroundCategorySpecialBonus(&category_data, value);
		}

		std::map<const SpellListData*, int> bonuses = JsonConverter::getDataPrimitiveMap<SpellListData, int>(json_body.as_object(), "spellListSpecialBonuses", serial_manager_.objectManager());
		builder.setSpellListSpecialBonuses(std::move(bonuses));
	
		// Special items
		int item_count = static_cast<int>(json_body.as_object().at("backgroundItemCount").as_int64());
		builder.generateBackgroundItems(item_count);

		// Apply all the existing choices
		builder.recalculateAggregatedState();

		// Now we have all the updates to the character we can create the character object at level 0.
		Character& character = builder.build();

		serial_manager_.objectManager().deleteObject(id); // We can delete the builder from the cache as it is no longer needed after the character has been built

		response.result(http::status::ok);
		response.set(http::field::content_type, "application/json");
		response.body() = serial_manager_.serializeObject<Character>(character);
	} catch (const std::exception& e) {
		response.result(http::status::internal_server_error);
		response.body() = R"({"error": "Failed to set hobby choices", "message": ")" + archid::escapeJson(e.what()) + R"("})";
	}
}

void CharacterOperationsRequestHandler::requestLevelUp(http::response<http::string_body>& response, const http::request<http::string_body>& request) {
	using namespace rm::game::character;

	try {
		std::string request_body = request.body();
		json::value json_body = json::parse(request.body());
		if (!json_body.is_object()) {
			response.result(http::status::bad_request);
			response.set(http::field::content_type, "application/json");
			response.body() = R"({"error": "Invalid request body", "message": "Expected a JSON object"})";
			return;
		}
		// This API is called twice, the first time with only the character ID which expects the training package costs to be populated and then a second time with a payload of the selected levelling choices. To handle this we will check if
		// the leveller id has been set on the leveller object, if not we will populate the training package costs and return, if it has been set we know that the client is sending the levelling choices and we can perform the level up operation.
		std::string character_id = json_body.as_object().at("character").as_string().c_str();
		std::string id = json_body.as_object().at("id").as_string().c_str();
		bool has_id = !id.empty();

		// This returns a const object, but we need a non-const reference to update the leveller with the choices, so we will deserialize it first to update the cache and then get a non-const reference to it to perform the updates.
		const CharacterLeveller& deserialized = serial_manager_.deserializeObject<CharacterLeveller>(json_body.as_object());
		id = deserialized.id();
		CharacterLeveller& leveller = serial_manager_.objectManager().get<CharacterLeveller>(id);

		if (!has_id) {
			leveller.buildTrainingPackageCosts();
			leveller.buildSpellListCosts();
			response.body() = serial_manager_.serializeObject<CharacterLeveller>(id);
		} else {
			leveller.levelUp();
			serial_manager_.objectManager().deleteObject(id); // We can delete the leveller from the cache as it is no longer needed after the character has been built
			response.body() = serial_manager_.serializeObject<Character>(character_id);
		}

		response.result(http::status::ok);
		response.set(http::field::content_type, "application/json");
	} catch (const std::exception& e) {
		response.result(http::status::internal_server_error);
		response.body() = R"({"error": "Failed to set apprenticeship choices", "message": ")" + archid::escapeJson(e.what()) + R"("})";
	}
}

void CharacterOperationsRequestHandler::requestDump(http::response<http::string_body>& response, const http::request<http::string_body>& request, std::string id) {
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