#include <algorithm> // for std::transform
#include <cctype>    // for std::tolower
#include <iterator>  // for std::back_inserter
#include <sstream>
#include "StringUtils.h"

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