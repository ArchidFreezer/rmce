#include <ObjectRequestHandler.h>
#include <HttpPathParser.h>
#include <StringUtils.h>

namespace rm::rest {

void ObjectRequestHandler::handleRequest(const http::request<http::string_body>& request, http::response<http::string_body>& response) {
	// Decode the request target to handle URL-encoded characters
	std::string request_string = archid::uriDecode(request.target());
	// Extract path and query parameters
	const HttpPathParser path(request_string);
	// Get the type prefix from the path (e.g. "skill" from "/rmce/objects/skill")
	std::string type_prefix = std::string(path.extractNextSegment("/rmce/objects/"));
	// Get the ID from the path if it exists (e.g. "SKILL_ACTING" from "/rmce/objects/skill/SKILL_ACTING")
	std::string id = type_prefix.empty() ? "" : std::string(path.extractNextSegment("/rmce/objects/" + type_prefix + "/"));

	/*
	 * Handle GET requests
	 */
	if (request.method() == http::verb::get) {
		if (path.matchExact("/rmce/objects/count") && (path.params().find("types") != path.params().end())) {
			// Get the count of objects of a specific types (e.g. /rmce/objects/count?type=skill,race)
			const std::string& types = path.params().at("types");
			requestCountMultiTypeObjects(response, types);
		} else if (path.matchExact("/rmce/objects/prefixes")) {
			requestPrefixes(response);
		} else if (!type_prefix.empty() && id.empty()) {
			// We have 3 operations for this endpoint: list, count, and ids. We need to check the operation first before we can determine how to handle the request.

			// First case is with no parameters, which means we want to list all objects of a certain type (e.g. /rmce/objects/skill)
			if (path.params().empty()) {
				requestListObjects(response, type_prefix);
			} else {
				// Check what the query parameter is for. We support "count" and "ids" for now, but we can easily add more in the future if needed.
				if (path.params().find("ids") != path.params().end()) {
					requestListObjectIds(response, type_prefix);
				} else if (path.params().find("count") != path.params().end()) {
					requestCountTypeObjects(response, type_prefix);
				} else {
					response.result(http::status::bad_request);
					response.set(http::field::content_type, "application/json");
					response.body() = R"({"error": "Invalid query parameter", "message": "Supported parameters are 'ids' and 'count'"})";
				}
			}
		} else if (!type_prefix.empty() && !id.empty()) {
			// Get object by ID
			// Example: /api/objects/skill/SKILL_ACTING
			requestObjectById(response, type_prefix, id);
		}
	}

	/*
	 * Handle POST requests
	 */
	else if (request.method() == http::verb::post && path.match("/rmce/objects") && !type_prefix.empty()) {
		requestCreateObject(response, type_prefix, request);
	}

	/*
	 * Handle PUT requests
	 */
	else if (request.method() == http::verb::put && path.match("/rmce/objects") && !type_prefix.empty() && !id.empty()) {
		requestUpdateObject(response, type_prefix, request);
	}

	/*
	 * Handle DELETE requests
	 */
	else if (request.method() == http::verb::delete_ && path.match("/rmce/objects") && !type_prefix.empty() && !id.empty()) {
		requestDeleteObject(response, type_prefix, id);
	}

	/*
	 * Handle unknown requests
	 */
	else {
		response.result(http::status::not_found);
		response.set(http::field::content_type, "application/json");
		response.body() = R"({"error": "Endpoint not found", "message": "The requested endpoint does not exist"})";
	}
}

void ObjectRequestHandler::requestPrefixes(http::response<http::string_body>& response) {
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
		response.body() = R"({"error": "Failed to retrieve objects", "message": ")" + archid::escapeJson(e.what()) + R"("})";
	}
}

void ObjectRequestHandler::requestListObjects(http::response<http::string_body>& response, std::string_view type) {
	response.set(http::field::content_type, "application/json");
	try {
		std::ostringstream json;
		std::string json_str = serial_manager_.serializeAllObjects(type);
		json << json_str;
		response.result(http::status::ok);
		response.body() = json.str();
	} catch (const std::exception& e) {
		response.result(http::status::internal_server_error);
		response.body() = R"({"error": "Failed to retrieve objects", "message": ")" + archid::escapeJson(e.what()) + R"("})";
	}
}

void ObjectRequestHandler::requestListObjectIds(http::response<http::string_body>& response, std::string_view type) {
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
		response.body() = R"({"error": "Failed to retrieve objects", "message": ")" + archid::escapeJson(e.what()) + R"("})";
	}
}

void ObjectRequestHandler::requestCountTypeObjects(http::response<http::string_body>& response, std::string_view type) {
	response.set(http::field::content_type, "application/json");
	try {
		size_t count = serial_manager_.objectManager().getAllIds(type).size(); // Placeholder
		std::ostringstream json;
		json << "{\"count\": " << count << "}";
		response.result(http::status::ok);
		response.body() = json.str();
	} catch (const std::exception& e) {
		response.result(http::status::internal_server_error);
		response.body() = R"({"error": "Failed to retrieve objects", "message": ")" + archid::escapeJson(e.what()) + R"("})";
	}
}

void ObjectRequestHandler::requestObjectById(http::response<http::string_body>& response, std::string_view type, std::string_view id) {
	response.set(http::field::content_type, "application/json");
	try {
		std::string obj_json_str = serial_manager_.serializeAnyObject(std::string(id));
		if (obj_json_str.empty()) {
			response.result(http::status::not_found);
			response.body() = R"({"error": "Object not found", "id": ")" + archid::escapeJson(std::string(id)) + R"("})";
		} else {
			response.result(http::status::ok);
			response.body() = obj_json_str;
		}
	} catch (const std::exception& e) {
		response.result(http::status::internal_server_error);
		response.body() = R"({"error": "Failed to retrieve object", "message": ")" + archid::escapeJson(e.what()) + R"("})";
	}
}

void ObjectRequestHandler::requestCreateObject(http::response<http::string_body>& response, std::string_view type, const http::request<http::string_body>& request) {
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
		response.body() = R"({"message": "Object created successfully", "id": ")" + archid::escapeJson(new_obj_id) + R"("})";
	} catch (const std::exception& e) {
		response.result(http::status::internal_server_error);
		response.body() = R"({"error": "Failed to create object", "message": ")" + archid::escapeJson(e.what()) + R"("})";
	}
}

void ObjectRequestHandler::requestUpdateObject(http::response<http::string_body>& response, std::string_view type, const http::request<http::string_body>& request) {
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
			response.body() = R"({"error": "Object not found", "id": ")" + archid::escapeJson(id) + R"("})";
			return;
		}
		std::string new_obj_id = serial_manager_.deserializeObject(json_body.as_object(), type);
		serial_manager_.save(type); // Save after update to persist changes
		response.result(http::status::ok);
		response.set(http::field::content_type, "application/json");
		response.body() = R"({"message": "Object updated successfully", "id": ")" + archid::escapeJson(new_obj_id) + R"("})";
	} catch (const std::exception& e) {
		response.result(http::status::internal_server_error);
		response.body() = R"({"error": "Failed to update object", "message": ")" + archid::escapeJson(e.what()) + R"("})";
	}
}

void ObjectRequestHandler::requestDeleteObject(http::response<http::string_body>& response, std::string_view type, std::string_view id) {
	response.set(http::field::content_type, "application/json");
	try {
		serial_manager_.objectManager().deleteObject(std::string(id));
		serial_manager_.save(type); // Save after deletion to persist changes
		response.result(http::status::ok);
		response.body() = R"({"result": "Object flagged as deleted", "object": ")" + std::string(id) + R"("})";
	} catch (const std::exception& e) {
		response.result(http::status::internal_server_error);
		response.body() = R"({"error": "Failed to delete object", "message": ")" + archid::escapeJson(e.what()) + R"("})";
	}
}

void ObjectRequestHandler::requestCountMultiTypeObjects(http::response<http::string_body>& response, std::string_view types) {
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
		response.body() = R"({"error": "Failed to retrieve count of objects", "message": ")" + archid::escapeJson(e.what()) + R"("})";
	}
}
} // namespace rm::rest