#include <set>
#include <CharacterStat.h>
#include <Dice.h>
#include <HttpRequestHandler.h>
#include <StringUtils.h>

namespace rm::rest {

// Helper function to escape JSON strings
std::string escapeJson(const std::string& str) {
	std::ostringstream escaped;
	for (char c : str) {
		switch (c) {
		case '"':
			escaped << "\\\"";
			break;
		case '\\':
			escaped << "\\\\";
			break;
		case '\b':
			escaped << "\\b";
			break;
		case '\f':
			escaped << "\\f";
			break;
		case '\n':
			escaped << "\\n";
			break;
		case '\r':
			escaped << "\\r";
			break;
		case '\t':
			escaped << "\\t";
			break;
		default:
			if (c < 0x20) {
				escaped << "\\u" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(c);
			} else {
				escaped << c;
			}
		}
	}
	return escaped.str();
}

void HttpRequestHandler::handleRequest(const http::request<http::string_body>& request, http::response<http::string_body>& response) {
	// Decode the request target to handle URL-encoded characters
	std::string request_string = archid::uriDecode(request.target());

	// Extract path and query parameters
	const PathParser path(request_string, "/rmce/objects/");

	// Route handling
	if (request.method() == http::verb::get && path.matchExact("/")) {
		response.result(http::status::ok);
		response.set(http::field::content_type, "application/json");
		response.body() = R"({"message": "REST API Server", "status": "running"})";
	} else if (request.method() == http::verb::get && path.matchExact("/health")) {
		response.result(http::status::ok);
		response.set(http::field::content_type, "application/json");
		response.body() = R"({"status": "healthy"})";
	} else if (request.method() == http::verb::get && path.matchExact("/rmce/version")) {
		response.result(http::status::ok);
		response.set(http::field::content_type, "application/json");
		response.body() = R"({"version": "1.0.0", "api": "v1"})";
	} else if (request.method() == http::verb::get && path.matchExact("/rmce/prefixes")) {
		requestPrefixes(response);
	} else if (request.method() == http::verb::get && path.matchExact("/rmce/objects/count") && (path.params().find("types") != path.params().end())) {
		// Get the count of objects of a specific types (e.g. /rmce/objects/count?type=skill)
		const std::string& types = path.params().at("types");
		requestCountMultiTypeObjects(response, types);
	} else if (request.method() == http::verb::get && path.match("/rmce/objects") && !path.type().empty() && path.id().empty()) {
		// We have 3 operations for this endpoint: list, count, and ids. We need to check the operation first before we can determine how to handle the request.

		// First case is with no parameters, which means we want to list all objects of a certain type (e.g. /rmce/objects/skill)
		if (path.params().empty()) {
			requestListObjects(response, path.type());
		} else {
			// Check what the query parameter is for. We support "count" and "ids" for now, but we can easily add more in the future if needed.
			if (path.params().find("ids") != path.params().end()) {
				requestListObjectIds(response, path.type());
			} else if (path.params().find("count") != path.params().end()) {
				requestCountTypeObjects(response, path.type());
			} else {
				response.result(http::status::bad_request);
				response.set(http::field::content_type, "application/json");
				response.body() = R"({"error": "Invalid query parameter", "message": "Supported parameters are 'ids' and 'count'"})";
			}
		}
	} else if (request.method() == http::verb::get && path.match("/rmce/objects") && !path.type().empty() && !path.id().empty()) {
		// Get object by ID
		// Example: /api/objects/skill/SKILL_ACTING
		requestObjectById(response, path.type(), path.id());
	} else if (request.method() == http::verb::post && path.match("/rmce/objects") && !path.type().empty() && path.id().empty()) {
		// Create new object of a certain type (e.g. /rmce/objects/skill)
		// We can use the request body to get the data for the new object, and we can use the type from the path to determine what type of object to create.
		requestCreateObject(response, path.type(), request);
	} else if (request.method() == http::verb::put && path.match("/rmce/objects") && !path.type().empty() && !path.id().empty()) {
		// Update existing object of a certain type and ID (e.g. /rmce/objects/skill/SKILL_ACTING)
		requestUpdateObject(response, path.type(), request);
	} else if (request.method() == http::verb::delete_ && path.match("/rmce/objects") && !path.type().empty() && !path.id().empty()) {
		requestDeleteObject(response, path.type(), path.id());
	} else if (request.method() == http::verb::post && path.match("/rmce/operations/character/stat-rolls")) {
		// Example: /rmce/operations/character/stat-rolls
		// This is a custom operation endpoint that doesn't fit the standard CRUD pattern. We can use this to perform specific operations that may involve multiple objects or complex logic.
		requestCharacterStatRolls(response, request);
	} else {
		response.result(http::status::not_found);
		response.set(http::field::content_type, "application/json");
		response.body() = R"({"error": "Not Found", "message": "The requested resource was not found"})";
	}
}

void HttpRequestHandler::requestPrefixes(http::response<http::string_body>& response) {
	response.result(http::status::ok);
	response.set(http::field::content_type, "application/json");

	try {
		std::set<std::string> prefixes = serial_manager_.objectManager().getAllPrefixes();

		std::ostringstream json;
		std::string json_str = serial_manager_.serializeContainer(prefixes, "prefixes");
		json << json_str;

		response.result(http::status::ok);
		response.body() = json.str();
	} catch (const std::exception& e) {
		response.result(http::status::internal_server_error);
		response.body() = R"({"error": "Failed to retrieve objects", "message": ")" + escapeJson(e.what()) + R"("})";
	}
}

void HttpRequestHandler::requestListObjects(http::response<http::string_body>& response, std::string_view type) {
	response.set(http::field::content_type, "application/json");
	try {
		std::ostringstream json;
		std::string json_str = serial_manager_.serializeAllObjects(type);
		json << json_str;
		response.result(http::status::ok);
		response.body() = json.str();
	} catch (const std::exception& e) {
		response.result(http::status::internal_server_error);
		response.body() = R"({"error": "Failed to retrieve objects", "message": ")" + escapeJson(e.what()) + R"("})";
	}
}

void HttpRequestHandler::requestListObjectIds(http::response<http::string_body>& response, std::string_view type) {
	response.set(http::field::content_type, "application/json");
	try {
		std::set<std::string> ids = serial_manager_.objectManager().getAllIds(type);
		std::string key = serial_manager_.getRootKeyForType(type);
		std::ostringstream json;
		std::string json_str = serial_manager_.serializeContainer(ids, key);
		json << json_str;
		response.result(http::status::ok);
		response.body() = json.str();
	} catch (const std::exception& e) {
		response.result(http::status::internal_server_error);
		response.body() = R"({"error": "Failed to retrieve objects", "message": ")" + escapeJson(e.what()) + R"("})";
	}
}

void HttpRequestHandler::requestCountTypeObjects(http::response<http::string_body>& response, std::string_view type) {
	response.set(http::field::content_type, "application/json");
	try {
		size_t count = serial_manager_.objectManager().getAllIds(type).size(); // Placeholder
		std::ostringstream json;
		json << "{\"count\": " << count << "}";
		response.result(http::status::ok);
		response.body() = json.str();
	} catch (const std::exception& e) {
		response.result(http::status::internal_server_error);
		response.body() = R"({"error": "Failed to retrieve objects", "message": ")" + escapeJson(e.what()) + R"("})";
	}
}

void HttpRequestHandler::requestObjectById(http::response<http::string_body>& response, std::string_view type, std::string_view id) {
	response.set(http::field::content_type, "application/json");
	try {
		std::string obj_json_str = serial_manager_.serializeAnyObject(std::string(id));
		if (obj_json_str.empty()) {
			response.result(http::status::not_found);
			response.body() = R"({"error": "Object not found", "id": ")" + escapeJson(std::string(id)) + R"("})";
		} else {
			response.result(http::status::ok);
			response.body() = obj_json_str;
		}
	} catch (const std::exception& e) {
		response.result(http::status::internal_server_error);
		response.body() = R"({"error": "Failed to retrieve object", "message": ")" + escapeJson(e.what()) + R"("})";
	}
}

void HttpRequestHandler::requestCreateObject(http::response<http::string_body>& response, std::string_view type, const http::request<http::string_body>& request) {
	try {
		json::value json_body = json::parse(request.body());
		if (!json_body.is_object()) {
			response.result(http::status::bad_request);
			response.set(http::field::content_type, "application/json");
			response.body() = R"({"error": "Invalid request body", "message": "Expected a JSON object"})";
			return;
		}
		std::string new_obj_id = serial_manager_.deserializeObject(json_body.as_object(), type);
		serial_manager_.save(type); // Save after creation to persist changes
		response.result(http::status::created);
		response.set(http::field::content_type, "application/json");
		response.body() = R"({"message": "Object created successfully", "id": ")" + escapeJson(new_obj_id) + R"("})";
	} catch (const std::exception& e) {
		response.result(http::status::internal_server_error);
		response.body() = R"({"error": "Failed to create object", "message": ")" + escapeJson(e.what()) + R"("})";
	}
}

void HttpRequestHandler::requestUpdateObject(http::response<http::string_body>& response, std::string_view type, const http::request<http::string_body>& request) {
	try {
		json::value json_body = json::parse(request.body());
		if (!json_body.is_object()) {
			response.result(http::status::bad_request);
			response.set(http::field::content_type, "application/json");
			response.body() = R"({"error": "Invalid request body", "message": "Expected a JSON object"})";
			return;
		}
		std::string id = json_body.as_object().at("id").as_string().c_str();
		if (id.empty()) {
			response.result(http::status::bad_request);
			response.set(http::field::content_type, "application/json");
			response.body() = R"({"error": "Missing object ID", "message": "The JSON object must contain an 'id' field"})";
			return;
		}
		if (!serial_manager_.objectManager().existsAny(id)) {
			response.result(http::status::not_found);
			response.set(http::field::content_type, "application/json");
			response.body() = R"({"error": "Object not found", "id": ")" + escapeJson(id) + R"("})";
			return;
		}
		std::string new_obj_id = serial_manager_.deserializeObject(json_body.as_object(), type);
		serial_manager_.save(type); // Save after update to persist changes
		response.result(http::status::ok);
		response.set(http::field::content_type, "application/json");
		response.body() = R"({"message": "Object updated successfully", "id": ")" + escapeJson(new_obj_id) + R"("})";
	} catch (const std::exception& e) {
		response.result(http::status::internal_server_error);
		response.body() = R"({"error": "Failed to update object", "message": ")" + escapeJson(e.what()) + R"("})";
	}
}

void HttpRequestHandler::requestDeleteObject(http::response<http::string_body>& response, std::string_view type, std::string_view id) {
	response.set(http::field::content_type, "application/json");
	try {
		serial_manager_.objectManager().deleteObject(std::string(id));
		serial_manager_.save(type); // Save after deletion to persist changes
		response.result(http::status::ok);
		response.body() = R"({"result": "Object flagged as deleted", "object": ")" + std::string(id) + R"("})";
	} catch (const std::exception& e) {
		response.result(http::status::internal_server_error);
		response.body() = R"({"error": "Failed to delete object", "message": ")" + escapeJson(e.what()) + R"("})";
	}
}

void HttpRequestHandler::requestCountMultiTypeObjects(http::response<http::string_body>& response, std::string_view types) {
	response.set(http::field::content_type, "application/json");
	try {
		std::vector<std::string> type_list = archid::tokenise(std::string(types), ",");
		std::ostringstream json;
		json << "{ \"counts\": [";
		for (const auto& type : type_list) {
			std::size_t count = serial_manager_.objectManager().getAllIds(type).size();
			json << "{\"type\": \"" << type << "\", \"count\": " << count << "}";
			if (&type != &type_list.back()) {
				json << ",";
			}
		}
		json << "]}";

		response.result(http::status::ok);
		response.body() = json.str();
	} catch (const std::exception& e) {
		response.result(http::status::internal_server_error);
		response.body() = R"({"error": "Failed to retrieve count of objects", "message": ")" + escapeJson(e.what()) + R"("})";
	}
}

void HttpRequestHandler::requestCharacterStatRolls(http::response<http::string_body>& response, const http::request<http::string_body>& request) {
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
		response.body() = R"({"error": "Failed to perform stat rolls", "message": ")" + escapeJson(e.what()) + R"("})";
	}
}

} // namespace rm::rest