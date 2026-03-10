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
	const PathParser path(request.target(), "/api/objects/");

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
	} else if (request.method() == http::verb::get && path.match("/api/objects/") && !path.type().empty() && (path.op() == "list")) {
		// List all object IDs
		// Example: /api/objects/skill/list
		response.set(http::field::content_type, "application/json");

		try {
			std::ostringstream json;
			// json << "{\"objects\": [";
			std::string json_str = serial_manager_.serializeAllObjects(path.type());
			json << json_str;
			// json << "], \"count\": " << objects.size() << "}";

			response.result(http::status::ok);
			response.body() = json.str();
		} catch (const std::exception& e) {
			response.result(http::status::internal_server_error);
			response.body() = R"({"error": "Failed to retrieve objects", "message": ")" + escapeJson(e.what()) + R"("})";
		}
	} else if (request.method() == http::verb::get && path.matchExact("/api/objects")) {
		// Get object by ID
		// Example: /api/objects?id=SKILL_123
		response.set(http::field::content_type, "application/json");

		auto id_it = path.params().find("id");
		if (id_it == path.params().end()) {
			response.result(http::status::bad_request);
			response.body() = R"({"error": "Missing 'id' parameter"})";
		} else {
			try {
				const std::string& id = id_it->second;
				// Get the JSON representation of the object by ID
				auto obj_json_str = serial_manager_.serializeAnyObject(id);
				if (obj_json_str.empty()) {
					// Wew should never get here if the object manager is implemented correctly, but just in case
					response.result(http::status::not_found);
					response.body() = R"({"error": "Object not found", "id": ")" + escapeJson(id) + R"("})";
				} else {
					response.result(http::status::ok);
					response.body() = obj_json_str;
				}
			} catch (const std::exception& e) {
				// This could be as simple as an incorrect prefix in the ID (e.g. "SKILLS_" instead of "SKILL_") or a more serious issue with the object manager
				response.result(http::status::internal_server_error);
				response.body() = R"({"error": "Failed to retrieve object", "message": ")" + escapeJson(e.what()) + R"("})";
			}
		}
	} else if (request.method() == http::verb::get && path.match("/api/objects/") && !path.type().empty() && (path.op() == "count")) {
		// Get count of objects
		response.set(http::field::content_type, "application/json");

		try {
			const size_t count = serial_manager_.objectManager().getAllIds(path.type()).size(); // Placeholder

			std::ostringstream json;
			json << "{\"count\": " << count << "}";

			response.result(http::status::ok);
			response.body() = json.str();
		} catch (const std::exception& e) {
			response.result(http::status::internal_server_error);
			response.body() = R"({"error": "Failed to get count", "message": ")" + escapeJson(e.what()) + R"("})";
		}
	} else if (request.method() == http::verb::get && path.match("/api/objects/") && !path.type().empty() && (path.op() == "ids")) {
		// Get count of objects
		response.set(http::field::content_type, "application/json");

		try {
			const std::set<std::string> ids = serial_manager_.objectManager().getAllIds(path.type());
			std::string key = serial_manager_.getRootKeyForType(path.type());

			std::ostringstream json;
			json << serial_manager_.serializeContainer(ids, key);

			response.result(http::status::ok);
			response.body() = json.str();
		} catch (const std::exception& e) {
			response.result(http::status::internal_server_error);
			response.body() = R"({"error": "Failed to get ids", "message": ")" + escapeJson(e.what()) + R"("})";
		}
	} else if (request.method() == http::verb::get && path.matchExact("/api/echo")) {
		// Echo endpoint for testing
		response.result(http::status::ok);
		response.set(http::field::content_type, "application/json");

		std::ostringstream json;
		json << "{\"query_params\": {";

		bool first = true;
		for (const auto& [key, value] : path.params()) {
			if (!first)
				json << ", ";
			json << "\"" << escapeJson(key) << "\": \"" << escapeJson(value) << "\"";
			first = false;
		}

		json << "}}";
		response.body() = json.str();
	} else {
		response.result(http::status::not_found);
		response.set(http::field::content_type, "application/json");
		response.body() = R"({"error": "Not Found", "message": "The requested resource was not found"})";
	}
}

} // namespace rm::rest