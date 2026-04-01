#include <HttpPathParser.h>

namespace rm::rest {

// Create a map of query parameters from the target string (e.g., "id=ABC&name=XYZ")
void HttpPathParser::parseQueryParams() {
	std::map<std::string, std::string> params;

	// Find the query string start
	auto query_pos = path_.find('?');
	if (query_pos == std::string_view::npos) {
		params_ = params;
		return;
	}

	// Extract query string
	std::string_view query = path_.substr(query_pos + 1);

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
std::string_view HttpPathParser::extractNextSegment(std::string_view pattern) const {
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

std::string_view HttpPathParser::extractPreParamPath(std::string_view path) const {
	size_t queryPos = path.find('?');
	if (queryPos != std::string_view::npos) {
		return path.substr(0, queryPos);
	} else {
		return path;
	}
}

} // namespace rm::rest