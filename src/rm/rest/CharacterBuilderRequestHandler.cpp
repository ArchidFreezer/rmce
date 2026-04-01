#include <CharacterBuilderRequestHandler.h>
#include <CharacterStat.h>
#include <Dice.h>
#include <StringUtils.h>

namespace rm::rest {

void CharacterBuilderRequestHandler::requestCharacterInitialChoices(http::response<http::string_body>& response, const http::request<http::string_body>& request) {
	std::string id{};
	try {
		using namespace rm::game::character;

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
		CharacterBuilder& builder = object_manager_.get<CharacterBuilder>();
		builder.setIntialChoices(object_manager_, name, race_id, culture_id, profession_id, magical_realms);
		id = builder.id();
	} catch (const std::exception&) {
	}
	// This is a placeholder implementation. You can replace it with your actual logic to generate character initial choices based on the request body.
	response.result(http::status::ok);
	response.set(http::field::content_type, "application/json");
	response.body() = R"({"id": ")" + id + R"("})";
}

void CharacterBuilderRequestHandler::requestCharacterStatRolls(http::response<http::string_body>& response, const http::request<http::string_body>& request) {
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

} // namespace rm::rest