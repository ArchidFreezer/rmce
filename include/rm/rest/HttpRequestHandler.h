#pragma once

#include <map>
#include <string>
#include <string_view>
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

	void requestPrefixes(http::response<http::string_body>& response);

	void requestListObjects(http::response<http::string_body>& response, std::string_view type);

	void requestListObjectIds(http::response<http::string_body>& response, std::string_view type);

	void requestCountObjects(http::response<http::string_body>& response, std::string_view type);

	void requestObjectById(http::response<http::string_body>& response, std::string_view type, std::string_view id);
};

/**
 * @class PathParser
 * @brief Utility class for parsing request paths and query parameters
 *
 * This handles the specific case of object retrieval endpoints, where the path may contain variable segments (e.g., "api/objects/{type}/get?id=ABC").
 */
class PathParser {
public:
	/**
	 * @brief Constructs a PathParser object to parse a path with a given prefix.
	 * @param path The path string to be parsed.
	 * @param prefix The prefix string used to extract variable, operation, and pre-parameter path information.
	 */
	explicit PathParser(std::string_view path, std::string_view prefix) : path_(path), type_{extractVariable(prefix)}, id_{extractId(prefix)}, pre_param_path_{extractPreParamPath()} {
		parseQueryParams(path);
	}

	/**
	 * @brief Checks if the pre-parameter path exactly matches the given pattern.
	 * @param pattern The string pattern to compare against the pre-parameter path.
	 * @return True if the pre-parameter path exactly matches the pattern, false otherwise.
	 */
	bool matchExact(std::string_view pattern) const {
		return pre_param_path_ == pattern;
	}

	/**
	 * @brief Checks if the pre-parameter path starts with the given pattern.
	 * @param pattern The string pattern to check as a prefix of the pre-parameter path.
	 * @return True if the pre-parameter path starts with the pattern, false otherwise.
	 */
	bool match(std::string_view pattern) const {
		return path_.starts_with(pattern);
	}

	/**
	 * @brief Gets the parameters map.
	 * @return A constant reference to the map containing string key-value pairs of parameters.
	 */
	const std::map<std::string, std::string>& params() const {
		return params_;
	}

	/**
	 * @brief Gets the type of the object to be processed.
	 * @return A string view representing the type.
	 */
	std::string_view type() const {
		return type_;
	}

	/**
	 * @brief Gets the operation to be performed on the object.
	 * @return A string view representing the operation.
	 */
	std::string_view id() const {
		return id_;
	}

private:
	std::string_view path_{};
	std::string_view type_{};
	std::string_view id_{};
	std::string_view pre_param_path_{};
	std::map<std::string, std::string> params_{};

	// Create a map of query parameters from the target string (e.g., "id=ABC&name=XYZ")
	void parseQueryParams(std::string_view target) {
		std::map<std::string, std::string> params;

		// Find the query string start
		auto query_pos = target.find('?');
		if (query_pos == std::string_view::npos) {
			params_ = params;
		}

		// Extract query string
		std::string_view query = target.substr(query_pos + 1);

		// Parse key=value pairs
		size_t start = 0;
		while (start < query.length()) {
			// Find next parameter separator
			auto amp_pos = query.find('&', start);
			auto param = query.substr(start, amp_pos - start);

			// Split on '='
			auto eq_pos = param.find('=');
			if (eq_pos != std::string_view::npos) {
				std::string key(param.substr(0, eq_pos));
				std::string value(param.substr(eq_pos + 1));

				// URL decode (basic implementation)
				// Replace '+' with space and handle %XX encoding
				for (size_t i = 0; i < value.length(); ++i) {
					if (value[i] == '+') {
						value[i] = ' ';
					}
				}

				params[key] = value;
			} else {
				// Handle case where there is a key with no value (e.g., "flag" instead of "flag=true")
				std::string key(param);
				params[key] = "";
			}

			if (amp_pos == std::string_view::npos)
				break;
			start = amp_pos + 1;
		}

		params_ = params;
	}

	// Extract the variable part of the path after the pattern and before the next segment
	std::string_view extractVariable(std::string_view pattern) const {
		if (match(pattern)) {
			// First get the part of the path after the pattern
			std::string_view postfix = path_.substr(pattern.size());
			// Then find the next '/' which indicates the end of the variable segment
			size_t nextSlash = postfix.find('/');
			if (nextSlash == std::string_view::npos) {
				// Get up the query parameters if there are any
				size_t queryPos = postfix.find('?');
				if (queryPos != std::string_view::npos) {
					return postfix.substr(0, queryPos); // Return the variable segment without query parameters
				} else {
					return postfix; // Return the variable segment
				}
			} else {
				return postfix.substr(0, nextSlash); // Return the variable segment
			}
		}
		return {};
	}

	std::string_view extractId(std::string_view pattern) const {
		if (match(pattern)) {
			// First get the part of the path after the pattern
			std::string_view postfix = path_.substr(pattern.size());
			// Then find the next '/' which indicates the end of the variable segment
			size_t nextSlash = postfix.find('/');
			if (nextSlash != std::string_view::npos) {
				// Get up the query parameters if there are any
				size_t queryPos = postfix.find('?');
				if (queryPos != std::string_view::npos) {
					return postfix.substr(nextSlash + 1, queryPos - nextSlash - 1);
				} else {
					return postfix.substr(nextSlash + 1); // Return the operation segment
				}
			}
		}
		return {};
	}

	std::string_view extractPreParamPath() const {
		size_t queryPos = path_.find('?');
		if (queryPos != std::string_view::npos) {
			return path_.substr(0, queryPos);
		} else {
			return path_;
		}
	}
};

} // namespace rm::rest
