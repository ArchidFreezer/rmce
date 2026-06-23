#include <set>
#include <CharacterOperationsRequestHandler.h>
#include <HttpPathParser.h>
#include <HttpRequestHandler.h>
#include <DataRequestHandler.h>
#include <ObjectRequestHandler.h>
#include <StringUtils.h>

namespace rm::rest {

void HttpRequestHandler::handleRequest(const http::request<http::string_body>& request, http::response<http::string_body>& response) {
	// Decode the request target to handle URL-encoded characters
	std::string request_string = archid::uriDecode(request.target());

	// Extract path and query parameters
	const HttpPathParser path(request_string);

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
	} else if (request.method() == http::verb::get && path.matchExact("/rmce/save")) {
		try {
			serial_manager_.save();
			response.result(http::status::ok);
			response.set(http::field::content_type, "application/json");
			response.body() = R"({"save": "successful"})";
		} catch (const std::exception& e) {
			response.result(http::status::internal_server_error);
			response.set(http::field::content_type, "application/json");
			response.body() = R"({"error": "Save Failed", "message": ")" + std::string(e.what()) + R"("})";
		}
	} else if (request.method() == http::verb::get && path.matchExact("/rmce/reload")) {
		try {
			serial_manager_.resetCache();
			serial_manager_.load();
			response.result(http::status::ok);
			response.set(http::field::content_type, "application/json");
			response.body() = R"({"reload": "successful"})";
		} catch (const std::exception& e) {
			response.result(http::status::internal_server_error);
			response.set(http::field::content_type, "application/json");
			response.body() = R"({"error": "Reload Failed", "message": ")" + std::string(e.what()) + R"("})";
		}
	} else if (path.match("/rmce/data/")) {
		DataRequestHandler handler {serial_manager_};
		handler.handleRequest(request, response);
	} else if (path.match("/rmce/objects/")) {
		ObjectRequestHandler handler{serial_manager_};
		handler.handleRequest(request, response);
	} else if (path.match("/rmce/operations/character/")) {
		CharacterOperationsRequestHandler handler {serial_manager_};
		handler.handleRequest(request, response);
	} else {
		response.result(http::status::not_found);
		response.set(http::field::content_type, "application/json");
		response.body() = R"({"error": "Not Found", "message": "The requested resource was not found"})";
	}
}

} // namespace rm::rest