#include <algorithm> // for std::transform
#include <cctype>    // for std::tolower
#include <iomanip>   // for std::setw, std::setfill
#include <iterator>  // for std::back_inserter
#include <sstream>
#include <StringUtils.h>

namespace archid {

const std::string lcase(std::string_view sv) {
	std::string lower{};
	std::transform(sv.begin(), sv.end(), std::back_inserter(lower), [](char c) { return static_cast<char>(std::tolower(static_cast<unsigned char>(c))); });
	return lower;
}

const std::string ucase(std::string_view sv) {
	std::string upper{};
	std::transform(sv.begin(), sv.end(), std::back_inserter(upper), [](char c) { return static_cast<char>(std::toupper(static_cast<unsigned char>(c))); });
	return upper;
}

const std::string lcaseconcat(std::string_view sv) {
	std::stringstream ss{};

	// loop through each character and only adding aplphanums
	bool last_alpha{false};
	for (char c : sv) {
		if (isalnum(c)) {
			ss.put(tolower(c));
		}
	}

	return ss.str();
}

std::vector<std::string> tokenise(const std::string& text, const std::string& delimiters) {
	std::vector<std::string> tokens;

	std::size_t start_pos;
	std::size_t end_pos = 0;

	while ((start_pos = text.find_first_not_of(delimiters, end_pos)) != std::string::npos) {
		end_pos = text.find_first_of(delimiters, start_pos);
		tokens.emplace_back(text.substr(start_pos, end_pos - start_pos));
	}

	return tokens;
}

std::vector<std::string> tokeniseByString(std::string_view text, const std::string& delimiter) {
	std::vector<std::string> tokens{};
	std::size_t start_pos{0};
	std::size_t end_pos{text.find(delimiter)};

	while (end_pos != std::string::npos) {
		tokens.emplace_back(text.substr(start_pos, end_pos - start_pos));
		start_pos = end_pos + delimiter.length();
		end_pos = text.find(delimiter, start_pos);
	}
	tokens.emplace_back(text.substr(start_pos)); // Add the last token after the final delimiter
	return tokens;
}

std::string uriDecode(const std::string& encoded) {
	std::string decoded;
	decoded.reserve(encoded.size());

	for (std::size_t i = 0; i < encoded.size(); ++i) {
		if (encoded[i] == '%' && i + 2 < encoded.size()) {
			int value;
			std::istringstream is(encoded.substr(i + 1, 2));
			if (is >> std::hex >> value) {
				decoded += static_cast<char>(value);
				i += 2;
			} else {
				decoded += '%';
			}
		} else if (encoded[i] == '+') {
			decoded += ' ';
		} else {
			decoded += encoded[i];
		}
	}

	return decoded;
}

std::string uriEncode(const std::string& decoded) {
	std::ostringstream encoded;
	encoded << std::uppercase << std::hex << std::setfill('0');

	for (unsigned char c : decoded) {
		// Unreserved characters per RFC 3986: A-Z a-z 0-9 - _ . ~
		if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
			encoded << static_cast<char>(c);
		} else {
			encoded << '%' << std::setw(2) << static_cast<int>(c);
		}
	}

	return encoded.str();
}

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

} // namespace archid