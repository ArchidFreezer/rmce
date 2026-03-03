#pragma once

#include <boost/json.hpp>
#include <string>
#include <optional>
#include <vector>
#include <map>

namespace rm::rule::serial {

namespace json = boost::json;

/**
 * @class JsonConverter
 * @brief Utility class for converting between C++ types and boost::json types
 */
class JsonConverter {
public:
	/**
	 * @brief Retrieves a string value from a JSON object by key, returning a default value if the key is not found.
	 * @param obj The JSON object to search.
	 * @param key The key of the string value to retrieve.
	 * @param default_value The default string value to return if the key is not found. Defaults to an empty string.
	 * @return The string value associated with the key, or the default value if the key does not exist.
	 */
	static std::string getString(const json::object& obj, const std::string& key, const std::string& default_value = "");
	/**
	 * @brief Retrieves an optional string value from a JSON object by key.
	 * @param obj The JSON object to search.
	 * @param key The key of the string value to retrieve.
	 * @return An optional containing the string value if the key exists and the value is a string, otherwise an empty optional.
	 */
	static std::optional<std::string> getOptionalString(const json::object& obj, const std::string& key);

	/**
	 * @brief Retrieves an integer value from a JSON object by key, or returns a default value if the key is not found.
	 * @param obj The JSON object to search.
	 * @param key The key to look up in the JSON object.
	 * @param default_value The default value to return if the key is not found or the value cannot be converted to an integer. Defaults to 0.
	 * @return The integer value associated with the key, or the default value if the key is not found.
	 */
	static int getInt(const json::object& obj, const std::string& key, int default_value = 0);
	/**
	 * @brief Retrieves a double value from a JSON object by key, returning a default value if the key is not found.
	 * @param obj The JSON object to retrieve the value from.
	 * @param key The key to look up in the JSON object.
	 * @param default_value The default value to return if the key is not found or the value cannot be converted to a double. Defaults to 0.0.
	 * @return The double value associated with the key, or the default value if the key is not found.
	 */
	static double getDouble(const json::object& obj, const std::string& key, double default_value = 0.0);
	/**
	 * @brief Retrieves a boolean value from a JSON object by key, returning a default value if the key is not found.
	 * @param obj The JSON object to search.
	 * @param key The key to look up in the JSON object.
	 * @param default_value The default value to return if the key is not found or the value cannot be converted to a boolean. Defaults to false.
	 * @return The boolean value associated with the key, or the default value if the key is not found.
	 */
	static bool getBool(const json::object& obj, const std::string& key, bool default_value = false);

	/**
	 * @brief Retrieves an array of strings from a JSON object using the specified key.
	 * @param obj The JSON object to retrieve the string array from.
	 * @param key The key associated with the string array in the JSON object.
	 * @return A vector containing the strings from the JSON array.
	 */
	static std::vector<std::string> getStringArray(const json::object& obj, const std::string& key);
	/**
	 * @brief Retrieves an array of integers from a JSON object using the specified key.
	 * @param obj The JSON object to retrieve the integer array from.
	 * @param key The key associated with the integer array in the JSON object.
	 * @return A vector containing the integers from the JSON array.
	 */
	static std::vector<int> getIntArray(const json::object& obj, const std::string& key);

	/**
	 * @brief Retrieves a map of strings from a JSON object using the specified key.
	 * @param obj The JSON object to retrieve the string map from.
	 * @param key The key associated with the string map in the JSON object.
	 * @return A map containing the key-value pairs of strings from the JSON object.
	 */
	static std::map<std::string, std::string> getStringMap(const json::object& obj, const std::string& key);

	/**
	 * @brief Sets a string value in a JSON object for the specified key.
	 * @param obj The JSON object to modify.
	 * @param key The key under which to store the string value.
	 * @param value The string value to store in the JSON object.
	 */
	static void setString(json::object& obj, const std::string& key, const std::string& value);
	/**
	 * @brief Sets an integer value in a JSON object with the specified key.
	 * @param obj The JSON object to modify.
	 * @param key The key under which to store the integer value.
	 * @param value The integer value to set.
	 */
	static void setInt(json::object& obj, const std::string& key, int value);
	/**
	 * @brief Sets a double-precision floating-point value in a JSON object.
	 * @param obj The JSON object to modify.
	 * @param key The key under which to store the value.
	 * @param value The double-precision floating-point value to set.
	 */
	static void setDouble(json::object& obj, const std::string& key, double value);
	/**
	 * @brief Sets a boolean value in a JSON object for the specified key.
	 * @param obj The JSON object to modify.
	 * @param key The key under which to store the boolean value.
	 * @param value The boolean value to set.
	 */
	static void setBool(json::object& obj, const std::string& key, bool value);

	/**
	 * @brief Sets a string array value in a JSON object with the specified key.
	 * @param obj The JSON object to modify.
	 * @param key The key under which to store the string array.
	 * @param values The vector of strings to store as an array in the JSON object.
	 */
	static void setStringArray(json::object& obj, const std::string& key, const std::vector<std::string>& values);
	/**
	 * @brief Sets an integer array value in a JSON object with the specified key.
	 * @param obj The JSON object to modify.
	 * @param key The key under which to store the integer array.
	 * @param values The vector of integers to store as an array in the JSON object.
	 */
	static void setIntArray(json::object& obj, const std::string& key, const std::vector<int>& values);

	/**
	 * @brief Sets a string value in a JSON object if the optional value is present.
	 * @param obj The JSON object to modify.
	 * @param key The key under which to store the value.
	 * @param value The optional string value to set. If the optional has no value, no modification is made.
	 */
	static void setOptionalString(json::object& obj, const std::string& key, const std::optional<std::string>& value);
};

} // namespace rm::rule::serial