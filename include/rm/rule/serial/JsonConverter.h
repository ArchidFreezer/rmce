#pragma once

#include <boost/json.hpp>
#include <string>
#include <optional>
#include <vector>
#include <map>
#include <set>
#include <sstream>

namespace rm::rule::serial {

namespace json = boost::json;

/**
 * @class JsonConverter
 * @brief Utility class for converting between C++ types and boost::json types
 */
class JsonConverter {
public:
	/**
	 * @class NestedBuilder
	 * @brief A builder class for constructing nested JSON objects with a fluent interface.
	 *
	 * An example of it's use would be:
	 * @code
	 * std::pair<int, int> encounter_range = ref.encounterRange();
	 * if (encounter_range.first != 0 || encounter_range.second != 0) {
	 *      JsonConverter::createNested(obj)
	 *      .beginObject("encounter-range")
	 *          .setInt("min", encounter_range.first)
	 *          .setInt("max", encounter_range.second)
	 *      .endObject();
	 * }
	 * @endcode
	 *
	 * This would produce something like:
	 * @code
	 * {
	 *     "encounter-range": {
	 *         "min": 1,
	 *         "max": 10
	 *     }
	 * }
	 * @endcode
	 *
	 * For arrays with objects:
	 * @code
	 * JsonConverter::createNested(obj)
	 *     .beginArray("arr")
	 *         .addObject()
	 *             .setInt("val1", 12)
	 *             .setString("val2", "hello")
	 *         .endObject()
	 *         .addObject()
	 *             .setInt("val1", 34)
	 *             .setString("val2", "world")
	 *         .endObject()
	 *     .endArray();
	 * @endcode
	 *
	 * This produces:
	 * @code
	 * {
	 *     "arr": [
	 *         { "val1": 12, "val2": "hello" },
	 *         { "val1": 34, "val2": "world" }
	 *     ]
	 * }
	 * @endcode
	 *
	 * For more complex nested structures, you can mix and match the beginObject, beginArray, addObject, setInt, setString, etc. methods to build up the desired JSON structure in a fluent and readable way.
	 * @code
	 * JsonConverter::createNested(obj)
	 * .beginObject("character")
	 *    .setString("name", "Hero")
	 *    .beginArray("inventory")
	 *        .addObject()
	 *            .setString("item", "potion")
	 *            .setInt("quantity", 5)
	 *        .endObject()
	 *        .addObject()
	 *            .setString("item", "gold")
	 *            .setInt("quantity", 100)
	 *        .endObject()
	 *    .endArray()
	 *    .beginArray("skills")
	 *        .addString("fireball")
	 *        .addString("heal")
	 *    .endArray()
	 *.endObject();
	 *
	 * This produces:
	 * @code
	 * {
	 *     "character": {
	 *     "name": "Hero",
	 *     "inventory": [
	 *         { "item": "potion", "quantity": 5 },
	 *         { "item": "gold", "quantity": 100 }
	 *     ],
	 *     "skills": ["fireball", "heal"]
	 * }
	 * @endcode
	 */
	class NestedBuilder {
	public:
		/**
		 * @brief Constructs a NestedBuilder with a reference to a JSON object root.
		 * @param root A reference to the JSON object that serves as the root for nested building operations.
		 */
		explicit NestedBuilder(json::object& root) : root_(root) {
		}

		/**
		 * @brief Begins a nested object with the specified key in the builder.
		 * @param key The key name for the nested object to begin.
		 * @return A reference to this builder for method chaining.
		 */
		NestedBuilder& beginObject(const std::string& key) {
			path_.push_back({key, PathType::Object});
			return *this;
		}

		/**
		 * @brief Ends the current object in the nested structure and returns to the parent level.
		 * @return A reference to this NestedBuilder instance for method chaining.
		 */
		NestedBuilder& endObject() {
			if (!path_.empty() && (path_.back().type == PathType::Object || path_.back().type == PathType::ArrayObject)) {
				path_.pop_back();
			}
			return *this;
		}

		/**
		 * @brief Begins a new JSON array with the specified key in the current object.
		 * @param key The key name for the array to be added to the current object.
		 * @return A reference to this NestedBuilder instance for method chaining.
		 */
		NestedBuilder& beginArray(const std::string& key) {
			getCurrentObject()[key] = json::array();
			path_.push_back({key, PathType::Array});
			return *this;
		}

		/**
		 * @brief Ends the current array in the nested structure being built.
		 * @return A reference to this NestedBuilder instance for method chaining.
		 */
		NestedBuilder& endArray() {
			if (!path_.empty() && path_.back().type == PathType::Array) {
				path_.pop_back();
			}
			return *this;
		}

		/**
		 * @brief Adds a new empty JSON object to the current array and navigates into it.
		 * @return A reference to this NestedBuilder instance for method chaining.
		 */
		NestedBuilder& addObject() {
			json::array& arr = getCurrentArray();
			arr.push_back(json::object());
			path_.push_back({"", PathType::ArrayObject});
			return *this;
		}

		/**
		 * @brief Adds an integer value to the current array being built.
		 * @param value The integer value to add to the array.
		 * @return A reference to this NestedBuilder instance for method chaining.
		 */
		NestedBuilder& addInt(int value) {
			getCurrentArray().push_back(json::value(value));
			return *this;
		}

		/**
		 * @brief Adds a double-precision floating-point value to the current JSON array.
		 * @param value The double value to add to the array.
		 * @return A reference to this NestedBuilder instance for method chaining.
		 */
		NestedBuilder& addDouble(double value) {
			getCurrentArray().push_back(json::value(value));
			return *this;
		}

		/**
		 * @brief Adds a string value to the current array being built.
		 * @param value The string value to add to the array.
		 * @return A reference to this NestedBuilder instance for method chaining.
		 */
		NestedBuilder& addString(const std::string& value) {
			getCurrentArray().push_back(json::value(value));
			return *this;
		}

		/**
		 * @brief Adds a boolean value to the current array being built.
		 * @param value The boolean value to add to the array.
		 * @return A reference to this NestedBuilder instance for method chaining.
		 */
		NestedBuilder& addBool(bool value) {
			getCurrentArray().push_back(json::value(value));
			return *this;
		}

		/**
		 * @brief Adds an enum value to the current array after converting it to a string.
		 * @tparam EnumType The enumeration type of the value to be added.
		 * @param value The enum value to convert and add to the array.
		 * @return A reference to this NestedBuilder for method chaining.
		 */
		template<typename EnumType>
		NestedBuilder& addEnum(EnumType value) {
			getCurrentArray().push_back(json::value(toString(value)));
			return *this;
		}

		/**
		 * @brief Sets an integer value for the specified key in the current object.
		 * @param key The key name to associate with the integer value.
		 * @param value The integer value to set.
		 * @return A reference to this NestedBuilder instance for method chaining.
		 */
		NestedBuilder& setInt(const std::string& key, int value) {
			getCurrentObject()[key] = value;
			return *this;
		}

		/**
		 * @brief Sets a double-precision floating-point value for the specified key in the current object.
		 * @param key The key name to associate with the value.
		 * @param value The double-precision floating-point value to set.
		 * @return A reference to this NestedBuilder instance for method chaining.
		 */
		NestedBuilder& setDouble(const std::string& key, double value) {
			getCurrentObject()[key] = value;
			return *this;
		}

		/**
		 * @brief Sets a string value for the specified key in the current object.
		 * @param key The key to associate with the string value.
		 * @param value The string value to store.
		 * @return A reference to this NestedBuilder instance for method chaining.
		 */
		NestedBuilder& setString(const std::string& key, const std::string& value) {
			getCurrentObject()[key] = value;
			return *this;
		}

		/**
		 * @brief Sets a boolean value for the specified key in the current object.
		 * @param key The key name to associate with the boolean value.
		 * @param value The boolean value to set.
		 * @return A reference to this NestedBuilder instance for method chaining.
		 */
		NestedBuilder& setBool(const std::string& key, bool value) {
			getCurrentObject()[key] = value;
			return *this;
		}

		/**
		 * @brief Sets a JSON array of strings in the current object using values from a set.
		 * @param key The key under which to store the string array in the JSON object.
		 * @param values A set of strings to be converted into a JSON array.
		 * @return A reference to this NestedBuilder instance for method chaining.
		 */
		NestedBuilder& setStringArray(const std::string& key, const std::set<std::string>& values) {
			json::array arr;
			for (const auto& value : values) {
				arr.push_back(json::value(value));
			}
			getCurrentObject()[key] = arr;
			return *this;
		}

		/**
		 * @brief Sets an integer array value in the current JSON object.
		 * @param key The key under which to store the integer array.
		 * @param values A vector of integers to be stored as a JSON array.
		 * @return A reference to this NestedBuilder instance for method chaining.
		 */
		NestedBuilder& setIntArray(const std::string& key, const std::vector<int>& values) {
			json::array arr;
			for (int value : values) {
				arr.push_back(json::value(value));
			}
			getCurrentObject()[key] = arr;
			return *this;
		}

		/**
		 * @brief Sets a double array value in the current JSON object.
		 * @param key The key under which to store the double array.
		 * @param values A vector of double values to be stored as a JSON array.
		 * @return A reference to this NestedBuilder instance for method chaining.
		 */
		NestedBuilder& setDoubleArray(const std::string& key, const std::vector<double>& values) {
			json::array arr;
			for (double value : values) {
				arr.push_back(json::value(value));
			}
			getCurrentObject()[key] = arr;
			return *this;
		}

		/**
		 * @brief Sets an array of enum values converted to strings in the current JSON object.
		 * @tparam EnumType The enum type to be converted to strings.
		 * @param key The key under which to store the array in the JSON object.
		 * @param values A set of enum values to convert to strings and store as a JSON array.
		 * @return A reference to this NestedBuilder for method chaining.
		 */
		template<typename EnumType>
		NestedBuilder& setEnumArray(const std::string& key, const std::set<EnumType>& values) {
			json::array arr;
			for (const auto& value : values) {
				arr.push_back(toString(value));
			}
			getCurrentObject()[key] = arr;
			return *this;
		}

	private:
		enum class PathType {
			Object,     // Named object: { "key": { ... } }
			Array,      // Array: { "key": [ ... ] }
			ArrayObject // Object inside an array: [ { ... } ]
		};

		struct PathElement {
			std::string key;
			PathType type;
		};

		json::object& getCurrentObject() {
			json::object* current = &root_;

			for (size_t i = 0; i < path_.size(); ++i) {
				const auto& element = path_[i];

				if (element.type == PathType::Object) {
					// Navigate into a named object
					auto it = current->find(element.key);
					if (it == current->end() || !it->value().is_object()) {
						(*current)[element.key] = json::object();
					}
					current = &(*current)[element.key].as_object();
				} else if (element.type == PathType::ArrayObject) {
					// Get the object at the end of the parent array
					json::array* parent_array = getArrayAtDepth(i - 1);
					if (parent_array && !parent_array->empty() && parent_array->back().is_object()) {
						current = &parent_array->back().as_object();
					}
				}
				// For Array type, we don't navigate into it for object access
			}

			return *current;
		}

		json::array& getCurrentArray() {
			// Find the most recent array in the path
			for (auto it = path_.rbegin(); it != path_.rend(); ++it) {
				if (it->type == PathType::Array) {
					size_t depth = std::distance(it, path_.rend()) - 1;
					json::array* arr = getArrayAtDepth(depth);
					if (arr) {
						return *arr;
					}
				}
			}
			throw std::runtime_error("No active array context");
		}

		json::array* getArrayAtDepth(size_t depth) {
			json::object* current = &root_;

			for (size_t i = 0; i <= depth && i < path_.size(); ++i) {
				const auto& element = path_[i];

				if (element.type == PathType::Object) {
					auto it = current->find(element.key);
					if (it == current->end() || !it->value().is_object()) {
						return nullptr;
					}
					current = &(*current)[element.key].as_object();
				} else if (element.type == PathType::Array) {
					auto it = current->find(element.key);
					if (it != current->end() && it->value().is_array()) {
						return &it->value().as_array();
					}
					return nullptr;
				}
			}
			return nullptr;
		}

		json::object& root_;
		std::vector<PathElement> path_;
	};

	/**
	 * @brief Creates a nested builder for constructing nested JSON objects.
	 * @param obj A reference to the JSON object where nested content will be built.
	 * @return A NestedBuilder instance initialized with the provided JSON object.
	 */
	static NestedBuilder createNested(json::object& obj) {
		return NestedBuilder(obj);
	}

	/**
	 * @brief Retrieves a nested JSON object from within another JSON object using a path string.
	 *
	 * The path string should use '/' as a delimiter to specify the hierarchy of nested objects. For example, if you have a JSON object like:
	 * @code
	 * {
	 *     "level1": {
	 *         "level2": {
	 *             "SomeKey": "SomeValue"
	 *         }
	 *     }
	 * }
	 * @endcode
	 * You can retrieve the nested object at "level1/level2" using this method:
	 * @code
	 * json::object obj = ...; // Assume this is your JSON object
	 * std::string path = "level1/level2";
	 * auto* nestedObj = JsonConverter::getNestedObject(obj, path);
	 * // Do something with nestedObj, which would point to the object { "SomeKey": "SomeValue }
	 * @endcode
	 *
	 * @param obj The root JSON object to search within.
	 * @param path The path string specifying the location of the nested object.
	 * @return A pointer to the nested JSON object if found, or null if the path is invalid or the object does not exist.
	 */
	static json::object* getNestedObject(json::object& obj, const std::string& path);

	/**
	 * @brief Retrieves a nested value from a JSON object using a path string.
	 *
	 * The path string should use '/' as a delimiter to specify the hierarchy of nested objects. For example, if you have a JSON object like:
	 * @code
	 * {
	 *     "level1": {
	 *         "level2": {
	 *             "SomeKey": "SomeValue"
	 *         }
	 *     }
	 * }
	 * @endcode
	 * You can retrieve the nested value at "level1/level2/SomeKey" using this method:
	 * @code
	 * json::object obj = ...; // Assume this is your JSON object
	 * std::string path = "level1/level2/SomeKey";
	 * auto* nestedValue = JsonConverter::getNestedValue(obj, path);
	 * // Do something with nestedValue, which would point to the value "SomeValue"
	 * @endcode
	 *
	 * @param obj The JSON object to search within.
	 * @param path The path string specifying the location of the nested value.
	 * @return A pointer to the JSON value at the specified path, or nullptr if not found.
	 */
	static json::value* getNestedValue(json::object& obj, const std::string& path);

	/**
	 * @brief Retrieves a nested integer value from a JSON object using a path string.
	 *
	 * The path string should use '/' as a delimiter to specify the hierarchy of nested objects. For example, if you have a JSON object like:
	 * @code
	 * {
	 *     "level1": {
	 *         "level2": {
	 *             "SomeInt": 42
	 *         }
	 *     }
	 * }
	 * @endcode
	 * You can retrieve the nested integer value at "level1/level2/SomeInt" using this method:
	 * @code
	 * json::object obj = ...; // Assume this is your JSON object
	 * std::string path = "level1/level2/SomeInt";
	 * int value = JsonConverter::getNestedInt(obj, path, 0);
	 * // value would be 42, or 0 if the path is not found or the value is not an integer
	 * @endcode
	 *
	 * @param obj The JSON object to search within.
	 * @param path The path string specifying the location of the nested integer value.
	 * @param defaultValue The default value to return if the path is not found or the value is not an integer. Defaults to 0.
	 * @return The integer value at the specified path, or the default value if not found.
	 */
	static int getNestedInt(json::object& obj, const std::string& path, int defaultValue = 0);

	/**
	 * @brief Retrieves a double value from a nested JSON object using a path string.
	 *
	 * The path string should use '/' as a delimiter to specify the hierarchy of nested objects. For example, if you have a JSON object like:
	 * @code
	 * {
	 *     "level1": {
	 *         "level2": {
	 *             "SomeDouble": 3.14
	 *         }
	 *     }
	 * }
	 * @endcode
	 * You can retrieve the nested integer value at "level1/level2/SomeInt" using this method:
	 * @code
	 * json::object obj = ...; // Assume this is your JSON object
	 * std::string path = "level1/level2/SomeInt";
	 * double value = JsonConverter::getNestedDouble(obj, path, 0.0);
	 * // value would be 3.14, or 0.0 if the path is not found or the value is not a double
	 * @endcode
	 *
	 * @param obj The JSON object to search within.
	 * @param path The path string specifying the location of the nested double value.
	 * @param defaultValue The default value to return if the path is not found or the value is not a double. Defaults to 0.0.
	 * @return The double value at the specified path, or the default value if not found.
	 */
	static double getNestedDouble(json::object& obj, const std::string& path, double defaultValue = 0.0);

	/**
	 * @brief Retrieves a nested string value from a JSON object using a path notation.
	 *
	 * The path should use '/' as a delimiter to specify the hierarchy of nested objects. For example, if you have a JSON object like:
	 * @code
	 * {
	 *     "level1": {
	 *         "level2": {
	 *             "SomeKey": "SomeValue"
	 *         }
	 *     }
	 * }
	 * @endcode
	 * You can retrieve the nested value at "level1/level2/SomeKey" using this method:
	 * @code
	 * json::object obj = ...; // Assume this is your JSON object
	 * std::string path = "level1/level2/SomeKey";
	 * std::string value = JsonConverter::getNestedString(obj, path, "default");
	 * @endcode
	 *
	 * @param obj The JSON object to search within.
	 * @param path The path to the nested string value (e.g., "parent.child.key").
	 * @param defaultValue The default value to return if the path is not found or the value is not a string. Defaults to an empty string.
	 * @return The string value at the specified path, or the default value if not found.
	 */
	static std::string getNestedString(json::object& obj, const std::string& path, const std::string& defaultValue = "");

	/**
	 * @brief Retrieves a nested array of strings from a JSON object using a path specification.
	 *
	 * The path should use '/' as a delimiter to specify the hierarchy of nested objects. For example, if you have a JSON object like:
	 * @code
	 * {
	 *     "level1": {
	 *         "level2": [
	 *             "SomeValue1",
	 *             "SomeValue2",
	 *             "SomeValue3"
	 *         ]
	 *     }
	 * }
	 * @endcode
	 * You can retrieve the nested string array at "level1/level2" using this method:
	 * @code
	 * json::object obj = ...; // Assume this is your JSON object
	 * std::string path = "level1/level2";
	 * std::set<std::string> values = JsonConverter::getNestedStringArray(obj, path);
	 * // Do something with the set of strings, which would contain "SomeValue1", "SomeValue2", and "SomeValue3"
	 * @endcode
	 * 
	 * @param obj The JSON object to traverse.
	 * @param path The path string specifying the location of the nested string array within the object.
	 * @return A set containing the strings from the nested array.
	 */
	static std::set<std::string> getNestedStringArray(json::object& obj, const std::string& path);

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
	 * @brief Retrieves a JSON array from a JSON object using the specified key.
	 * 
	 * Given a JSON object and a key, this method attempts to retrieve the value associated with the key and checks if it is a JSON array. If the key exists and the value is an array, it returns that array. If the key does not exist or the
	 * value is not an array, it returns an empty JSON array.
	 * 
	 * For the following JSON object:
	 * @code
	 * {
	 *     "myArray": [
	 *         { "name": "Alice", "age": 30 },
	 *         { "name": "Bob", "age": 25 },
	 *         { "name": "Charlie", "age": 35 }
	 *     ]
	 * }
	 * @endcode
	 * You can retrieve the array associated with "myArray" using this method:
	 * @code
	 * json::object obj = ...; // Assume this is your JSON object
	 * json::array arr = JsonConverter::getJsonArray(obj, "myArray");
	 * for (const auto& item : arr) {
	 *     std::string name = JsonConverter::getString(item.as_object(), "name");
	 *     int age = JsonConverter::getInt(item.as_object(), "age");
	 * }
	 * @endcode
	 * 
	 * @param obj The JSON object to retrieve the array from.
	 * @param key The key associated with the array in the JSON object.
	 * @return A JSON array if the key exists and is an array, otherwise an empty JSON array.
	 */
	static json::array getJsonArray(const json::object& obj, const std::string& key);

	/**
	 * @brief Retrieves an array of integers from a JSON object using the specified key.
	 * @param obj The JSON object to retrieve the integer array from.
	 * @param key The key associated with the integer array in the JSON object.
	 * @return A vector containing the integers from the JSON array.
	 */
	static std::vector<int> getIntArray(const json::object& obj, const std::string& key);

	/**
	 * @brief Retrieves an array of enum values from a JSON object using the specified key.
	 * @tparam EnumType The type of the enum values.
	 * @param obj The JSON object to retrieve the enum array from.
	 * @param key The key associated with the enum array in the JSON object.
	 * @return A set containing the enum values retrieved from the JSON array. Each string in the JSON array will be converted to its corresponding enum value using a hypothetical `fromString` function.
	 */
	template<typename EnumType>
	static std::set<EnumType> getEnumArray(const json::object& obj, const std::string& key);

	/**
	 * @brief Retrieves a map of strings from a JSON object using the specified key.
	 * 
	 * For a JSON object like:
	 * @code
	 * {
	 *     "myMap": { "key1": "value1", "key2": "value2", "key3": "value3" }
	 * }
	 * @endcode
	 * You can retrieve the map of strings associated with "myMap" using this method:
	 * @code
	 * json::object obj = ...; // Assume this is your JSON object
	 * std::map<std::string, std::string> myMap = JsonConverter::getStringMap(obj, "myMap");
	 * // myMap would contain { "key1": "value1", "key2": "value2", "key3": "value3" }
	 * @endcode
	 * 
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

	/**
	 * @brief Sets an array of enum values in a JSON object with the specified key.
	 * @tparam EnumType The type of the enum values.
	 * @param obj The JSON object to modify.
	 * @param key The key under which to store the enum array.
	 * @param enumValues The set of enum values to store as an array in the JSON object. Each enum value will be converted to its string representation using a hypothetical `toString` function.
	 */
	template<typename EnumType>
	static void setEnumArray(json::object& obj, const std::string& key, const std::set<EnumType>& enumValues);
};

template<typename EnumType>
void JsonConverter::setEnumArray(json::object& obj, const std::string& key, const std::set<EnumType>& enumValues) {
	json::array arr;
	for (const auto& enumValue : enumValues) {
		arr.push_back(json::value(toString(enumValue)));
	}
	obj[key] = arr;
}

template<typename EnumType>
std::set<EnumType> JsonConverter::getEnumArray(const json::object& obj, const std::string& key) {
	std::set<EnumType> result;
	auto it = obj.find(key);
	if (it != obj.end() && it->value().is_array()) {
		for (const auto& item : it->value().as_array()) {
			if (item.is_string()) {
				EnumType enum_value{};
				fromString(item.as_string(), enum_value);
				result.emplace(enum_value);
			}
		}
	}
	return result;
}

} // namespace rm::rule::serial