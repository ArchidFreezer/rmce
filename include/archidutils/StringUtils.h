#pragma once

#include <string>
#include <string_view>
/**
 * @file StringUtils.h
 * 
 * Set of utility finctions for string handling
 */

/**
 * @brief Gets a lower case std::string from a std::string_view
 * 
 * Only 1:1 character mapping can be performed by this function
 * 
 * @param sv std::string_view to transform
 * @return std::string of the parameter in lowercase
 */
const std::string lcase(std::string_view sv);