#include <set>
#include <HttpRequestHandler.h>

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
	// Extract path and query parameters
	const PathParser path(request.target(), "/rmce/objects/");

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
	} else if (request.method() == http::verb::get && path.match("/rmce/objects") && !path.type().empty() && path.id().empty()) {
		// We have 3 operations for this endpoint: list, count, and ids. We need to check the operation first before we can determine how to handle the request.

		// First case is with no parameters, which means we want to list all objects of a certain type (e.g. /api/objects/skill)
		if (path.params().empty()) {
			requestListObjects(response, path.type());
		} else {
			// Check what the query parameter is for. We support "count" and "ids" for now, but we can easily add more in the future if needed.
			if (path.params().find("ids") != path.params().end()) {
				requestListObjectIds(response, path.type());
			} else if (path.params().find("count") != path.params().end()) {
				requestCountObjects(response, path.type());
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
		// Create new object of a certain type (e.g. /api/objects/skill)
		// We can use the request body to get the data for the new object, and we can use the type from the path to determine what type of object to create.
		requestCreateObject(response, path.type(), request);
	} else if (request.method() == http::verb::delete_ && path.match("/rmce/objects") && !path.type().empty() && !path.id().empty()) {
		requestDeleteObject(response, path.type(), path.id());
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

void HttpRequestHandler::requestCountObjects(http::response<http::string_body>& response, std::string_view type) {
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
		response.result(http::status::created);
		response.set(http::field::content_type, "application/json");
		response.body() = R"({"message": "Object created/updated successfully", "id": ")" + escapeJson(new_obj_id) + R"("})";
	} catch (const std::exception& e) {
		response.result(http::status::internal_server_error);
		response.body() = R"({"error": "Failed to create object", "message": ")" + escapeJson(e.what()) + R"("})";
	}

}

void HttpRequestHandler::requestDeleteObject(http::response<http::string_body>& response, std::string_view type, std::string_view id) {
	response.set(http::field::content_type, "application/json");
	try {
		serial_manager_.objectManager().deleteObject(std::string(id));
		response.result(http::status::ok);
		response.body() = R"({"result": "Object flagged as deleted", "object": ")" + std::string(id) + R"("})";
	} catch (const std::exception& e) {
		response.result(http::status::internal_server_error);
		response.body() = R"({"error": "Failed to delete object", "message": ")" + escapeJson(e.what()) + R"("})";
	}
}

} // namespace rm::rest