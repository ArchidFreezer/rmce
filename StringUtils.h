#pragma once

#include <algorithm> // for std::transform
#include <cctype>    // for std::tolower
#include <iterator>  // for std::back_inserter
#include <string>
#include <string_view>

// This function returns a std::string that is the lower-case version of the std::string_view passed in.
// Only 1:1 character mapping can be performed by this function
std::string toASCIILowerCase(std::string_view sv)
{
  std::string lower{};
  std::transform(sv.begin(), sv.end(), std::back_inserter(lower),
    [](char c)
    {
      return static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    });
  return lower;
}