#pragma once

#include <boost/beast/http.hpp>
#include <PersistentObjectSerializationManager.h>

// Forward declarations
namespace rm {
class PersistentObjectSerializationManager;
} // namespace rm

namespace http = boost::beast::http;

namespace rm::rest {

/**
 * @class HttpRequestHandler
 * @brief Handles incoming HTTP requests and prepares responses based on the request path and parameters
 *
 * This class is responsible for processing HTTP requests, extracting relevant information from the request path and query parameters, and generating appropriate HTTP responses. It uses the PersistentObjectSerializationManager to access
 * data as needed for certain endpoints.
 */
class HttpRequestHandler {
public:
	/**
	 * @brief Constructor to initialize the HttpRequestHandler with a reference to the PersistentObjectSerializationManager
	 * @param serial_manager Reference to PersistentObjectSerializationManager for data access during request handling
	 */
	HttpRequestHandler(PersistentObjectSerializationManager& serial_manager) : serial_manager_{serial_manager} {};

	/**
	 * @brief Handle an incoming HTTP request and prepare the response
	 * @param request The HTTP request to handle
	 * @param response The HTTP response to populate based on the request
	 */
	void handleRequest(const http::request<http::string_body>& request, http::response<http::string_body>& response);

private:
	PersistentObjectSerializationManager& serial_manager_;

};

} // namespace rm::rest
