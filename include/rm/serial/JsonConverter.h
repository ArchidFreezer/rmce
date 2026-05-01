#pragma once

#include <boost/json.hpp>
#include <string>
#include <optional>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <GameRuleDataChoice.h>
#include <LanguageRanks.h>
#include <SubcategoriedSkillData.h>
#include <PersistentObjectManager.h>

namespace rm::serial {

using namespace rm::rule;

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
		 * @brief Sets an array of enum values from any iterable container in the current JSON object.
		 * This function accepts any iterable container (e.g., std::vector, std::set, std::list) containing enum values and converts each enum value to its string representation using the `toString` function before storing them as a JSON
		 * array in the specified key of the JSON object.
		 * @code
		 * // With std::vector
		 * std::vector<MyEnum> enumVec = {MyEnum::Value1, MyEnum::Value2, MyEnum::Value3};
		 * NestedBuilder::setEnumSet(obj, "enums", enumVec);
		 *
		 * // With std::list
		 * std::list<MyEnum> enumList = {MyEnum::Value1, MyEnum::Value2};
		 * NestedBuilder::setEnumSet(obj, "enums", enumList);
		 * @endcode
		 * @tparam Container The type of the iterable container.
		 * @param key The key under which to store the array in the JSON object.
		 * @param values The container of enum values to convert to strings and store as a JSON array.
		 * @return A reference to this NestedBuilder for method chaining.
		 */
		template<typename Container>
		NestedBuilder& setEnumArray(const std::string& key, const Container& values) {
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
	 * @brief Retrieves a nested JSON object from within another JSON object using a path string.*
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
	 * @brief Retrieves a float value from a nested JSON object using a path string.
	 *
	 * The path string should use '/' as a delimiter to specify the hierarchy of nested objects. For example, if you have a JSON object like:
	 * @code
	 * {
	 *     "level1": {
	 *         "level2": {
	 *             "SomeFloat": 3.14
	 *         }
	 *     }
	 * }
	 * @endcode
	 * You can retrieve the nested float value at "level1/level2/SomeFloat" using this method:
	 * @code
	 * json::object obj = ...; // Assume this is your JSON object
	 * std::string path = "level1/level2/SomeFloat";
	 * float value = JsonConverter::getNestedFloat(obj, path, 0.0);
	 * // value would be 3.14, or 0.0 if the path is not found or the value is not a float
	 * @endcode
	 *
	 * @param obj The JSON object to search within.
	 * @param path The path string specifying the location of the nested float value.
	 * @param defaultValue The default value to return if the path is not found or the value is not a float. Defaults to 0.0.
	 * @return The float value at the specified path, or the default value if not found.
	 */
	static float getNestedFloat(json::object& obj, const std::string& path, float defaultValue = 0.0);

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
	 * You can retrieve the nested double value at "level1/level2/SomeDouble" using this method:
	 * @code
	 * json::object obj = ...; // Assume this is your JSON object
	 * std::string path = "level1/level2/SomeDouble";
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
	 * @brief Retrieves a float value from a JSON object by key, returning a default value if the key is not found.
	 * @param obj The JSON object to search.
	 * @param key The key to look up in the JSON object.
	 * @param default_value The default value to return if the key is not found or the value cannot be converted to a float. Defaults to 0.0.
	 * @return The float value associated with the key, or the default value if the key is not found.
	 */
	static float getFloat(const json::object& obj, const std::string& key, float default_value = 0.0);
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
	static std::vector<std::string> getStringVector(const json::object& obj, const std::string& key);

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
	static std::set<EnumType> getEnumSet(const json::object& obj, const std::string& key);

	/**
	 * @brief Retrieves an array of enum values from a JSON object using the specified key.
	 * @tparam EnumType The type of the enum values.
	 * @param obj The JSON object to retrieve the enum array from.
	 * @param key The key associated with the enum array in the JSON object.
	 * @return A vector containing the enum values retrieved from the JSON array. Each string in the JSON array will be converted to its corresponding enum value using a hypothetical `fromString` function.
	 */
	template<typename EnumType>
	static std::vector<EnumType> getEnumVector(const json::object& obj, const std::string& key);

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
	 * @brief Sets a single-precision floating-point value in a JSON object for the specified key.
	 * @param obj The JSON object to modify.
	 * @param key The key under which to store the float value.
	 * @param value The single-precision floating-point value to set.
	 */
	static void setFloat(json::object& obj, const std::string& key, float value);
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
	 * @tparam Container The type of the iterable container (e.g., std::vector, std::set, std::list).
	 * @param obj The JSON object to modify.
	 * @param key The key under which to store the string array.
	 * @param values The vector of strings to store as an array in the JSON object.
	 */
	template<typename Container>
	static void setStringSet(json::object& obj, const std::string& key, const Container& values);
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
	 * @brief Sets an array of enum values from any iterable container in a JSON object.
	 *
	 * This function is for use with C-style arrays or any container that provides iterators. It takes a range defined by two iterators (begin and end) and converts each enum value in that range to its string representation using the
	 * `toString` function before storing them as a JSON array
	 *
	 * For example, if you have a C-style array of enum values:
	 * @code
	 * MyEnum enumArray[] = {MyEnum::Value1, MyEnum::Value
	 * JsonConverter::setEnumArrayFromIterators(obj, "enums", std::begin(enumArray), std::end(enumArray));
	 * @endcode
	 *
	 * For C++ containers the setEnumSet function is more convenient, but this function allows for more flexibility with different types of containers that may not be directly supported by setEnumSet.
	 *
	 * @tparam Iterator The iterator type for the container.
	 * @param obj The JSON object to modify.
	 * @param key The key under which to store the enum array.
	 * @param begin Iterator to the beginning of the container.
	 * @param end Iterator to the end of the container.
	 * @see setEnumSet for a more convenient function that accepts any iterable container directly.
	 */
	template<typename Iterator>
	static void setEnumArrayFromIterators(json::object& obj, const std::string& key, Iterator begin, Iterator end);

	/**
	 * @brief Sets an array of enum values in a JSON object with the specified key, accepting any iterable container.
	 *
	 * This function accepts any iterable container (e.g., std::vector, std::set, std::list) containing enum values and converts each enum value to its string representation using the `toString` function before storing them as a JSON array
	 * in the specified key of the JSON object.
	 * @code
	 * // With std::vector
	 * std::vector<MyEnum> enumVec = {MyEnum::Value1, MyEnum::Value2, MyEnum::Value3};
	 * JsonConverter::setEnumSet(obj, "enums", enumVec);
	 *
	 * // With std::list
	 * std::list<MyEnum> enumList = {MyEnum::Value1, MyEnum::Value2};
	 * JsonConverter::setEnumSet(obj, "enums", enumList);
	 * @endcode
	 * @tparam Container The type of the iterable container (e.g., std::vector, std::set, std::list).
	 * @param obj The JSON object to modify.
	 * @param key The key under which to store the enum array.
	 * @param enumValues The container of enum values to store as an array in the JSON object. Each enum value will be converted to its string representation using the `toString` function.
	 */
	template<typename Container>
	static void setEnumSet(json::object& obj, const std::string& key, const Container& enumValues);

	/**
	 * @brief Retrieves a set of SubcategoriedSkillData from a JSON object using the specified key.
	 *
	 * This function retrieves a JSON array associated with the given key and converts it into a set of SubcategoriedSkillData objects. Each element in the JSON array is expected to be an object with an "id" field and an optional
	 * "subcategory" field. The "id" field is used to look up the corresponding SkillData object using the provided PersistentObjectManager, and the "subcategory" field is used to create a SubcategoriedSkillData object that combines the
	 * skill data with its subcategory.
	 *
	 * For example, if you have a JSON object like:
	 * @code
	 * {
	 *     "skills": [
	 *         { "id": "SKILL_SWORDSMANSHIP", "subcategory": "SUBCATEGORY_MELEE" },
	 *         { "id": "SKILL_ARCHERY" }
	 *     ]
	 * }
	 * @endcode
	 * You can retrieve the set of SubcategoriedSkillData using this method:
	 * @code
	 * json::object obj = ...; // Assume this is your JSON object
	 * std::set<SubcategoriedSkillData> skillSet = JsonConverter::getSkillSet(obj, "skills", manager);
	 * // skillSet would contain SubcategoriedSkillData objects for SKILL_SWORDSMANSHIP with SUBCATEGORY_MELEE and SKILL_ARCHERY with no subcategory
	 * @endcode
	 *
	 * @param obj The JSON object to retrieve the skill set from.
	 * @param key The key associated with the skill array in the JSON object.
	 * @param manager A PersistentObjectManager used to look up SkillData objects based on their IDs when constructing SubcategoriedSkillData objects.
	 * @return A set of pointers to SubcategoriedSkillData objects constructed from the entries in the JSON array associated with the specified key.
	 */
	static std::set<const SubcategoriedSkillData*> getSkillSet(const json::object& obj, const std::string& key, rm::PersistentObjectManager& manager);

	/**
	 * @brief Sets an array of skill values in a JSON object with the specified key, accepting any iterable container.
	 *
	 * This function accepts any iterable container (e.g., std::vector, std::set, std::list) containing pointers to SubcategoriedSkillData objects. It converts each skill data object into a JSON object with "id" and optional "subcategory"
	 * fields, and stores them as a JSON array in the specified key of the JSON object. The skills are sorted by their combined id and subcategory for consistent ordering in the JSON output.
	 * @code
	 * // With std::vector
	 * std::vector<const rm::rule::SubcategoriedSkillData*> skillVec = {skillData1, skillData2, skillData3};
	 * JsonConverter::setSkillSet(obj, "skills", skillVec);
	 *
	 * // With std::list
	 * std::list<const rm::rule::SubcategoriedSkillData*> skillList = {skillData1, skillData2};
	 * JsonConverter::setSkillSet(obj, "skills", skillList);
	 * @endcode
	 * @tparam Container The type of the iterable container (e.g., std::vector, std::set, std::list).
	 * @param obj The JSON object to modify.
	 * @param key The key under which to store the skill array.
	 * @param skillValues The container of pointers to SubcategoriedSkillData objects to store as an array in the JSON object. Each skill will be converted to a JSON object with "id" and optional "subcategory" fields.
	 */
	template<typename Container>
	static void setSkillSet(json::object& obj, const std::string& key, const Container& skillValues);

	/**
	 * @brief Retrieves a map of language abilities from a JSON object using the specified key.
	 *
	 * This function retrieves a JSON array object associated with the given key and converts it into a map where the keys are strings and the values are LanguageRanks objects. It assumes that the JSON object has a structure where each
	 *key is a string and each value can be converted to a LanguageRanks using a hypothetical `fromJson` function.
	 *
	 * For example, if you have a JSON object like:
	 * @code
	 * {
	 *     "languageAbilities": [
	 *         {
	 *             "language": "LANGUAGE_COMMON_SPEECH",
	 *             "somantic": 4
	 *             "spoken": 8,
	 *             "written": 8
	 *         },
	 *         {
	 *             "language": "LANGUAGE_HIGH_SPEECH",
	 *             "spoken": 6,
	 *             "written": 6
	 *         }
	 *		]
	 * }
	 * @endcode
	 * You can retrieve the map of language abilities using this method:
	 * @code
	 * json::object obj = ...; // Assume this is your JSON object
	 * std::map<std::string, const rm::game::character::LanguageRanks> languageMap = JsonConverter::getLanguageAbilityMap(obj, "languageAbilities");
	 * // languageMap would contain entries for "LANGUAGE_COMMON_SPEECH" and "LANGUAGE_HIGH_SPEECH" with their corresponding LanguageRanks objects
	 * @endcode
	 *
	 * @param obj The JSON object to retrieve the language ability map from.
	 * @param key The key associated with the language ability map in the JSON object.
	 * @param manager A PersistentObjectManager used to look up language data when constructing LanguageRanks objects.
	 * @return A map where each key is a string and each value is a LanguageRanks object, constructed from the corresponding entries in the JSON object.
	 */
	static std::map<std::string, const rm::game::character::LanguageRanks> getLanguageAbilityMap(const json::object& obj, const std::string& key, rm::PersistentObjectManager& manager);

	/**
	 * @brief Converts a map of language abilities into a JSON array.
	 *
	 * This function takes a map where the keys are strings and the values are LanguageRanks objects, and converts it into a JSON array. Each entry in the map is converted into a JSON object with fields corresponding to the properties of
	 * the LanguageRanks, such as "language", "somantic", "spoken", and "written". The resulting JSON array can be used to store or transmit the language abilities in a structured format.
	 *
	 * For example, if you have a map like:
	 * @code
	 * std::map<std::string, const rm::game::character::LanguageRanks> languageMap = {
	 *     {"LANGUAGE_COMMON_SPEECH", LanguageRanks{...}},
	 *     {"LANGUAGE_HIGH_SPEECH", LanguageRanks{...}}
	 * };
	 * @endcode
	 * You can convert this map into a JSON array using this method:
	 * @code
	 * JsonConverter::getLanguageAbilityArray(jsonObj, "languages", language_map);
	 * // languageArray would contain JSON objects representing each language ability in the map
	 * @endcode
	 *
	 * @param obj The JSON object to modify.
	 * @param key The key under which to store the language ability array in the JSON object
	 * @param language_map A map where each key is a string and each value is a LanguageRanks object to be converted into a JSON object.
	 */
	static void setLanguageAbilities(json::object& obj, const std::string& key, const std::map<std::string, const rm::game::character::LanguageRanks>& language_map);

	/**
	 * @brief Retrieves a set of data objects of type GameRuleData from a JSON object using the specified key.
	 *
	 * This function retrieves a JSON array associated with the given key and converts it into a set of pointers to data objects of type T. Each element in the JSON array is expected to be a string that represents the ID of a data object.
	 * The function uses the provided PersistentObjectManager to look up each data object by its ID and adds it to the resulting set if it exists.
	 *
	 * For example, if you have a JSON object like:
	 * @code
	 * {
	 *     "items": ["ITEM_SWORD", "ITEM_SHIELD", "ITEM_POTION"]
	 * }
	 * @endcode
	 * You can retrieve the set of item data objects using this method:
	 * @code
	 * json::object obj = ...; // Assume this is your JSON object
	 * std::set<const ItemData*> itemSet = JsonConverter::getDataSet<ItemData>(obj, "items", manager);
	 * // itemSet would contain pointers to ItemData objects for ITEM_SWORD, ITEM_SHIELD, and ITEM_POTION if they exist in the manager
	 * @endcode
	 *
	 * @tparam GameRuleData The type of the data objects to retrieve. This type must satisfy the game_rule_data_object concept.
	 * @param obj The JSON object to retrieve the data object set from.
	 * @param key The key associated with the data object array in the JSON object.
	 * @param manager A PersistentObjectManager used to look up data objects based on their IDs when constructing the set.
	 * @return A set of pointers to data objects of type T constructed from the entries in the JSON array associated with the specified key.
	 */
	template<game_rule_data_object GameRuleData>
	static std::set<const GameRuleData*> getDataSet(const json::object& obj, const std::string& key, rm::PersistentObjectManager& manager);

	/**
	 * @brief Sets an array of data objects of type GameRuleData in a JSON object with the specified key.
	 *
	 * This function takes a set of pointers to data objects of type T and converts it into a JSON array. Each data object is represented as a string in the JSON array, which corresponds to the ID of the data object. The function uses the
	 * `id()` method of each data object to retrieve its ID and stores it as a string in the JSON array under the specified key in the JSON object.
	 *
	 * For example, if you have a set of item data objects:
	 * @code
	 * std::set<const ItemData*> itemSet = {itemData1, itemData2, itemData3};
	 * @endcode
	 * You can convert this set into a JSON array using this method:
	 * @code
	 * JsonConverter::setDataObjectSet(obj, "items", itemSet);
	 * // The JSON object would now contain an array under "items" with the IDs of the item data objects as strings
	 * @endcode
	 *
	 * @tparam GameRuleData The type of the data objects to set. This type must satisfy the game_rule_data_object concept.
	 * @param obj The JSON object to modify.
	 * @param key The key under which to store the data object array in the JSON object.
	 * @param data_objects A set of pointers to data objects of type T to be converted into strings and stored as an array in the JSON object.
	 */
	template<game_rule_data_object GameRuleData>
	static void setDataSet(json::object& obj, const std::string& key, const std::set<const GameRuleData*>& data_objects);

	/**
	 * @brief Retrieves a map of data objects of type GameRuleData to primitive values from a JSON object using the specified key.
	 *
	 * This function retrieves a JSON array associated with the given key and converts it into a map where the keys are pointers to data objects of type GameRuleData and the values are of a primitive type specified by the template parameter
	 * Primitive. Each element in the JSON array is expected to be an object with an "id" field that represents the ID of a data object, and a "value" field that represents the primitive value associated with that data object. The function
	 * uses the provided PersistentObjectManager to look up each data object by its ID and constructs the resulting map accordingly.
	 *
	 * For example, if you have a JSON object like:
	 * @code
	 * {
	 *     "itemQuantities": [
	 *         { "id": "ITEM_SWORD", "value": 10 },
	 *         { "id": "ITEM_SHIELD", "value": 5 }
	 *     ]
	 * }
	 * @endcode
	 * You can retrieve the map of item data objects to their quantities using this method:
	 * @code
	 * json::object obj = ...; // Assume this is your JSON object
	 * std::map<const ItemData*, int> itemQuantityMap = JsonConverter::getDataPrimitiveMap<ItemData, int>(obj, "itemQuantities", manager);
	 * // itemQuantityMap would contain entries mapping ITEM_SWORD to 10 and ITEM_SHIELD to 5 if those items exist in the manager
	 * @endcode
	 *
	 * @tparam GameRuleData The type of the data objects used as keys in the resulting map. This type must satisfy the game_rule_data_object concept.
	 * @tparam Primitive The type of the primitive values used as values in the resulting map (e.g., int, float, double, bool).
	 * @param obj The JSON object to retrieve the data object primitive map from.
	 * @param key The key associated with the data object primitive array in the JSON object.
	 * @param manager A PersistentObjectManager used to look up data objects based on their IDs when constructing the map.
	 * @return A map where each key is a pointer to a data object of type GameRuleData and each value is a primitive value
	 */
	template<game_rule_data_object GameRuleData, typename Primitive>
	static std::map<const GameRuleData*, Primitive> getDataPrimitiveMap(const json::object& obj, const std::string& key, rm::PersistentObjectManager& manager);

	/**
	 * @brief Sets an array of data objects of type GameRuleData mapped to primitive values in a JSON object with the specified key.
	 *
	 * This function takes a map where the keys are pointers to data objects of type GameRuleData and the values are of a primitive type specified by the template parameter Primitive. It converts this map into a JSON array, where each entry
	 * is represented as an object with an "id" field corresponding to the ID of the data object and a "value" field corresponding to the primitive value. The resulting JSON array is stored under the specified key in the JSON object.
	 *
	 * For example, if you have a map of item data objects to their quantities:
	 * @code
	 * std::map<const ItemData*, int> itemQuantityMap = {
	 *     {itemData1, 10},
	 *     {itemData2, 5}
	 * };
	 * @endcode
	 * You can convert this map into a JSON array using this method:
	 * @code
	 * JsonConverter::setDataPrimitiveMap<ItemData, int>(obj, "itemQuantities", itemQuantityMap);
	 * // The JSON object would now contain an array under "itemQuantities" with objects representing each item and its quantity
	 * @endcode
	 *
	 * @tparam GameRuleData The type of the data objects used as keys in the input map. This type must satisfy the game_rule_data_object concept.
	 * @tparam Primitive The type of the primitive values used as values in the input map (e.g., int, float, double, bool).
	 * @param obj The JSON object to modify.
	 * @param key The key under which to store the data object primitive array in the JSON object.
	 * @param map A map where each key is a pointer to a data object of type GameRuleData and each value is a primitive value to be converted into an object in the JSON array.
	 */
	template<game_rule_data_object GameRuleData, typename Primitive>
	static void setDataPrimitiveMap(json::object& obj, const std::string& key, const std::map<const GameRuleData*, Primitive>& map);

	/**
	 * @brief Retrieves a map of SubcategoriedSkillData pointers to primitive values from a JSON object using the specified key.
	 *
	 * This function retrieves a JSON array associated with the given key and converts it into a map where the keys are pointers to SubcategoriedSkillData objects and the values are of a primitive type specified by the template parameter
	 * Primitive. Each element in the JSON array is expected to be an object with an "id" field that represents the ID of a skill, an optional "subcategory" field that represents the subcategory of the skill, and a "value" field that
	 * represents the primitive value associated with that skill. The function uses the provided PersistentObjectManager to look up each skill data object by its ID and subcategory when constructing the resulting map.
	 *
	 * For example, if you have a JSON object like:
	 * @code
	 * {
	 *     "skillLevels": [
	 *         { "id": "SKILL_SWORDSMANSHIP", "subcategory": "SUBCATEGORY_MELEE", "value": 5 },
	 *         { "id": "SKILL_ARCHERY", "value": 3 }
	 *     ]
	 * }
	 * @endcode
	 * You can retrieve the map of skills to their levels using this method:
	 * @code
	 * json::object obj = ...; // Assume this is your JSON object
	 * std::map<const SubcategoriedSkillData*, int> skillLevelMap = JsonConverter::getSkillPrimitiveMap<int>(obj, "skillLevels", manager);
	 * // skillLevelMap would contain entries mapping SKILL_SWORDSMANSHIP with SUBCATEGORY_MELEE to 5 and SKILL_ARCHERY with no subcategory to 3 if those skills exist in the manager
	 * @endcode
	 *
	 * @tparam Primitive The type of the primitive values used as values in the resulting map (e.g., int, float, double, bool).
	 * @param obj The JSON object to retrieve the skill primitive map from.
	 * @param key The key associated with the skill primitive array in the JSON object.
	 * @param manager A PersistentObjectManager used to look up skill data objects based on their IDs and subcategories when constructing the map.
	 * @return A map where each key is a pointer to a SubcategoriedSkillData object and each value is a primitive value associated with that skill, constructed from the corresponding entries in the JSON object associated with the specified
	 * key.
	 */
	template<typename Primitive>
	static std::map<const SubcategoriedSkillData*, Primitive> getSkillPrimitiveMap(const json::object& obj, const std::string& key, rm::PersistentObjectManager& manager);

	/**
	 * @brief Sets an array of SubcategoriedSkillData pointers mapped to primitive values in a JSON object with the specified key.
	 *
	 * This function takes a map where the keys are pointers to SubcategoriedSkillData objects and the values are of a primitive type specified by the template parameter Primitive. It converts this map into a JSON array, where each entry
	 * is represented as an object with an "id" field corresponding to the ID of the skill, an optional "subcategory" field corresponding to the subcategory of the skill, and a "value" field corresponding to the primitive value. The
	 * resulting JSON array is stored under the specified key in the JSON object.
	 *
	 * For example, if you have a map of skills to their levels:
	 * @code
	 * std::map<const SubcategoriedSkillData*, int> skillLevelMap = {
	 *     {skillData1, 5},
	 *     {skillData2, 3}
	 * };
	 * @endcode
	 * You can convert this map into a JSON array using this method:
	 * @code
	 * JsonConverter::setSkillPrimitiveMap<int>(obj, "skillLevels", skillLevelMap);
	 * // The JSON object would now contain an array under "skillLevels" with objects representing each skill and its level
	 * @endcode
	 *
	 * @tparam Primitive The type of the primitive values used as values in the input map (e.g., int, float, double, bool).
	 * @param obj The JSON object to modify.
	 * @param key The key under which to store the skill primitive array in the JSON object.
	 * @param map A map where each key is a pointer to a SubcategoriedSkillData object and each value is a primitive value to be converted into an object in the JSON array.
	 */
	template<typename Primitive>
	static void setSkillPrimitiveMap(json::object& obj, const std::string& key, const std::map<const SubcategoriedSkillData*, Primitive>& map);

	/**
	 * @brief Retrieves a map of SubcategoriedSkillData pointers to enum values from a JSON object using the specified key.
	 *
	 * This function retrieves a JSON array associated with the given key and converts it into a map where the keys are pointers to SubcategoriedSkillData objects and the values are of an enum type specified by the template parameter
	 * EnumType. Each element in the JSON array is expected to be an object with an "id" field that represents the ID of a skill, an optional "subcategory" field that represents the subcategory of the skill, and a "value" field that
	 * represents the enum value associated with that skill as a string. The function uses the provided PersistentObjectManager to look up each skill data object by its ID and subcategory, and converts the string representation of the
	 * enum value to its corresponding enum value using a hypothetical `fromString` function when constructing the resulting map.
	 *
	 * For example, if you have a JSON object like:
	 * @code
	 * {
	 *     "skillCategories": [
	 *         { "id": "SKILL_RIDING", "subcategory": "Horse", "value": "Occupational" },
	 *         { "id": "SKILL_DUPING", "value": "Everyman" }
	 *     ]
	 * }
	 * @endcode
	 * You can retrieve the map of skills to their categories using this method:
	 * @code
	 * json::object obj = ...; // Assume this is your JSON object
	 * std::map<const SubcategoriedSkillData*, SkillCategory> skillCategoryMap = JsonConverter::getSkillEnumMap<SkillCategory>(obj, "skillCategories", manager);
	 * // skillCategoryMap would contain entries mapping SKILL_SWORDSMANSHIP with SUBCATEGORY_MELEE to CATEGORY_OFFENSIVE and SKILL_ARCHERY with no subcategory to CATEGORY_RANGED if those skills exist in the manager and the enum values are
	 * valid
	 * @endcode
	 *
	 * @tparam EnumType The type of the enum values used as values in the resulting map.
	 * @param obj The JSON object to retrieve the skill enum map from.
	 * @param key The key associated with the skill enum array in the JSON object.
	 * @param manager A PersistentObjectManager used to look up skill data objects based on their IDs and subcategories when constructing the map.
	 */
	template<typename EnumType>
	static std::map<const SubcategoriedSkillData*, EnumType> getSkillEnumMap(const json::object& obj, const std::string& key, rm::PersistentObjectManager& manager);

	/**
	 * @brief Sets an array of SubcategoriedSkillData pointers mapped to enum values in a JSON object with the specified key.
	 *
	 * This function takes a map where the keys are pointers to SubcategoriedSkillData objects and the values are of an enum type specified by the template parameter EnumType. It converts this map into a JSON array, where each entry is
	 * represented as an object with an "id" field corresponding to the ID of the skill, an optional "subcategory" field corresponding to the subcategory of the skill, and a "value" field corresponding to the string representation of the
	 * enum value. The resulting JSON array is stored under the specified key in the JSON object.
	 *
	 * For example, if you have a map of skills to their categories:
	 * @code
	 * std::map<const SubcategoriedSkillData*, SkillCategory> skillCategoryMap = {
	 *     {SKILL_RIDING_HORSE, Occupational},
	 *     {SKILL_DUPING, Everyman}
	 * };
	 * @endcode
	 * You can convert this map into a JSON array using this method:
	 * @code
	 * JsonConverter::setSkillEnumMap<SkillCategory>(obj, "skillCategories", skillCategoryMap);
	 * // The JSON object would now contain an array under "skillCategories" with objects representing each skill and its category as a string
	 * @endcode
	 *
	 * @tparam EnumType The type of the enum values used as values in the input map.
	 * @param obj The JSON object to modify.
	 * @param key The key under which to store the skill enum array in the JSON object.
	 * @param map A map where each key is a pointer to a SubcategoriedSkillData object and each value is an enum value to be converted into a string in the JSON array.
	 */
	template<typename EnumType>
	static void setSkillEnumMap(json::object& obj, const std::string& key, const std::map<const SubcategoriedSkillData*, EnumType>& map);

	/**
	 * @brief Retrieves a set of GameRuleDataChoice objects from a JSON object using the specified key.
	 *
	 * This function retrieves a JSON array associated with the given key and converts it into a set of GameRuleDataChoice objects.
	 *
	 * For example, if you have a JSON object like:
	 * @code
	 * {
	 *     "spellListChoices": [
	 *         {
	 *             "numChoices": 1,
	 *             "options": [
	 *                 "SPELLLIST_ANIMAL_MASTERY",
	 *                 "SPELLLIST_HERB_MASTERY",
	 *                 "SPELLLIST_PLANT_MASTERY"
	 *             ]
	 *         },
	 *         {
	 *             "numChoices": 1,
	 *             "options": [
	 *                 "SPELLLIST_FIRE_MASTERY",
	 *                 "SPELLLIST_ICE_MASTERY"
	 *             ]
	 *         }
	 *     ]
	 * }
	 * @endcode
	 * You can retrieve the set of GameRuleDataChoice objects using this method:
	 * @code
	 * json::object obj = ...; // Assume this is your JSON object
	 * std::set<GameRuleDataChoice<ItemData>> choices = JsonConverter::getDataChoiceSet<ItemData>(obj, "choices", manager);
	 * // choices would contain GameRuleDataChoice objects for ITEM_SWORD and ITEM_SHIELD with their corresponding choices if those items exist in the manager
	 * @endcode
	 *
	 * @tparam GameRuleData The type of the data objects used in the GameRuleDataChoice. This type must satisfy the game_rule_data_object concept.
	 * @param obj The JSON object to retrieve the data choice set from.
	 * @param key The key associated with the data choice array in the JSON object.
	 * @param manager A PersistentObjectManager used to look up data objects based on their IDs when constructing the set of GameRuleDataChoice objects.
	 * @return A set of GameRuleDataChoice objects constructed from the entries in the JSON array associated with the specified key.
	 */
	template<game_rule_data_object GameRuleData>
	static std::set<GameRuleDataChoice<GameRuleData>> getDataChoiceSet(const json::object& obj, const std::string& key, rm::PersistentObjectManager& manager);

	/**
	 * @brief Sets an array of GameRuleDataChoice objects in a JSON object with the specified key.
	 *
	 * This function takes a set of GameRuleDataChoice objects and converts it into a JSON array. Each GameRuleDataChoice is represented as a JSON object with fields corresponding to the properties of the GameRuleDataChoice, such as
	 * "numChoices" and "options". The resulting JSON array is stored under the specified key in the JSON object.
	 *
	 * For example, if you have a set of GameRuleDataChoice objects:
	 * @code
	 * std::set<GameRuleDataChoice<ItemData>> choices = {
	 *     GameRuleDataChoice<ItemData>{1, {"ITEM_SWORD", "ITEM_SHIELD"}},
	 *     GameRuleDataChoice<ItemData>{2, {"ITEM_POTION", "ITEM_RUNE", "ITEM_WAND"}}
	 * };
	 * @endcode
	 * You can convert this set into a JSON array using this method:
	 * @code
	 * JsonConverter::setDataChoiceSet(obj, "choices", choices);
	 * // The JSON object would now contain an array under "choices" with objects representing each GameRuleDataChoice
	 * @endcode
	 *
	 * @tparam GameRuleData The type of the data objects used in the GameRuleDataChoice. This type must satisfy the game_rule_data_object concept.
	 * @param obj The JSON object to modify.
	 * @param key The key under which to store the data choice array in the JSON object.
	 * @param choices A set of GameRuleDataChoice objects to be converted into JSON objects and stored as an array in the JSON object.
	 */
	template<game_rule_data_object GameRuleData>
	static void setDataChoiceSet(json::object& obj, const std::string& key, const std::set<GameRuleDataChoice<GameRuleData>>& choices);

	/**
	 * @brief Retrieves a map of GameRuleDataChoice objects to enum values from a JSON object using the specified key.
	 *
	 * This function retrieves a JSON array associated with the given key and converts it into a map where the keys are GameRuleDataChoice objects and the values are of an enum type specified by the template parameter EnumType. Each
	 * element in the JSON array is expected to be an object with fields that correspond to the properties of a GameRuleDataChoice, as well as a "value" field that represents the enum value associated with that choice as a string. The
	 * function uses the provided PersistentObjectManager to look up any necessary data objects when constructing the GameRuleDataChoice objects, and converts the string representation of the enum value to its corresponding enum value
	 * using a hypothetical `fromString` function when constructing the resulting map.
	 *
	 * For example, if you have a JSON object like:
	 * @code
	 * {
	 *     "spellListChoices": [
	 *         {
	 *             "numChoices": 1,
	 *             "value": "Everyman"
	 *             "options": [
	 *                 "SPELLLIST_ANIMAL_MASTERY",
	 *                 "SPELLLIST_HERB_MASTERY",
	 *                 "SPELLLIST_PLANT_MASTERY"
	 *             ],
	 *         },
	 *         {
	 *             "numChoices": 1,
	 *             "value": "Occupational"
	 *             "options": [
	 *                 "SPELLLIST_FIRE_MASTERY",
	 *                 "SPELLLIST_ICE_MASTERY"
	 *             ],
	 *         }
	 *     ]
	 * }
	 * @endcode
	 * You can retrieve the map of choices to their categories using this method:
	 * @code
	 * json::object obj = ...; // Assume this is your JSON object
	 * std::map<GameRuleDataChoice<ItemData>, SpellCategory> choiceCategoryMap = JsonConverter::getSkillChoiceEnumMap<SpellCategory>(obj, "spellListChoices", manager);
	 * // choiceCategoryMap would contain entries mapping each GameRuleDataChoice for the spell list choices to their corresponding SpellCategory enum values if those choices exist in the manager and the enum values are valid
	 * @endcode
	 *
	 * @tparam EnumType The type of the enum values used as values in the resulting map.
	 * @param obj The JSON object to retrieve the skill choice enum map from.
	 * @param key The key associated with the skill choice enum array in the JSON object.
	 * @param manager A PersistentObjectManager used to look up any necessary data objects based on their IDs when constructing the GameRuleDataChoice objects for the keys in the resulting map.
	 * @return A map where each key is a GameRuleDataChoice object constructed from the corresponding entries in the JSON array associated with the specified key, and each value is an enum value associated with that choice constructed from
	 * the "value" field in the JSON object.
	 */
	template<typename EnumType>
	static std::map<GameRuleDataChoice<SubcategoriedSkillData>, EnumType> getSkillChoiceEnumMap(const json::object& obj, const std::string& key, rm::PersistentObjectManager& manager);

	/**
	 * @brief Sets an array of GameRuleDataChoice objects mapped to enum values in a JSON object with the specified key.
	 *
	 * This function takes a map where the keys are GameRuleDataChoice objects and the values are of an enum type specified by the template parameter EnumType. It converts this map into a JSON array, where each entry is represented as
	 * an object with fields corresponding to the properties of the GameRuleDataChoice, as well as a "value" field corresponding to the string representation of the enum value. The resulting JSON array is stored under the specified
	 * key in the JSON object.
	 *
	 * For example, if you have a map of choices to their categories:
	 * @code
	 * std::map<GameRuleDataChoice<ItemData>, SpellCategory> choiceCategoryMap = {
	 *     {GameRuleDataChoice<ItemData>{1, {"ITEM_SWORD", "ITEM_SHIELD"}}, ENUM_VALUE},
	 *     {GameRuleDataChoice<ItemData>{2, {"ITEM_POTION", "ITEM_RUNE", "ITEM_WAND"}}, ENUM_VALUE}
	 * };
	 * @endcode
	 * You can convert this map into a JSON array using this method:
	 * @code
	 * JsonConverter::setSkillChoiceEnumMap<SpellCategory>(obj, "spellListChoices", choiceCategoryMap);
	 * // The JSON object would now contain an array under "spellListChoices" with objects representing each GameRuleDataChoice and its associated SpellCategory as a string
	 * @endcode
	 *
	 * @tparam EnumType The type of the enum values used as values in the input map.
	 * @param obj The JSON object to modify.
	 * @param key The key under which to store the skill choice enum array in the JSON object.
	 * @param map A map where each key is a GameRuleDataChoice object and each value is an enum value to be converted into a string in the JSON array.
	 */
	template<typename EnumType>
	static void setSkillChoiceEnumMap(json::object& obj, const std::string& key, const std::map<GameRuleDataChoice<SubcategoriedSkillData>, EnumType>& map);

	/**
	 * @brief Retrieves a map of GameRuleDataChoice objects to primitive values from a JSON object using the specified key.
	 *
	 * This function retrieves a JSON array associated with the given key and converts it into a map where the keys are GameRuleDataChoice objects and the values are of a primitive type specified by the template parameter Primitive. Each
	 * element in the JSON array is expected to be an object with fields that correspond to the properties of a GameRuleDataChoice, as well as a "value" field that represents the primitive value associated with that choice. The function
	 * uses the provided PersistentObjectManager to look up any necessary data objects when constructing the GameRuleDataChoice objects, and constructs the resulting map accordingly.
	 *
	 * For example, if you have a JSON object like:
	 * @code
	 * {
	 *     "spellListChoices": [
	 *         {
	 *             "numChoices": 1,
	 *             "value": 5
	 *             "options": [
	 *                 "SPELLLIST_ANIMAL_MASTERY",
	 *                 "SPELLLIST_HERB_MASTERY",
	 *                 "SPELLLIST_PLANT_MASTERY"
	 *             ],
	 *         },
	 *         {
	 *             "numChoices": 1,
	 *             "value": 3
	 *             "options": [
	 *                 "SPELLLIST_FIRE_MASTERY",
	 *                 "SPELLLIST_ICE_MASTERY"
	 *             ],
	 *         }
	 *     ]
	 * }
	 * @endcode
	 * You can retrieve the map of choices to their values using this method:
	 * @code
	 * json::object obj = ...; // Assume this is your JSON object
	 * std::map<GameRuleDataChoice<ItemData>, int> choiceValueMap = JsonConverter::getSkillChoicePrimitiveMap<int>(obj, "spellListChoices", manager);
	 * // choiceValueMap would contain entries mapping each GameRuleDataChoice for the spell list choices to their corresponding integer values if those choices exist in the manager and the values are valid
	 * @endcode
	 *
	 * @tparam Primitive The type of the primitive values used as values in the resulting map (e.g., int, float, double, bool).
	 * @param obj The JSON object to retrieve
	 * @param key The key associated with the skill choice primitive array in the JSON object.
	 * @param manager A PersistentObjectManager used to look up any necessary data objects based on
	 * @return A map where each key is a GameRuleDataChoice object constructed from the corresponding entries in the JSON array associated with the specified key, and each value is a primitive value associated with that choice, constructed
	 * from the "value" field in the JSON object.
	 */
	template<typename Primitive>
	static std::map<GameRuleDataChoice<SubcategoriedSkillData>, Primitive> getSkillChoicePrimitiveMap(const json::object& obj, const std::string& key, rm::PersistentObjectManager& manager);

	/**
	 * @brief Sets an array of GameRuleDataChoice objects mapped to primitive values in a JSON object with the specified key.
	 *
	 * This function takes a map where the keys are GameRuleDataChoice objects and the values are of a primitive type specified by the template parameter Primitive. It converts this map into a JSON array, where each entry is represented as
	 * an object with fields corresponding to the properties of the GameRuleDataChoice, as well as a "value" field corresponding to the primitive value. The resulting JSON array is stored under the specified key in the JSON object.
	 *
	 * For example, if you have a map of choices to their values:
	 * @code
	 * std::map<GameRuleDataChoice<ItemData>, int> choiceValueMap = {
	 *     {GameRuleDataChoice<ItemData>{1, {"ITEM_SWORD", "ITEM_SHIELD"}}, 5},
	 *     {GameRuleDataChoice<ItemData>{2, {"ITEM_POTION", "ITEM_RUNE", "ITEM_WAND"}}, 3}
	 * };
	 * @endcode
	 * You can convert this map into a JSON array using this method:
	 * @code
	 * JsonConverter::setSkillChoicePrimitiveMap<int>(obj, "spellListChoices", choiceValueMap);
	 * // The JSON object would now contain an array under "spellListChoices" with objects representing each GameRuleDataChoice and its associated integer value
	 * @endcode
	 *
	 * @tparam Primitive The type of the primitive values used as values in the input map (e.g., int, float, double, bool).
	 * @param obj The JSON object to modify.
	 * @param key The key under which to store the skill choice primitive array in the JSON object.
	 * @param map A map where each key is a GameRuleDataChoice object and each value is a primitive value to be converted into an object in the JSON array.
	 */
	template<typename Primitive>
	static void setSkillChoicePrimitiveMap(json::object& obj, const std::string& key, const std::map<GameRuleDataChoice<SubcategoriedSkillData>, Primitive>& map);

	/**
	 * @brief Retrieves a map of GameRuleDataChoice objects to enum values from a JSON object using the specified key.
	 *
	 * This function retrieves a JSON array associated with the given key and converts it into a map where the keys are GameRuleDataChoice objects and the values are of an enum type specified by the template parameter EnumType. Each
	 * element in the JSON array is expected to be an object with fields that correspond to the properties of a GameRuleDataChoice, as well as a "value" field that represents the enum value associated with that choice as a string. The
	 * function uses the provided PersistentObjectManager to look up any necessary data objects when constructing the GameRuleDataChoice objects, and converts the string representation of the enum value to its corresponding enum value
	 * using a hypothetical `fromString` function when constructing the resulting map.
	 *
	 * For example, if you have a JSON object like:
	 * @code
	 * {
	 *     "spellListChoices": [
	 *         {
	 *             "numChoices": 1,
	 *             "value": "Everyman"
	 *             "options": [
	 *                 "SPELLLIST_ANIMAL_MASTERY",
	 *                 "SPELLLIST_HERB_MASTERY",
	 *                 "SPELLLIST_PLANT_MASTERY"
	 *             ],
	 *         },
	 *         {
	 *             "numChoices": 1,
	 *             "value": "Occupational"
	 *             "options": [
	 *                 "SPELLLIST_FIRE_MASTERY",
	 *                 "SPELLLIST_ICE_MASTERY"
	 *             ],
	 *         }
	 *     ]
	 * }
	 * @endcode
	 * You can retrieve the map of choices to their categories using this method:
	 * @code
	 * json::object obj = ...; // Assume this is your JSON object
	 * std::map<GameRuleDataChoice<ItemData>, SpellCategory> choiceCategoryMap = JsonConverter::getDataChoiceEnumMap<SpellCategory>(obj, "spellListChoices", manager);
	 * // choiceCategoryMap would contain entries mapping each GameRuleDataChoice for the spell list choices to their corresponding SpellCategory enum values if those choices exist in the manager and the enum values are valid
	 * @endcode
	 *
	 * @tparam EnumType The type of the enum values used as values in the resulting map.
	 * @param obj The JSON object to retrieve the skill choice enum map from.
	 * @param key The key associated with the skill choice enum array in the JSON object.
	 * @param manager A PersistentObject Manager used to look up any necessary data objects based on their IDs when constructing the GameRuleDataChoice objects for the keys in the resulting map.
	 * @return A map where each key is a GameRuleDataChoice object constructed from the corresponding entries in the JSON array associated with the specified key, and each value is an enum value associated with that choice constructed from
	 * the "value" field in the JSON object.
	 */
	template<game_rule_data_object GameRuleData, typename EnumType>
	static std::map<GameRuleDataChoice<GameRuleData>, EnumType> getDataChoiceEnumMap(const json::object& obj, const std::string& key, rm::PersistentObjectManager& manager);

	/**
	 * @brief Sets an array of GameRuleDataChoice objects mapped to enum values in a JSON object with the specified key.
	 *
	 * This function takes a map where the keys are GameRuleDataChoice objects and the values are of an enum type specified by the template parameter EnumType. It converts this map into a JSON array, where each entry is represented as
	 * an object with fields corresponding to the properties of the GameRuleDataChoice, as well as a "value" field corresponding to the string representation of the enum value. The resulting JSON array is stored under the specified
	 * key in the JSON object.
	 *
	 * For example, if you have a map of choices to their categories:
	 * @code
	 * std::map<GameRuleDataChoice<ItemData>, SpellCategory> choiceCategoryMap = {
	 *     {GameRuleDataChoice<ItemData>{1, {"ITEM_SWORD", "ITEM_SHIELD"}}, ENUM_VALUE},
	 *     {GameRuleDataChoice<ItemData>{2, {"ITEM_POTION", "ITEM_RUNE", "ITEM_WAND"}}, ENUM_VALUE}
	 * };
	 * @endcode
	 * You can convert this map into a JSON array using this method:
	 * @code
	 * JsonConverter::setDataChoiceEnumMap<SpellCategory>(obj, "spellListChoices", choiceCategoryMap);
	 * // The JSON object would now contain an array under "spellListChoices" with objects representing each GameRuleDataChoice and its associated SpellCategory as a string
	 * @endcode
	 *
	 * @tparam EnumType The type of the enum values used as values in the input map.
	 * @param obj The JSON object to modify.
	 * @param key The key under which to store the skill choice enum array in the JSON object.
	 * @param map A map where each key is a GameRuleDataChoice object and each value is an enum value to be converted into a string in the JSON array.
	 */
	template<game_rule_data_object GameRuleData, typename EnumType>
	static void setDataChoiceEnumMap(json::object& obj, const std::string& key, const std::map<GameRuleDataChoice<GameRuleData>, EnumType>& map);

	/**
	 * @brief Retrieves a map of GameRuleDataChoice objects to primitive values from a JSON object using the specified key.
	 *
	 * This function retrieves a JSON array associated with the given key and converts it into a map where the keys are GameRuleDataChoice objects and the values are of a primitive type specified by the template parameter Primitive. Each
	 * element in the JSON array is expected to be an object with fields that correspond to the properties of a GameRuleDataChoice, as well as a "value" field that represents the primitive value associated with that choice. The function
	 * uses the provided PersistentObjectManager to look up any necessary data objects when constructing the GameRuleDataChoice objects, and constructs the resulting map accordingly.
	 *
	 * For example, if you have a JSON object like:
	 * @code
	 * {
	 *     "spellListChoices": [
	 *         {
	 *             "numChoices": 1,
	 *             "value": 5
	 *             "options": [
	 *                 "SPELLLIST_ANIMAL_MASTERY",
	 *                 "SPELLLIST_HERB_MASTERY",
	 *                 "SPELLLIST_PLANT_MASTERY"
	 *             ],
	 *         },
	 *         {
	 *             "numChoices": 1,
	 *             "value": 3
	 *             "options": [
	 *                 "SPELLLIST_FIRE_MASTERY",
	 *                 "SPELLLIST_ICE_MASTERY"
	 *             ],
	 *         }
	 *     ]
	 * }
	 * @endcode
	 * You can retrieve the map of choices to their values using this method:
	 * @code
	 * json::object obj = ...; // Assume this is your JSON object
	 * std::map<GameRuleDataChoice<ItemData>, int> choiceValueMap = JsonConverter::getDataChoicePrimitiveMap<int>(obj, "spellListChoices", manager);
	 * // choiceValueMap would contain entries mapping each GameRuleDataChoice for the spell list choices to their corresponding integer values if those choices exist in the manager and the values are valid
	 * @endcode
	 *
	 * @tparam Primitive The type of the primitive values used as values in the resulting map (e.g., int, float, double, bool).
	 * @param obj The JSON object to retrieve
	 * @param key The key associated with the skill choice primitive array in the JSON object.
	 * @param manager A PersistentObject Manager used to look up any necessary data objects based on
	 * @return A map where each key is a GameRuleDataChoice object constructed from the corresponding entries in the JSON array associated with the specified key, and each value is a primitive value associated with that choice, constructed
	 * from the "value" field in the JSON object.
	 */
	template<game_rule_data_object GameRuleData, typename Primitive>
	static std::map<GameRuleDataChoice<GameRuleData>, Primitive> getDataChoicePrimitiveMap(const json::object& obj, const std::string& key, rm::PersistentObjectManager& manager);

	/**
	 * @brief Sets an array of GameRuleDataChoice objects mapped to primitive values in a JSON object with the specified key.
	 *
	 * This function takes a map where the keys are GameRuleDataChoice objects and the values are of a primitive type specified by the template parameter Primitive. It converts this map into a JSON array, where each entry is represented as
	 * an object with fields corresponding to the properties of the GameRuleDataChoice, as well as a "value" field corresponding to the primitive value. The resulting JSON array is stored under the specified key in the JSON object.
	 *
	 * For example, if you have a map of choices to their values:
	 * @code
	 * std::map<GameRuleDataChoice<ItemData>, int> choiceValueMap = {
	 *     {GameRuleDataChoice<ItemData>{1, {"ITEM_SWORD", "ITEM_SHIELD"}}, 5},
	 *     {GameRuleDataChoice<ItemData>{2, {"ITEM_POTION", "ITEM_RUNE", "ITEM_WAND"}}, 3}
	 * };
	 * @endcode
	 * You can convert this map into a JSON array using this method:
	 * @code
	 * JsonConverter::setDataChoicePrimitiveMap<int>(obj, "spellListChoices", choiceValueMap);
	 * // The JSON object would now contain an array under "spellListChoices" with objects representing each GameRuleDataChoice and its associated integer value
	 * @endcode
	 *
	 * @tparam Primitive The type of the primitive values used as values in the input map (e.g., int, float, double, bool).
	 * @param obj The JSON object to modify.
	 * @param key The key under which to store the skill choice primitive array in the JSON object.
	 * @param map A map where each key is a GameRuleDataChoice object and each value is a primitive value to be converted into an object in the JSON array.
	 */
	template<game_rule_data_object GameRuleData, typename Primitive>
	static void setDataChoicePrimitiveMap(json::object& obj, const std::string& key, const std::map<GameRuleDataChoice<GameRuleData>, Primitive>& map);

	/**
	 * @brief Retrieves a map of GameRuleData objects to enum values from a JSON object using the specified key.
	 *
	 * This function retrieves a JSON array associated with the given key and converts it into a map where the keys are pointers to GameRuleData objects and the values are of an enum type specified by the template parameter EnumType. Each
	 * element in the JSON array is expected to be an object with an "id" field that represents the ID of a data object, and a "value" field that represents the enum value associated with that data object as a string. The function uses
	 * the provided PersistentObjectManager to look up each data object by its ID, and converts the string representation of the enum value to its corresponding enum value using a hypothetical `fromString` function when constructing the
	 * resulting map.
	 *
	 * For example, if you have a JSON object like:
	 * @code
	 * {
	 *     "itemCategories": [
	 *         { "id": "ITEM_SWORD", "value": "EnumWeapon" },
	 *         { "id": "ITEM_SHIELD", "value": "EnumArmour" }
	 *     ]
	 * }
	 * @endcode
	 * You can retrieve the map of items to their categories using this method:
	 * @code
	 * json::object obj = ...; // Assume this is your JSON object
	 * std::map<const ItemData*, ItemCategory> itemCategoryMap = JsonConverter::getDataEnumMap<ItemData, ItemCategory>(obj, "itemCategories", manager);
	 * // itemCategoryMap would contain entries mapping ITEM_SWORD to Weapon and ITEM_SHIELD to Armor if those items exist in the manager and the enum values are valid
	 * @endcode
	 *
	 * @tparam GameRuleData The type of the data objects used as keys in the resulting map. This type must satisfy the game_rule_data_object concept.
	 * @tparam EnumType The type of the enum values used as values in the resulting map.
	 * @param obj The JSON object to retrieve the data enum map from.
	 * @param key The key associated with the data enum array in the JSON object.
	 * @param manager A PersistentObjectManager used to look up data objects based on their IDs when constructing the map.
	 */
	template<game_rule_data_object GameRuleData, typename EnumType>
	static std::map<const GameRuleData*, EnumType> getDataEnumMap(const json::object& obj, const std::string& key, rm::PersistentObjectManager& manager);

	/**
	 * @brief Sets an array of GameRuleData objects mapped to enum values in a JSON object with the specified key.
	 *
	 * This function takes a map where the keys are pointers to GameRuleData objects and the values are of an enum type specified by the template parameter EnumType. It converts this map into a JSON array, where each entry is represented
	 * as an object with an "id" field corresponding to the ID of the data object and a "value" field corresponding to the string representation of the enum value. The resulting JSON array is stored under the specified key in the JSON
	 * object.
	 *
	 * For example, if you have a map of items to their categories:
	 * @code
	 * std::map<const ItemData*, ItemEnum> itemCategoryMap = {
	 *     {itemData1, EnumWeapon},
	 *     {itemData2, EnumArmour}
	 * };
	 * @endcode
	 * You can convert this map into a JSON array using this method:
	 * @code
	 * JsonConverter::setDataEnumMap<ItemData, ItemCategory>(obj, "itemCategories", itemCategoryMap);
	 * // The JSON object would now contain an array under "itemCategories" with objects representing each item and its category as a string
	 * @endcode
	 *
	 * @tparam GameRuleData The type of the data objects used as keys in the input map. This type must satisfy the game_rule_data_object concept.
	 * @tparam EnumType The type of the enum values used as values in the input map.
	 * @param obj The JSON object to modify.
	 * @param key The key under which to store the data enum array in the JSON object.
	 * @param map A map where each key is a pointer to a data object of type GameRuleData and each value is an enum value to be converted into a string in the JSON array.
	 */
	template<game_rule_data_object GameRuleData, typename EnumType>
	static void setDataEnumMap(json::object& obj, const std::string& key, const std::map<const GameRuleData*, EnumType>& map);

	/**
	 * @brief Retrieves a map of Enumerated objects to primitive values from a JSON object using the specified key.
	 *
	 * This function retrieves a JSON array associated with the given key and converts it into a map where the keys are pointers to Enumerated objects and the values are of a primitive type specified by the template parameter Primitive.
	 * Each element in the JSON array is expected to be an object with an "id" field that represents the ID of an enumeration, and a "value" field that represents the primitive value associated with that enumeration. The function uses the
	 * theoretical toString to look up each enumeration by its string when constructing the resulting map.
	 *
	 * For example, if you have a JSON object like:
	 * @code
	 * {
	 *     "itemQuantities": [
	 *         { "id": "ENUM_VAL1", "value": 5 },
	 *         { "id": "ENUM_VAL2", "value": 3 }
	 *     ]
	 * }
	 * @endcode
	 * You can retrieve the map of enums to their quantities using this method:
	 * @code
	 * json::object obj = ...; // Assume this is your JSON object
	 * std::map<const ItemData*, int> itemQuantityMap = JsonConverter::getDataPrimitiveMap<ItemData, int>(obj, "itemQuantities", manager);
	 * // itemQuantityMap would contain entries mapping ITEM_SWORD to 5 and ITEM_SHIELD to 3 if those items exist in the manager
	 * @endcode
	 *
	 * @tparam EnumType The type of the enumeration used as keys in the resulting map.
	 * @tparam Primitive The type of the primitive values used as values in the resulting map (e.g., int, float, double, bool).
	 * @param obj The JSON object to retrieve the data primitive map from.
	 * @param key The key associated with the data primitive array in the JSON object.
	 * @return A map where each key an Enumerated object corresponding to the "id" field in the JSON array, and each value is a primitive value associated with that enumeration constructed from the "value" field in the JSON
	 */
	template<typename EnumType, typename Primitive>
	static std::map<EnumType, Primitive> getEnumPrimitiveMap(const json::object& obj, const std::string& key);

	/**
	 * @brief Sets an array of Enumerated objects mapped to primitive values in a JSON object with the specified key.
	 *
	 * This function takes a map where the keys are of an enumeration type specified by the template parameter EnumType and the values are of a primitive type specified by the template parameter Primitive. It converts this map into a JSON
	 * array, where each entry is represented as an object with an "id" field corresponding to the string representation of the enumeration and a "value" field corresponding to the primitive value. The resulting JSON array is stored
	 * under the specified key in the JSON object.
	 *
	 * For example, if you have a map of enums to their quantities:
	 * @code
	 * std::map<ItemEnum, int> itemQuantityMap = {
	 *     {ENUM_VAL1, 5},
	 *     {ENUM_VAL2, 3}
	 * };
	 * @endcode
	 * You can convert this map into a JSON array using this method:
	 * @code
	 * JsonConverter::setDataPrimitiveMap<ItemEnum, int>(obj, "itemQuantities", itemQuantityMap);
	 * // The JSON object would now contain an array under "itemQuantities" with objects representing each enum and its quantity
	 * @endcode
	 *
	 * @tparam EnumType The type of the enumeration used as keys in the input map.
	 * @tparam Primitive The type of the primitive values used as values in the input map (e.g., int, float, double, bool).
	 * @param obj The JSON object to modify.
	 * @param key The key under which to store the data primitive array in the JSON object.
	 * @param map A map where each key is an enum value of type EnumType and each value is a primitive value to be converted into an object in the JSON array.
	 */
	template<typename EnumType, typename Primitive>
	static void setEnumPrimitiveMap(json::object& obj, const std::string& key, const std::map<EnumType, Primitive>& map);

	/**
	 * @brief Retrieves a set of strings from a JSON object using the specified key.
	 *
	 * This function retrieves a JSON array associated with the given key and converts it into a set of strings.
	 *
	 * For example, if you have a JSON object like:
	 * @code
	 * {
	 *     "SetOfStuff": [
	 *         "Value 1",
	 *         "Value 2",
	 *         "Value 3"
	 *     ]
	 * }
	 * @endcode
	 * You can retrieve the set of item IDs using this method:
	 * @code
	 * json::object obj = ...; // Assume this is your JSON object
	 * std::set<std::string> itemIds = JsonConverter::getStringSet<ItemData>(obj, "itemIds", manager);
	 * // itemIds would contain "Value 1", "Value 2", and "Value 3"
	 * @endcode
	 *
	 * @param obj The JSON object to retrieve the string set from.
	 * @param key The key associated with the string array in the JSON object.
	 * @return A set of strings in the JSON array.
	 */
	static std::set<std::string> getStringSet(const json::object& obj, const std::string& key);

	/**
	 * @brief Retrieves a subcategorized skill data object from a JSON object.
	 * @param obj The JSON object containing the skill data.
	 * @param manager The persistent object manager used to retrieve or manage the skill data.
	 * @return A pointer to the subcategorized skill data object, or nullptr if the skill could not be retrieved.
	 */
	static const SubcategoriedSkillData* getSkill(const json::object& obj, rm::PersistentObjectManager& manager);

	/**
	 * @brief Sets a subcategorized skill data object in a JSON object.
	 * @param skillData The subcategorized skill data to be set in the JSON object.
	 * @return A JSON object representing the subcategorized skill data, which can be stored in a larger JSON structure.
	 */
	static const json::object setSkill(const SubcategoriedSkillData& skillData);

private:
	template<typename Primitive>
	static Primitive getPrimitive(const json::object& obj, const std::string& key);

};

// Template implementations

template<typename Iterator>
void JsonConverter::setEnumArrayFromIterators(json::object& obj, const std::string& key, Iterator begin, Iterator end) {
	json::array arr;
	for (auto it = begin; it != end; ++it) {
		arr.push_back(json::value(toString(*it)));
	}
	if (arr.size())
		obj[key] = arr;
}

template<typename Container>
void JsonConverter::setEnumSet(json::object& obj, const std::string& key, const Container& enumValues) {
	setEnumArrayFromIterators(obj, key, std::begin(enumValues), std::end(enumValues));
}

template<typename EnumType>
std::set<EnumType> JsonConverter::getEnumSet(const json::object& obj, const std::string& key) {
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

template<typename EnumType>
std::vector<EnumType> JsonConverter::getEnumVector(const json::object& obj, const std::string& key) {
	std::vector<EnumType> result;
	auto it = obj.find(key);
	if (it != obj.end() && it->value().is_array()) {
		for (const auto& item : it->value().as_array()) {
			if (item.is_string()) {
				EnumType enum_value{};
				fromString(item.as_string(), enum_value);
				result.emplace_back(enum_value);
			}
		}
	}
	return result;
}

template<typename Container>
void JsonConverter::setSkillSet(json::object& obj, const std::string& key, const Container& skillValues) {
	json::array arr;
	std::map<std::string, const rm::rule::SubcategoriedSkillData*> sorted_map{};
	for (const auto& data : skillValues) {
		std::string key = data->skillData().id() + (data->subcategory() ? data->subcategory().value() : "");
		sorted_map.emplace(key, data);
	}

	for (const auto& [key, data] : sorted_map) {
		arr.push_back(setSkill(*data));
	}
	if (arr.size())
		obj[key] = arr;
}

template<game_rule_data_object GameRuleData>
std::set<const GameRuleData*> JsonConverter::getDataSet(const json::object& obj, const std::string& key, rm::PersistentObjectManager& manager) {
	std::set<const GameRuleData*> result;
	auto it = obj.find(key);
	if (it != obj.end() && it->value().is_array()) {
		for (const auto& item : it->value().as_array()) {
			if (item.is_string()) {
				const GameRuleData* data_object = &manager.get<GameRuleData>(std::string(item.as_string()));
				if (data_object) {
					result.insert(data_object);
				}
			}
		}
	}
	return result;
}

template<game_rule_data_object GameRuleData>
void JsonConverter::setDataSet(json::object& obj, const std::string& key, const std::set<const GameRuleData*>& data_objects) {
	json::array arr;
	std::map<std::string, const GameRuleData*> sorted_map{};
	for (const GameRuleData* data_object : data_objects) {
		sorted_map.emplace(data_object->id(), data_object);
	}

	for (const auto& [key, data] : sorted_map) {
		arr.push_back(json::value(data->id()));
	}
	if (arr.size())
		obj[key] = arr;
}

template<game_rule_data_object GameRuleData, typename Primitive>
std::map<const GameRuleData*, Primitive> JsonConverter::getDataPrimitiveMap(const json::object& obj, const std::string& key, rm::PersistentObjectManager& manager) {
	std::map<const GameRuleData*, Primitive> result;
	auto it = obj.find(key);
	if (it != obj.end() && it->value().is_array()) {
		for (const auto& item : it->value().as_array()) {
			if (item.is_object()) {
				json::object obj = item.as_object();
				auto id_it = obj.find("id");
				if (id_it != obj.end() && id_it->value().is_string()) {
					const GameRuleData* data_object = &manager.get<GameRuleData>(std::string(id_it->value().as_string()));
					if (data_object) {
						Primitive primitive_value{getPrimitive<Primitive>(obj, "value")};
						result.emplace(data_object, primitive_value);
					}
				}
			}
		}
	}
	return result;
}

template<game_rule_data_object GameRuleData, typename Primitive>
void JsonConverter::setDataPrimitiveMap(json::object& obj, const std::string& key, const std::map<const GameRuleData*, Primitive>& map) {
	json::array arr;
	std::map<std::string, Primitive> sorted_map{};
	for (const auto& [data_object, primitive_value] : map) {
		sorted_map.emplace(data_object->id(), primitive_value);
	}
	for (const auto& [id, primitive_value] : sorted_map) {
		json::object entry_obj;
		entry_obj["id"] = id;
		entry_obj["value"] = primitive_value;
		arr.push_back(entry_obj);
	}
	if (arr.size())
		obj[key] = arr;
}

template<typename Primitive>
std::map<const SubcategoriedSkillData*, Primitive> JsonConverter::getSkillPrimitiveMap(const json::object& obj, const std::string& key, rm::PersistentObjectManager& manager) {
	std::map<const SubcategoriedSkillData*, Primitive> result;
	json::array skill_array = getJsonArray(obj, key);
	for (const auto& skill_val : skill_array) {
		if (!skill_val.is_object())
			continue;

		const SubcategoriedSkillData* skill = getSkill(skill_val.as_object(), manager);
		Primitive primitive_value{getPrimitive<Primitive>(skill_val.as_object(), "value")};

		result.emplace(skill, primitive_value);
	}
	return result;
}

template<typename Primitive>
void JsonConverter::setSkillPrimitiveMap(json::object& obj, const std::string& key, const std::map<const SubcategoriedSkillData*, Primitive>& map) {
	json::array arr;
	std::map<std::string, const rm::rule::SubcategoriedSkillData*> sorted_map{};
	for (const auto& [data, primitive_value] : map) {
		std::string key = data->skillData().id() + (data->subcategory() ? data->subcategory().value() : "");
		sorted_map.emplace(key, data);
	}
	for (const auto& [key, data] : sorted_map) {
		json::object skill_obj = setSkill(*data);
		skill_obj["value"] = map.at(data);
		arr.push_back(skill_obj);
	}
	if (arr.size())
		obj[key] = arr;
}

template<typename EnumType>
std::map<const SubcategoriedSkillData*, EnumType> JsonConverter::getSkillEnumMap(const json::object& obj, const std::string& key, rm::PersistentObjectManager& manager) {
	std::map<const SubcategoriedSkillData*, EnumType> result;
	json::array skill_array = getJsonArray(obj, key);
	for (const auto& skill_val : skill_array) {
		if (!skill_val.is_object())
			continue;

		json::object skill_obj = skill_val.as_object();
		const SubcategoriedSkillData* skill = getSkill(skill_obj, manager);

		EnumType enum_value{};
		std::string enum_str = getString(skill_obj, "value");
		fromString(enum_str, enum_value);
		result.emplace(skill, enum_value);
	}
	return result;
}

template<typename EnumType>
void JsonConverter::setSkillEnumMap(json::object& obj, const std::string& key, const std::map<const SubcategoriedSkillData*, EnumType>& map) {
	json::array arr;
	std::map<std::string, const rm::rule::SubcategoriedSkillData*> sorted_map{};
	for (const auto& [data, primitive_value] : map) {
		std::string key = data->skillData().id() + (data->subcategory() ? data->subcategory().value() : "");
		sorted_map.emplace(key, data);
	}
	for (const auto& [key, data] : sorted_map) {
		json::object skill_obj = setSkill(*data);
		skill_obj["value"] = toString(map.at(data));
		arr.push_back(skill_obj);
	}
	if (arr.size())
		obj[key] = arr;
}

template<game_rule_data_object GameRuleData>
std::set<GameRuleDataChoice<GameRuleData>> JsonConverter::getDataChoiceSet(const json::object& obj, const std::string& key, rm::PersistentObjectManager& manager) {
	std::set<GameRuleDataChoice<GameRuleData>> result;
	auto it = obj.find(key);
	if (it != obj.end() && it->value().is_array()) {
		for (const auto& item : it->value().as_array()) {
			if (item.is_object()) {
				// Grab the choice object and create a GameRuleDataChoice from it
				json::object choice_obj = item.as_object();
				GameRuleDataChoice<GameRuleData> choice_data{};
				choice_data.setNumChoices(getInt(choice_obj, "numChoices", 1));

				// Loop through the options array and add each option to the GameRuleDataChoice
				json::array option_array = getJsonArray(choice_obj, "options");
				for (const auto& option_val : option_array) {
					if (option_val.is_string()) {
						const GameRuleData* data_option = &manager.get<GameRuleData>(std::string(option_val.as_string()));
						if (data_option)
							choice_data.addOption(*data_option);
					}
				}
				// Add the constructed GameRuleDataChoice to the result set
				result.insert(choice_data);
			}
		}
	}
	return result;
}

template<game_rule_data_object GameRuleData>
void JsonConverter::setDataChoiceSet(json::object& obj, const std::string& key, const std::set<GameRuleDataChoice<GameRuleData>>& choices) {
	json::array arr;
	for (const auto& item : choices) {
		json::object choice_obj;
		choice_obj["numChoices"] = item.numChoices();

		std::map<std::string, const GameRuleData*> sorted_options{};
		for (const GameRuleData* option : item.options()) {
			sorted_options.emplace(option->id(), option);
		}

		json::array option_array;
		for (const auto& pair : sorted_options) {
			option_array.emplace_back(pair.second->id());
		}

		choice_obj["options"] = option_array;
		arr.push_back(choice_obj);
	}
	if (arr.size())
		obj[key] = arr;
}

template<typename EnumType>
std::map<GameRuleDataChoice<SubcategoriedSkillData>, EnumType> JsonConverter::getSkillChoiceEnumMap(const json::object& obj, const std::string& key, rm::PersistentObjectManager& manager) {
	std::map<GameRuleDataChoice<SubcategoriedSkillData>, EnumType> result;
	auto it = obj.find(key);
	if (it != obj.end() && it->value().is_array()) {
		for (const auto& item : it->value().as_array()) {
			if (item.is_object()) {
				// Grab the choice object and create a GameRuleDataChoice from it
				json::object choice_obj = item.as_object();
				GameRuleDataChoice<SubcategoriedSkillData> choice_data{};
				choice_data.setNumChoices(getInt(choice_obj, "numChoices", 1));
				// Loop through the options array and add each option to the GameRuleDataChoice
				json::array option_array = getJsonArray(choice_obj, "options");
				for (const auto& option_val : option_array) {
					if (option_val.is_object()) {
						choice_data.addOption(*getSkill(option_val.as_object(), manager));
					}
				}
				// Get the enum value associated with this choice
				EnumType enum_value{};
				std::string enum_str = getString(choice_obj, "type");
				fromString(enum_str, enum_value);
				// Add the constructed GameRuleDataChoice and its associated enum value to the result map
				result.emplace(choice_data, enum_value);
			}
		}
	}
	return result;
}

template<typename EnumType>
void JsonConverter::setSkillChoiceEnumMap(json::object& obj, const std::string& key, const std::map<GameRuleDataChoice<SubcategoriedSkillData>, EnumType>& map) {
	json::array arr;
	for (const auto& [choice, enum_value] : map) {
		json::object choice_obj;
		choice_obj["numChoices"] = choice.numChoices();
		std::map<std::string, const SubcategoriedSkillData*> sorted_options{};
		for (const SubcategoriedSkillData* option : choice.options()) {
			std::string option_key = option->skillData().id() + (option->subcategory() ? option->subcategory().value() : "");
			sorted_options.emplace(option_key, option);
		}
		choice_obj["type"] = toString(enum_value);

		json::array option_array;
		for (const auto& pair : sorted_options) {
			option_array.push_back(setSkill(*pair.second));
		}
		choice_obj["options"] = option_array;
		arr.push_back(choice_obj);
	}
	if (arr.size())
		obj[key] = arr;
}

template<game_rule_data_object GameRuleData, typename EnumType>
std::map<GameRuleDataChoice<GameRuleData>, EnumType> JsonConverter::getDataChoiceEnumMap(const json::object& obj, const std::string& key, rm::PersistentObjectManager& manager) {
	std::map<GameRuleDataChoice<GameRuleData>, EnumType> result;
	auto it = obj.find(key);
	if (it != obj.end() && it->value().is_array()) {
		for (const auto& item : it->value().as_array()) {
			if (item.is_object()) {
				// Grab the choice object and create a GameRuleDataChoice from it
				json::object choice_obj = item.as_object();
				GameRuleDataChoice<GameRuleData> choice_data{};
				choice_data.setNumChoices(getInt(choice_obj, "numChoices", 1));
				// Loop through the options array and add each option to the GameRuleDataChoice
				json::array option_array = getJsonArray(choice_obj, "options");
				for (const auto& option_val : option_array) {
					if (option_val.is_string()) {
						const GameRuleData* data_option = &manager.get<GameRuleData>(std::string(option_val.as_string()));
						if (data_option)
							choice_data.addOption(*data_option);
					}
				}
				// Get the enum value associated with this choice
				EnumType enum_value{};
				std::string enum_str = getString(choice_obj, "type");
				fromString(enum_str, enum_value);
				// Add the constructed GameRuleDataChoice and its associated enum value to the result map
				result.emplace(choice_data, enum_value);
			}
		}
	}
	return result;
}

template<game_rule_data_object GameRuleData, typename EnumType>
void JsonConverter::setDataChoiceEnumMap(json::object& obj, const std::string& key, const std::map<GameRuleDataChoice<GameRuleData>, EnumType>& map) {
	json::array arr;
	for (const auto& [choice, enum_value] : map) {
		json::object choice_obj;
		choice_obj["numChoices"] = choice.numChoices();
		std::map<std::string, const GameRuleData*> sorted_options{};
		for (const GameRuleData* option : choice.options()) {
			sorted_options.emplace(option->id(), option);
		}
		json::array option_array;
		for (const auto& pair : sorted_options) {
			option_array.emplace_back(pair.second->id());
		}
		choice_obj["type"] = toString(enum_value);
		choice_obj["options"] = option_array;
		arr.push_back(choice_obj);
	}
	if (arr.size())
		obj[key] = arr;
}

template<game_rule_data_object GameRuleData, typename EnumType>
std::map<const GameRuleData*, EnumType> JsonConverter::getDataEnumMap(const json::object& obj, const std::string& key, rm::PersistentObjectManager& manager) {
	std::map<const GameRuleData*, EnumType> result;
	json::array skill_array = getJsonArray(obj, key);
	for (const auto& skill_val : skill_array) {
		if (!skill_val.is_object())
			continue;

		json::object skill_obj = skill_val.as_object();
		std::string id = getString(skill_obj, "id");

		EnumType enum_value{};
		std::string enum_str = getString(skill_obj, "value");
		fromString(enum_str, enum_value);
		result.emplace(&manager.get<GameRuleData>(id), enum_value);
	}
	return result;
}

template<game_rule_data_object GameRuleData, typename EnumType>
void JsonConverter::setDataEnumMap(json::object& obj, const std::string& key, const std::map<const GameRuleData*, EnumType>& map) {
	json::array arr;
	std::map<std::string, EnumType> sorted_map{};
	for (const auto& [data, enum_value] : map) {
		std::string key = data->id();
		sorted_map.emplace(key, enum_value);
	}
	for (const auto& [key, enum_value] : sorted_map) {
		json::object skill_obj;
		skill_obj["id"] = key;
		skill_obj["value"] = toString(enum_value);
		arr.push_back(skill_obj);
	}
	if (arr.size())
		obj[key] = arr;
}

template<typename EnumType, typename Primitive>
std::map<EnumType, Primitive> JsonConverter::getEnumPrimitiveMap(const json::object& obj, const std::string& key) {
	std::map<EnumType, Primitive> result;
	auto it = obj.find(key);
	if (it != obj.end() && it->value().is_array()) {
		for (const auto& item : it->value().as_array()) {
			if (item.is_object()) {
				json::object entry_obj = item.as_object();
				std::string enum_str = getString(entry_obj, "id");
				EnumType enum_value{};
				fromString(enum_str, enum_value);
				Primitive primitive_value{getPrimitive<Primitive>(entry_obj, "value")};
				result.emplace(enum_value, primitive_value);
			}
		}
	}
	return result;
}

template<typename EnumType, typename Primitive>
void JsonConverter::setEnumPrimitiveMap(json::object& obj, const std::string& key, const std::map<EnumType, Primitive>& map) {
	json::array arr;
	std::map<std::string, Primitive> sorted_map{};
	for (const auto& [enum_value, primitive_value] : map) {
		std::string key = toString(enum_value);
		sorted_map.emplace(key, primitive_value);
	}
	for (const auto& [key, primitive_value] : sorted_map) {
		json::object entry_obj;
		entry_obj["id"] = key;
		entry_obj["value"] = primitive_value;
		arr.push_back(entry_obj);
	}
	if (arr.size())
		obj[key] = arr;
}

template<typename Container>
void JsonConverter::setStringSet(json::object& obj, const std::string& key, const Container& values) {
	json::array arr;
	for (const auto& value : values) {
		arr.push_back(json::value(value));
	}
	if (arr.size())
		obj[key] = arr;
}

template<typename Primitive>
std::map<GameRuleDataChoice<SubcategoriedSkillData>, Primitive> JsonConverter::getSkillChoicePrimitiveMap(const json::object& obj, const std::string& key, rm::PersistentObjectManager& manager) {
	std::map<GameRuleDataChoice<SubcategoriedSkillData>, Primitive> result;
	auto it = obj.find(key);
	if (it != obj.end() && it->value().is_array()) {
		for (const auto& item : it->value().as_array()) {
			if (item.is_object()) {
				// Grab the choice object and create a GameRuleDataChoice from it
				json::object choice_obj = item.as_object();
				GameRuleDataChoice<SubcategoriedSkillData> choice_data{};
				choice_data.setNumChoices(getInt(choice_obj, "numChoices", 1));
				// Loop through the options array and add each option to the GameRuleDataChoice
				json::array option_array = getJsonArray(choice_obj, "options");
				for (const auto& option_val : option_array) {
					if (option_val.is_object()) {
						choice_data.addOption(*getSkill(option_val.as_object(), manager));
					}
				}
				// Get the primitive value associated with this choice
				Primitive primitive_value{getPrimitive<Primitive>(choice_obj, "value")};

				// Add the constructed GameRuleDataChoice and its associated primitive value to the result map
				result.emplace(choice_data, primitive_value);
			}
		}
	}
	return result;
}

template<typename Primitive>
void JsonConverter::setSkillChoicePrimitiveMap(json::object& obj, const std::string& key, const std::map<GameRuleDataChoice<SubcategoriedSkillData>, Primitive>& map) {
	json::array arr;
	for (const auto& [choice, primitive_value] : map) {
		json::object choice_obj;
		choice_obj["numChoices"] = choice.numChoices();
		std::map<std::string, const SubcategoriedSkillData*> sorted_options{};
		for (const SubcategoriedSkillData* option : choice.options()) {
			std::string option_key = option->skillData().id() + (option->subcategory() ? option->subcategory().value() : "");
			sorted_options.emplace(option_key, option);
		}
		json::array option_array;
		for (const auto& pair : sorted_options) {
			option_array.push_back(setSkill(*pair.second));
		}
		choice_obj["value"] = primitive_value;
		choice_obj["options"] = option_array;
		arr.push_back(choice_obj);
	}
	if (arr.size())
		obj[key] = arr;
}

template<game_rule_data_object GameRuleData, typename Primitive>
std::map<GameRuleDataChoice<GameRuleData>, Primitive> JsonConverter::getDataChoicePrimitiveMap(const json::object& obj, const std::string& key, rm::PersistentObjectManager& manager) {
	std::map<GameRuleDataChoice<GameRuleData>, Primitive> result;
	auto it = obj.find(key);
	if (it != obj.end() && it->value().is_array()) {
		for (const auto& item : it->value().as_array()) {
			if (item.is_object()) {
				// Grab the choice object and create a GameRuleDataChoice from it
				json::object choice_obj = item.as_object();
				GameRuleDataChoice<GameRuleData> choice_data{};
				choice_data.setNumChoices(getInt(choice_obj, "numChoices", 1));
				// Loop through the options array and add each option to the GameRuleDataChoice
				json::array option_array = getJsonArray(choice_obj, "options");
				for (const auto& option_val : option_array) {
					if (option_val.is_string()) {
						const GameRuleData* data_option = &manager.get<GameRuleData>(std::string(option_val.as_string()));
						if (data_option)
							choice_data.addOption(*data_option);
					}
				}
				// Get the primitive value associated with this choice
				Primitive primitive_value{ getPrimitive<Primitive>(choice_obj, "value") };

				// Add the constructed GameRuleDataChoice and its associated primitive value to the result map
				result.emplace(choice_data, primitive_value);
			}
		}
	}
	return result;
}

template<game_rule_data_object GameRuleData, typename Primitive>
void JsonConverter::setDataChoicePrimitiveMap(json::object& obj, const std::string& key, const std::map<GameRuleDataChoice<GameRuleData>, Primitive>& map) {
	json::array arr;
	for (const auto& [choice, primitive_value] : map) {
		json::object choice_obj;
		choice_obj["numChoices"] = choice.numChoices();
		std::map<std::string, const GameRuleData*> sorted_options{};
		for (const GameRuleData* option : choice.options()) {
			sorted_options.emplace(option->id(), option);
		}
		json::array option_array;
		for (const auto& pair : sorted_options) {
			option_array.emplace_back(pair.second->id());
		}
		choice_obj["value"] = primitive_value;
		choice_obj["options"] = option_array;
		arr.push_back(choice_obj);
	}
	if (arr.size())
		obj[key] = arr;
}

template<typename Primitive>
Primitive JsonConverter::getPrimitive(const json::object& obj, const std::string& key) {
	auto it = obj.find(key);
	if (it != obj.end()) {
		const json::value& val = it->value();
		if constexpr (std::is_same_v<Primitive, int>) {
			if (val.is_int64())
				return static_cast<Primitive>(val.as_int64());
		} else if constexpr (std::is_same_v<Primitive, float>) {
			if (val.is_double())
				return static_cast<Primitive>(val.as_double());
		} else if constexpr (std::is_same_v<Primitive, double>) {
			if (val.is_double())
				return val.as_double();
		} else if constexpr (std::is_same_v<Primitive, bool>) {
			if (val.is_bool())
				return val.as_bool();
		} else if constexpr (std::is_same_v<Primitive, std::string>) {
			if (val.is_string())
				return std::string(val.as_string());
		}
	}
	return Primitive{};
}

} // namespace rm::serial
