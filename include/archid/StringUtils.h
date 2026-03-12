#pragma once

#include <string>
#include <string_view>
#include <vector>
/**
 * @file StringUtils.h
 * 
 * Set of utility finctions for string handling
 */

namespace archid {

	/**
	 * @brief Gets a lower case std::string from a std::string_view
	 *
	 * Only 1:1 character mapping can be performed by this function
	 *
	 * @param sv std::string_view to transform
	 * @return std::string of the parameter in lowercase
	 */
	const std::string lcase(std::string_view sv);

	/**
	 * @brief Gets an upper case std::string from a std::string_view
	 *
	 * Only 1:1 character mapping can be performed by this function
	 *
	 * @param sv std::string_view to transform
	 * @return std::string of the parameter in uppercase
     */
	const std::string ucase(std::string_view sv);

	/**
	 * @brief Creates a lowercase string, removing all non alphanum characters
	 *
	 * @param sv string_view to transform
	 * @return std::string lowercase string with all non alphanum characters removed
	 */
	const std::string lcaseconcat(std::string_view sv);

	/**
	 * @brief Split a string into tokens on one or more delimiters
	 *
	 * The @a delimiters argument may consist of multiple characters each of which will be used as a delimiter, for example
	 * using the text string "Test 1,string 1,Test 2,string 2:Test 3:string 3" and delimiters ",:" would return an array of
	 * the following:
	 * + Test 1
	 * + string 1
	 * + Test 2
	 * + string 2
	 * + Test 3
	 * + string 3
	 *
	 * @param text String to split
	 * @param delimiters String containing the character(s) to use as tokens
	 * @return vector of the tokens
	 */
	std::vector<std::string> tokenise(const std::string& text, const std::string& delimiters = " ");

	/**
	 * @brief Split a string into tokens on a delimiter string
	 *
	 * The @a delimiter argument is used as a whole string to split the text, for example using the text string "Test 1,string 1,Test 2,string 2:Test 3:string 3" and delimiter ",Test " would return an array of
	 * the following:
	 * + Test 1,string 1
	 * + 2,string 2:Test 3:string 3
	 *
	 * @param text String to split
	 * @param delimiter String to use as a token
	 * @return vector of the tokens
	 */
	std::vector<std::string> tokeniseByString(std::string_view text, const std::string& delimiter);

}