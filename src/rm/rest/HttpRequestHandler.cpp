#include <set>
#include <CharacterBuilderRequestHandler.h>
#include <HttpPathParser.h>
#include <HttpRequestHandler.h>
#include <ObjectRequestHandler.h>
#include <StringUtils.h>

namespace rm::rest {

void HttpRequestHandler::handleRequest(const http::request<http::string_body>& request, http::response<http::string_body>& response) {
	// Decode the request target to handle URL-encoded characters
	std::string request_string = archid::uriDecode(request.target());

	// Extract path and query parameters
	const HttpPathParser path(request_string);

	// Get the type prefix from the path (e.g. "skill" from "/rmce/objects/skill")
	std::string type_prefix = std::string(path.extractNextSegment("/rmce/objects/"));
	// Get the ID from the path if it exists (e.g. "SKILL_ACTING" from "/rmce/objects/skill/SKILL_ACTING")
	std::string id = type_prefix.empty() ? "" : std::string(path.extractNextSegment("/rmce/objects/" + type_prefix + "/"));


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
	} else if (path.match("/rmce/objects/")) {
		ObjectRequestHandler handler {serial_manager_};
		handler.handleRequest(request, response);
	} else if (path.match("/rmce/operations/character/")) {
		CharacterBuilderRequestHandler handler {serial_manager_};
		handler.handleRequest(request, response);
	} else {
		response.result(http::status::not_found);
		response.set(http::field::content_type, "application/json");
		response.body() = R"({"error": "Not Found", "message": "The requested resource was not found"})";
	}
}

} // namespace rm::rest