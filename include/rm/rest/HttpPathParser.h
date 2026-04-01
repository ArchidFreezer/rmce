#pragma once

#include <map>
#include <string>
#include <string_view>

namespace rm::rest {
/**
 * @class HttpPathParser
 * @brief Utility class for parsing request paths and query parameters
 *
 * This class provides methods to extract segments and query parameters from a URL path and to match the path against specific patterns. It is designed to simplify the handling of REST API endpoints by allowing easy extraction of variable
 * segments and query parameters from the request path.
 *
 * The general usage pattern is to construct an HttpPathParser with the request target (which should be URL-decoded), and then use the matchExact, match, and extractNextSegment methods to determine which endpoint is being accessed and to
 * extract any variable segments. The params method can be used to access any query parameters as a map of key-value pairs.
 *
 * @code
 * const http::request<http::string_body>& request = ...; // Assume this is provided
 * // Decode the request target to handle URL-encoded characters
 * std::string request_string = archid::uriDecode(request.target());
 * const HttpPathParser path_parser(request_string);
 * @endcode
 *
 * @warning This class assumes that the input path is already URL-decoded and does not perform any decoding itself. It also does not handle URL encoding in query parameters, so it is expected that any necessary decoding has been done before
 * using this class. Internally data is stored as string_views to avoid unnecessary copying, so the input path must remain valid for the lifetime of the HttpPathParser object. In practice this means that the HttpPathParser should be
 * constructed with a string_view of the request target, which is valid for the duration of the request handling.
 *
 * This handles the specific case of object retrieval endpoints, where the path may contain variable segments (e.g., "api/objects/{type}/get?id=ABC").
 */
class HttpPathParser {
public:
	/**
	 * @brief Constructs a PathParser object to parse a path with a given prefix.
	 * @param path The path string to be parsed.
	 */
	explicit HttpPathParser(std::string_view path) : path_(path), pre_param_path_{extractPreParamPath(path)} {
		parseQueryParams();
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
	 * @brief Checks if the path starts with the given pattern.
	 * @param pattern The string pattern to check as a prefix of the path.
	 * @return True if the path starts with the pattern, false otherwise.
	 */
	bool match(std::string_view pattern) const {
		return path_.starts_with(pattern);
	}

	/**
	 * @brief Gets the parameters map.
	 *
	 * The map of query parameters from the path "api/objects/type/get?id=ABC&name=XYZ" would be:
	 *
	 * {"id": "ABC", "name": "XYZ"}
	 *
	 * @return A constant reference to the map containing string key-value pairs of parameters.
	 */
	const std::map<std::string, std::string>& params() const {
		return params_;
	}

	/**
	 * @brief Extracts a segment from the given pattern.
	 *
	 * Extract the part of the path after the pattern and before the next segment, excluding any query parameters.
	 *
	 * For example, for a path of "api/objects/type/get?id=ABC" and pattern of "api/objects/" the function would extract "type" as the segment, and a pattern of "api/objects/type/get" would extract "get".
	 *
	 * @param pattern The pattern string from which to extract the segment.
	 * @return A view of the extracted segment within the pattern.
	 */
	std::string_view extractNextSegment(std::string_view pattern) const;

private:
	std::string_view path_{};                     /**< The full path string to be parsed. */
	std::string_view pre_param_path_{};           /**< The part of the path before any query parameters. */
	std::map<std::string, std::string> params_{}; /**< The map of query parameters. */

	/**
	 * @brief Parses query parameters from a URL target string.
	 *
	 * Creates a map of query parameters from the target string (e.g., "id=ABC&name=XYZ") would produce:
	 *
	 * {"id": "ABC", "name": "XYZ"}
	 *
	 * @param target The URL target string containing query parameters to parse.
	 */
	void parseQueryParams();

	/**
	 * @brief Extracts the pre-parameter path from the full path.
	 *
	 * This is the part of the path before any query parameters (i.e., before the '?').
	 *
	 * For example, for a path of "api/objects/type/get?id=ABC" this function would return "api/objects/type/get".
	 *
	 * @param path The full path string from which to extract the pre-parameter path.
	 * @return A view of the pre-parameter path string.
	 */
	std::string_view extractPreParamPath(std::string_view path) const;
};

} // namespace rm::rest
