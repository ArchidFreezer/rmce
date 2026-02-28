#include <algorithm> // for std::transform
#include <cctype>    // for std::tolower
#include <iterator>  // for std::back_inserter
#include <sstream>
#include <StringUtils.h>

namespace archid {

  const std::string lcase(std::string_view sv) {
    std::string lower{};
    std::transform(sv.begin(), sv.end(), std::back_inserter(lower),
      [](char c) {
        return static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
      });
    return lower;
  }

  const std::string lcaseconcat(std::string_view sv) {
    std::stringstream ss{};

    // loop through each character and only adding aplphanums
    bool last_alpha{ false };
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
    std::size_t start_pos{ 0 };
    std::size_t end_pos{ text.find(delimiter) };

    while (end_pos != std::string::npos) {
      tokens.emplace_back(text.substr(start_pos, end_pos - start_pos));
      start_pos = end_pos + delimiter.length();
      end_pos = text.find(delimiter, start_pos);
    }
    tokens.emplace_back(text.substr(start_pos)); // Add the last token after the final delimiter
    return tokens;
  }
}