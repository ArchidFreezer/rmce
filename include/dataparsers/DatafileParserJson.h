#pragma once

#include <iostream>
#include <DatafileParser.h>
#include <EnumChoice.h>
#include <LanguageAbility.h>
#include <SkillDevelopmentType.h>
#include <StatType.h>

/**
 * @class DatafileParserJson
 * @brief Abstract class to parse json files containing game rule data
 *
 * This class implements the DatafileParser::read function to read the file into a boost ptree structure
 *
 * @see #DatafileParser
 */
class DatafileParserJson : public DatafileParser {
public:
	// We need this to prevent name hiding as we have a method with the same name defined in this class
	using DatafileParser::read;
	
	/**
	 * @brief Constructor
	 * @param cache Cache to use for #LanguageCategoryData objects
	 * @param datatype String containing the type of data to parse
	 * @param filename Path to the datafile to parse
	 */
	DatafileParserJson(GameRuleDataCache& cache, std::string_view datatype, std::string_view filename) : DatafileParser(cache, datatype, filename) {}

	/**
	 * @brief Constructor
	 * @param cache Cache to use for #LanguageCategoryData objects
	 * @param datatype String containing the type of data to parse
	 */
	DatafileParserJson(GameRuleDataCache& cache, std::string_view datatype) : DatafileParser(cache, datatype) {}

	/**
	 * @brief Default destructor
	 *
	 * This is made virtual to define the class as polymorphic as a standard practice, even though there is another virtial method.
	 */
	virtual ~DatafileParserJson() = default;

	/**
	 * @brief Read game rule data from file, convert to objects and store in the game rule data cache
	 * @param filename Path to the file to read the output from
	 */
	void read(const std::string& filename) override;


	/**
	 * @brief Get the key of the root node of the json file
	 * @return String reference to the root key
	 */
	const std::string& rootNode() const { return root_node_; };

	/**
 * @brief Write game rule data from the cache to a file
 * @param filename Path to the file to write the output to
 */
	template<GameRuleDataObject GameRuleData>
	void saveData(const std::string& filename);

	/**
	 * @brief Populate the given boost tree with the data from a game data object
	 * @param id Id of the object to populate from
	 * @param pdatum boost tree to populate
	 */
	virtual void populateDatum(std::string& id, pt::ptree& pdatum) = 0;

protected:
	/**
	 * @brief Set value of the root node key in the json file
	 * @param root_node value of teh root node key
	 */
	void setRootNode(std::string root_node) { root_node_ = root_node; };

	/**
	 * @brief Parse a boost ptree containing a map of game data objects and values into a std::map of pointers to the game data objects and primitive type values
	 * 
	 * The boost ptree expected by this function should be derived from the following json format:
	 * @code{.json}
	 * "root_node": [
	 *   {
	 *     "id": "id of the game data object",
	 *     "value": "value associated with the game data object"
	 *   }
	 * ]
	 * @endcode
	 * 
	 * @tparam GameRuleData Type of the game data objects being parsed, must be derived from GameRuleData
	 * @tparam Primitive Primitive type of the game data values
	 * @param tree Boost ptree containing the map of game data objects and values, with the game data objects represented by their ids
	 * @return Map of pointers to the game data objects and the values, with the game data objects retrieved from the cache using their ids
	 */
	template<GameRuleDataObject GameRuleData, typename Primitive>
	std::map<const GameRuleData*, Primitive> parseGameDataPairTree(boost::optional<const pt::ptree&> tree);

	/**
	 * @brief Parse a std::map of pointers to game data objects and values into a boost ptree containing a map of game data objects and primitive type values
	 *
	 * The boost ptree created by this function will generate the following json format:
	 * @code{.json}
	 * "root_node": [
	 *   {
	 *     "id": "id of the game data object",
	 *     "value": "value associated with the game data object"
	 *   }
	 * ]
	 * @endcode
	 *
	 * @tparam GameRuleData Type of the game data objects being parsed, must be derived from GameRuleData
	 * @tparam Primitive Primitive type of the game data values
	 * @param map Map of pointers to game data objects and values, with the game data objects retrieved from the cache using their ids
	 * @return Boost ptree containing the map of game data objects and values, with the game data objects represented by their ids
	 */
	template<GameRuleDataObject GameRuleData, typename Primitive>
	const pt::ptree getGameDataPairTree(std::map<const GameRuleData*, Primitive> map);

	/**
	 * @brief Parse a boost ptree containing a map of enums strings with associated values into a std::map of enum types and  values
	 *
	 * The boost ptree expected by this function should be derived from the following json format:
	 * @code{.json}
	 * "root_node": [
	 *   {
	 *     "id": "output of {enum type}.toString()",
	 *     "value": "value associated with the enum"
	 *   }
	 * ]
	 * @endcode
	 *
	 * @tparam EnumType Enum type being parsed
	 * @tparam Primitive Value type of the enum values
	 * @param tree Boost ptree containing the map of enum types and associated values, with the enum types represented by the output of their toString() function
	 * @return Map of enum types and associated values
	 */
	template<typename EnumType, typename Primitive>
	std::map<EnumType, Primitive> parseEnumPairTree(boost::optional<const pt::ptree&> tree);

	/**
	 * @brief Creates a property tree from a map of enum key-value pairs.
	 *
	 * The boost ptree created by this function will generate the following json format:
	 * @code{.json}
	 * "root_node": [
	 *   {
	 *     "id": "output of {enum type}.toString()",
	 *     "value": "value associated with the enum"
	 *   }
	 * ]
	 * @endcode
	 *
	 * @tparam EnumType The type of the map keys (typically an enum type).
	 * @tparam Primitive The type of the map values (typically a primitive type).
	 * @param map A map containing enum key-value pairs to convert into a property tree.
	 * @return A const property tree representation of the enum pair map.
	 */
	template<typename EnumType, typename Primitive>
	const pt::ptree getEnumPairTree(std::map<EnumType, Primitive> map);

	/**
	 * @brief Parse a std::map of pointers to game data objects and values into a boost ptree containing a map of game data objects and enum type values
	 *
	 * The boost ptree expected by this function should be derived from the following json format:
	 * @code{.json}
	 * "root_node": [
	 *   {
	 *     "id": "id of the game data object",
	 *     "value": "enum associated with the game data object"
	 *   }
	 * ]
	 * @endcode
	 *
	 * @tparam EnumType Enum type of the game data values
	 * @tparam GameRuleData Type of the game data objects being parsed, must be derived from GameRuleData
	 * @param tree Boost ptree containing the map of game data objects and enum, with the game data objects represented by their ids
	 * @return Map of pointers to the game data objects and the enum, with the game data objects retrieved from the cache using their ids
	 */
	template<GameRuleDataObject GameRuleData, typename EnumType>
	std::map<const GameRuleData*, EnumType> parseGameDataPairEnumTree(boost::optional<const pt::ptree&> tree);

	/**
	 * @brief Parse a std::map of pointers to game data objects and enums into a boost ptree containing a map of game data objects and enum type values
	 *
	 * The boost ptree created by this function will generate the following json format:
	 * @code{.json}
	 * "root_node": [
	 *   {
	 *     "id": "id of the game data object",
	 *     "value": "enum associated with the game data object"
	 *   }
	 * ]
	 * @endcode
	 *
	 * @tparam EnumType Enum type of the game data values
	 * @tparam GameRuleData Type of the game data objects being parsed, must be derived from GameRuleData
	 * @param map Map of pointers to game data objects and values, with the game data objects retrieved from the cache using their ids
	 * @return Boost ptree containing the map of game data objects and values, with the game data objects represented by their ids
	 */
	template<GameRuleDataObject GameRuleData, typename EnumType>
	const pt::ptree getGameDataPairEnumTree(std::map<const GameRuleData*, EnumType> map);

	/**
	 * @brief Parse a boost ptree containing a map of skills and values into a std::map of SubcategoriedSkillData and primitive type values

	 * The boost ptree expected by this function should be derived from the following json format:
	 * @code{.json}
	 * "root_node": [
	 *   {
	 *     "id": "id of the game data object",
	 *     "subcategory": "subcategory of the skill, optional",
	 *     "value": "value associated with the game data object"
	 *   }
	 * ]
	 * @endcode
	 *
	 * @tparam Primitive Primitive type of the skill values
	 * @param tree Boost ptree containing the map of skills and values, with the skills represented by their ids and optional subcategories
	 * @return Map of SubcategoriedSkillData and the values, with the skills retrieved from the cache using their ids and optional subcategories
	 */
	template<typename Primitive>
	std::map<SubcategoriedSkillData, Primitive> parseSkillPairTree(boost::optional<const pt::ptree&> tree);

	/**
	 * @brief Parse a std::map of SubcategoriedSkillData and values into a boost ptree containing a map of skills and primitive type values
	 *
	 * The boost ptree created by this function will generate the following json format:
	 * @code{.json}
	 * "root_node": [
	 *   {
	 *     "id": "id of the game data object",
	 *     "subcategory": "subcategory of the skill, optional",
	 *     "value": "value associated with the game data object"
	 *   }
	 * ]
	 * @endcode
	 *
	 * @tparam Primitive Primitive type of the skill values
	 * @param map Map of SubcategoriedSkillData and values, with the skills retrieved from the cache using their ids and optional subcategories
	 * @return Boost ptree containing the map of skills and values, with the skills represented by their ids and optional subcategories
	 */
	template<typename Primitive>
	const pt::ptree getSkillPairTree(std::map<SubcategoriedSkillData, Primitive> map);

	/**
 * @brief Parse a boost ptree containing a map of skills and enums into a std::map of SubcategoriedSkillData and enum type values
 *
 * The boost ptree expected by this function should be derived from the following json format:
 * @code{.json}
 * "root_node": [
 *   {
 *     "id": "id of the game data object",
 *     "subcategory": "subcategory of the skill, optional",
 *     "value": "enum associated with the game data object"
 *   }
 * ]
 * @endcode
 *
 * @tparam EnumType Enum type of the game data values
 * @param tree Boost ptree containing the map of game data objects and enum, with the game data objects represented by their ids
 * @return Map of pointers to the game data objects and the enum, with the game data objects retrieved from the cache using their ids
 */
	template<typename EnumType>
	std::map<SubcategoriedSkillData, EnumType> parseSkillPairEnumTree(boost::optional<const pt::ptree&> tree);

	/**
	 * @brief Parse a std::map of SubcategoriedSkillData and values into a boost ptree containing a map of skills and enum type values
	 *
	 * The boost ptree created by this function will generate the following json format:
	 * @code{.json}
	 * "root_node": [
	 *   {
	 *     "id": "id of the game data object",
	 *     "subcategory": "subcategory of the skill, optional",
	 *     "value": "value associated with the enum"
	 *   }
	 * ]
	 * @endcode
	 *
	 * @tparam EnumType enum type of the skill values
	 * @param map Map of SubcategoriedSkillData and enums, with the skills retrieved from the cache using their ids and optional subcategories
	 * @return Boost ptree containing the map of skills and enum values, with the skills represented by their ids and optional subcategories
	 */
	template<typename EnumType>
	const pt::ptree getSkillPairEnumTree(std::map<SubcategoriedSkillData, EnumType> map);

	/**
	 * @brief Parse a boost ptree containing a set of game data objects into a std::set of the game data objects
	 *
	 * The boost ptree expected by this function should be derived from the following json format:
	 * @code{.json}
	 * "root_node": [
	 *     "id of game data object 1",
	 *     "id of game data object 2"
	 * ]
	 * @endcode
	 *
	 * @tparam GameRuleData Game data object type, must be derived from GameRuleData
	 * @param tree Boost ptree containing the set of game data objects, with the game data objects represented by their ids
	 * @return Set of the game data objects, with the game data objects retrieved from the cache using their ids
	 */
	template<GameRuleDataObject GameRuleData>
	std::set<const GameRuleData*> parseGameDataSetTree(boost::optional<const pt::ptree&> tree);

	/**
	 * @brief Parse a std::set of game data objects into a boost ptree containing a set of game data objects
	 *
	 * The boost ptree created by this function will generate the following json format:
	 * @code{.json}
	 * "root_node": [
	 *     "id of game data object 1",
	 *     "id of game data object 2"
	 * ]
	 * @endcode
	 *
	 * @tparam GameRuleData Game data object type, must be derived from GameRuleData
	 * @param set Set of game data objects, with the game data objects retrieved from the cache using their ids
	 * @return Boost ptree containing the set of game data objects, with the game data objects represented by their ids
	 */
	template<GameRuleDataObject GameRuleData>
	const pt::ptree getGameDataSetTree(std::set<const GameRuleData*> set);

	/**
	 * @brief Parse a boost ptree containing a set of enums into a std::set of the enum types
	 * @tparam EnumType enum type being parsed
	 * @param tree Boost ptree containing the set of enums, with the enums represented by the output of their toString() function
	 * @return Set of the enum types, with the enums retrieved from the output of their toString() function
	 */
	template<typename EnumType>
	std::set<EnumType> parseEnumSetTree(boost::optional<const pt::ptree&> tree);

	/**
	 * @brief Converts a set of enumeration values into a property tree.
	 * @tparam EnumType The enumeration type of the elements in the set.
	 * @param set The set of enumeration values to convert.
	 * @return A property tree representation of the enumeration set.
	 */
	template<typename EnumType>
	const pt::ptree getEnumSetTree(std::set<EnumType> set);

	/**
	 * @brief Parses a property tree into a vector of enum values.
	 * @tparam EnumType The enum type to parse the values into.
	 * @param tree An optional reference to a property tree containing the enum values to parse.
	 * @return A vector containing the parsed enum values.
	 */
	template<typename EnumType>
	std::vector<EnumType> parseEnumVectorTree(boost::optional<const pt::ptree&> tree);

	/**
	 * @brief Parses a property tree into a set of primitive values.
	 * @tparam Primitive The primitive type of the elements to be stored in the set.
	 * @param tree An optional reference to a property tree to parse. If not present, returns an empty set.
	 * @return A set containing the parsed primitive values from the tree.
	 */
	template<typename Primitive>
	std::set<Primitive> parseSetTree(boost::optional<const pt::ptree&> tree);

	/**
	 * @brief Converts a set of primitive values into a property tree.
	 * @tparam Primitive The primitive type of the elements in the set.
	 * @param set The set of primitive values to convert.
	 * @return A constant property tree representation of the set.
	 */
	template<typename Primitive>
	const pt::ptree getSetTree(std::set<Primitive> set);

	/**
	 * @brief Converts a vector of enum values to a property tree representation.
	 * @tparam EnumType The enum type of the elements in the vector.
	 * @param vector The vector of enum values to convert.
	 * @return A property tree containing the enum vector data.
	 */
	template<typename EnumType>
	const pt::ptree getEnumVectorTree(std::vector<EnumType> vector);

	/**
	 * @brief Parse a boost ptree containing a set of skills into a std::set of SubcategoriedSkillData
	 * @param tree Boost ptree containing the set of skills, with the skills represented by their ids and optional subcategories
	 * @return Set of SubcategoriedSkillData, with the skills retrieved from the cache using their ids and optional subcategories
	 */
	std::set<SubcategoriedSkillData> parseSkillSetTree(boost::optional<const pt::ptree&> tree);

	/**
	 * @brief Parse a std::set of SubcategoriedSkillData into a boost ptree containing a set of skills
	 * @param set Set of SubcategoriedSkillData, with the skills retrieved from the cache using their ids and optional subcategories
	 * @return Boost ptree containing the set of skills, with the skills represented by their ids and optional subcategories
	 */
	const pt::ptree getSkillSetTree(std::set<SubcategoriedSkillData> set);

	/**
	 * @brief Parse a boost ptree containing a map of language abilities into a std::map of LanguageAbility
	 * @param tree Boost ptree containing the map of language abilities, with the languages represented by their ids
	 * @return Map of LanguageAbility, with the languages retrieved from the cache using their ids
	 */
	std::map<std::string, const LanguageAbility> parseLanguageAbilityMapTree(boost::optional<const pt::ptree&> tree);

	/**
	 * @brief Parse a std::map of LanguageAbility into a boost ptree containing a map of language abilities
	 * @param map Map of LanguageAbility, with the languages retrieved from the cache using their ids
	 * @return Boost ptree containing the map of language abilities, with the languages represented by their ids
	 */
	const pt::ptree getLanguageAbilityMapTree(std::map<std::string, const LanguageAbility> map);

	/**
	 * @brief Parses a property tree into a vector of game rule data choices.
	 * @tparam GameRuleData The game rule data object type that satisfies the GameRuleDataObject concept.
	 * @param tree An optional reference to a property tree containing the game data choice vector to parse.
	 * @return A vector of GameRuleDataChoice objects templated on the specified GameRuleData type.
	 */
	template<GameRuleDataObject GameRuleData>
	std::vector<GameRuleDataChoice<GameRuleData>> parseGameDataChoiceVectorTree(boost::optional<const pt::ptree&> tree);

	/**
	 * @brief Converts a vector of game rule data choices into a property tree representation.
	 * @tparam GameRuleData The type of game rule data object that satisfies the GameRuleDataObject concept.
	 * @param vector The vector of game rule data choices to convert.
	 * @return A property tree (ptree) containing the serialized game data choice vector.
	 */
	template<GameRuleDataObject GameRuleData>
	const pt::ptree getGameDataChoiceVectorTree(std::vector<GameRuleDataChoice<GameRuleData>> vector);

	/**
	 * @brief Parses a property tree into a set of game rule data choices.
	 * @tparam GameRuleData The game rule data object type that satisfies the GameRuleDataObject concept.
	 * @param tree An optional reference to a property tree containing the game data choice set to parse.
	 * @return A set of GameRuleDataChoice objects templated on the specified GameRuleData type.
	 */
	template<GameRuleDataObject GameRuleData>
	std::set<GameRuleDataChoice<GameRuleData>> parseGameDataChoiceSetTree(boost::optional<const pt::ptree&> tree);

	/**
	 * @brief Converts a set of game rule data choices into a property tree representation.
	 * @tparam GameRuleData The type of game rule data object that satisfies the GameRuleDataObject concept.
	 * @param set The set of game rule data choices to convert.
	 * @return A property tree (ptree) containing the serialized game data choice set.
	 */
	template<GameRuleDataObject GameRuleData>
	const pt::ptree getGameDataChoiceSetTree(std::set<GameRuleDataChoice<GameRuleData>> set);

	/**
	 * @brief Parses a property tree into a set of skill data choices.
	 * @param tree An optional reference to a property tree containing the skill choice set to parse.
	 * @return A set of GameRuleDataChoice objects templated on SubcategoriedSkillData.
	 */
	std::set<GameRuleDataChoice<SubcategoriedSkillData>> parseSkillChoiceSetTree(boost::optional<const pt::ptree&> tree);

	/**
	 * @brief Converts a set of skill data choices into a property tree representation.
	 * @param set The set of GameRuleDataChoice objects templated on SubcategoriedSkillData to convert.
	 * @return A property tree (ptree) containing the serialized skill choice set.
	 */
	const pt::ptree getSkillChoiceSetTree(std::set<GameRuleDataChoice<SubcategoriedSkillData>> set);

	/**
	 * @brief Parses a property tree into a map of game rule data choices paired with enumeration values.
	 * @tparam EnumType The enumeration type to be associated with each game rule data choice.
	 * @tparam GameRuleData A concept or type constraint specifying the game rule data object type.
	 * @param tree An optional reference to a property tree containing the game data choice-enum pairs to parse.
	 * @return A map where keys are game rule data choices of the specified type and values are the corresponding enumeration values.
	 */
	template<GameRuleDataObject GameRuleData, typename EnumType>
	std::map<GameRuleDataChoice<GameRuleData>, EnumType> parseGameDataChoicePairEnumTree(boost::optional<const pt::ptree&> tree);

	/**
	 * @brief Converts a map of game rule data choices to enum values into a property tree structure.
	 * @tparam EnumType The enum type used as values in the map.
	 * @tparam GameRuleData A type that satisfies the GameRuleDataObject concept, representing the game rule data structure.
	 * @param map A map associating game rule data choices with their corresponding enum values.
	 * @return A property tree (ptree) representation of the game data choice-to-enum mappings.
	 */
	template<GameRuleDataObject GameRuleData, typename EnumType>
	const pt::ptree getGameDataChoicePairEnumTree(std::map<GameRuleDataChoice<GameRuleData>, EnumType> map);

	/**
	 * @brief Parses a property tree into a map of skill data choices to enum values.
	 * @tparam EnumType The enum type to be used as the value in the resulting map.
	 * @param tree An optional reference to a property tree containing the skill choice and enum pair data to parse.
	 * @return A map where keys are game rule data choices containing subcategorized skill data, and values are of the specified enum type.
	 */
	template<typename EnumType>
	std::map<GameRuleDataChoice<SubcategoriedSkillData>, EnumType> parseSkillChoicePairEnumTree(boost::optional<const pt::ptree&> tree);

	/**
	 * @brief Converts a map of skill choice pairs to a property tree representation.
	 * @tparam EnumType The enumeration type used as the value type in the map.
	 * @param map A map where keys are game rule data choices containing subcategorized skill data, and values are enumeration types.
	 * @return A property tree (ptree) containing the serialized representation of the skill choice pair enumeration map.
	 */
	template<typename EnumType>
	const pt::ptree getSkillChoicePairEnumTree(std::map<GameRuleDataChoice<SubcategoriedSkillData>, EnumType> map);

	/**
	 * @brief Parses a property tree into a map of skill data choices paired with primitive values.
	 * @tparam Primitive The primitive type of the values to be paired with skill choices in the resulting map.
	 * @param tree An optional reference to a property tree containing skill choice-primitive pairs to parse.
	 * @return A map where keys are game rule data choices for subcategorized skill data and values are of the specified primitive type.
	 */
	template<typename Primitive>
	std::map<GameRuleDataChoice<SubcategoriedSkillData>, Primitive> parseSkillChoicePairTree(boost::optional<const pt::ptree&> tree);

	/**
	 * @brief Converts a map of skill choice data to a property tree representation.
	 * @tparam Primitive The primitive type used as the value type in the map.
	 * @param map A map where keys are game rule data choices containing subcategorized skill data, and values are primitive type values.
	 * @return A constant property tree (ptree) representation of the skill choice data map.
	 */
	template<typename Primitive>
	const pt::ptree getSkillChoicePairTree(std::map<GameRuleDataChoice<SubcategoriedSkillData>, Primitive> map);

	/**
	 * @brief Parses a property tree into a set of enumeration choices.
	 * @tparam EnumType The enumeration type for the choices.
	 * @param tree An optional reference to a property tree containing the enumeration choices to parse.
	 * @return A set of parsed enumeration choices of the specified enum type.
	 */
	template<typename EnumType>
	std::set<EnumChoice<EnumType>> parseEnumChoiceSetTree(boost::optional<const pt::ptree&> tree);


private:
	std::string root_node_{}; /**< Key of the root node of the json file */
};

template<GameRuleDataObject GameRuleData>
inline void DatafileParserJson::saveData(const std::string& filename) {
	if (filename.empty()) return;

	// Main tree
	pt::ptree tree;

	// Tree of spell lists
	pt::ptree data;

	std::set<std::string> keys{};
	factory().keys<GameRuleData>(keys);

	for (std::string key : keys) {
		try {
			pt::ptree datum;
			populateDatum(key, datum);
			data.push_back(std::make_pair("", datum));
		} catch (const std::out_of_range& e) {
			std::cout << "Error: " << e.what() << std::endl;
		}
	}

	tree.add_child(rootNode(), data);

	pt::write_json(filename, tree);
}

template<GameRuleDataObject GameRuleData, typename Primitive>
inline std::map<const GameRuleData*, Primitive> DatafileParserJson::parseGameDataPairTree(boost::optional<const pt::ptree&> tree) {
	std::map<const GameRuleData*, Primitive> datum{};

	if (tree) {
		for (const auto& items : tree.get()) {
			std::string id{ items.second.get<std::string>("id") };
			datum.emplace(&factory().get<GameRuleData>(id), items.second.get<Primitive>("value"));
		}
	}
	return datum;
}

template<GameRuleDataObject GameRuleData, typename Primitive>
inline const pt::ptree DatafileParserJson::getGameDataPairTree(std::map<const GameRuleData*, Primitive> map) {
	pt::ptree tree{};

	std::map<std::string, const GameRuleData*> sorted_map{};
	for (const auto& pair : map) {
		sorted_map.emplace(pair.first->id(), pair.first);
	}

	for (const auto& pair : sorted_map) {
		pt::ptree value_tree{};
		value_tree.put("id", pair.first);
		value_tree.put("value", map[pair.second]);
		tree.push_back(std::make_pair("", value_tree));
	}
	return tree;
}

template<typename EnumType, typename Primitive>
inline std::map<EnumType, Primitive> DatafileParserJson::parseEnumPairTree(boost::optional<const pt::ptree&> tree) {
	std::map<EnumType, Primitive> datum{};

	if (tree) {
		for (const auto& items : tree.get()) {
			EnumType enum_val{};
			fromString(items.second.get<std::string>("id"), enum_val);
			datum.emplace(enum_val, items.second.get<Primitive>("value"));
		}
	}
	return datum;
}

template<typename EnumType, typename Primitive>
inline const pt::ptree DatafileParserJson::getEnumPairTree(std::map<EnumType, Primitive> map) {
	pt::ptree tree{};

	for (const auto& pair : map) {
		pt::ptree value_tree{};
		value_tree.put("id", toString(pair.first));
		value_tree.put("value", map[pair.first]);
		tree.push_back(std::make_pair("", value_tree));
	}
	return tree;
}

template<GameRuleDataObject GameRuleData, typename EnumType>
inline std::map<const GameRuleData*, EnumType> DatafileParserJson::parseGameDataPairEnumTree(boost::optional<const pt::ptree&> tree) {
	std::map<const GameRuleData*, EnumType> datum{};

	if (tree) {
		for (const auto& items : tree.get()) {
			std::string id{ items.second.get<std::string>("id") };
			EnumType enum_val {};
			fromString(items.second.get<std::string>("value"), enum_val);
			datum.emplace(&factory().get<GameRuleData>(id), enum_val);
		}
	}
	return datum;
}

template<GameRuleDataObject GameRuleData, typename EnumType>
inline const pt::ptree DatafileParserJson::getGameDataPairEnumTree(std::map<const GameRuleData*, EnumType> map) {
	pt::ptree tree{};

	std::map<std::string, const GameRuleData*> sorted_map{};
	for (const auto& pair : map) {
		sorted_map.emplace(pair.first->id(), pair.first);
	}

	for (const auto& pair : sorted_map) {
		pt::ptree value_tree{};
		value_tree.put("id", pair.first);
		value_tree.put("value", toString(map[pair.second]));
		tree.push_back(std::make_pair("", value_tree));
	}
	return tree;
}

template<typename Primitive>
inline std::map<SubcategoriedSkillData, Primitive> DatafileParserJson::parseSkillPairTree(boost::optional<const pt::ptree&> tree) {
	std::map<SubcategoriedSkillData, Primitive> datum{};
	if (tree) {
		for (const auto& items : tree.get()) {
			std::string id{ items.second.get<std::string>("id") };
			boost::optional<std::string> subcategory = items.second.get_optional<std::string>("subcategory");
			if (subcategory) {
				datum.emplace(factory().subcategoriedSkillData(id, subcategory.get()), items.second.get<Primitive>("value"));
			} else {
				datum.emplace(factory().subcategoriedSkillData(id), items.second.get<Primitive>("value"));
			}
		}
	}
	return datum;
}

template<typename Primitive>
inline const pt::ptree DatafileParserJson::getSkillPairTree(std::map<SubcategoriedSkillData, Primitive> map) {
	pt::ptree tree{};

	for (const auto& pair : map) {
		pt::ptree value_tree{};
		value_tree.put("id", pair.first.skillData().id());
		if (pair.first.subcategory()) value_tree.put("subcategory", pair.first.subcategory().value());
		value_tree.put("value", map[pair.first]);
		tree.push_back(std::make_pair("", value_tree));
	}
	return tree;
}

template<typename EnumType>
inline std::map<SubcategoriedSkillData, EnumType> DatafileParserJson::parseSkillPairEnumTree(boost::optional<const pt::ptree&> tree) {
	std::map<SubcategoriedSkillData, EnumType> datum{};
	if (tree) {
		for (const auto& items : tree.get()) {
			std::string id{ items.second.get<std::string>("id") };
			boost::optional<std::string> subcategory = items.second.get_optional<std::string>("subcategory");
			EnumType enum_val{};
			fromString(items.second.get<std::string>("value"), enum_val);
			if (subcategory) {
				datum.emplace(factory().subcategoriedSkillData(id, subcategory.get()), enum_val);
			} else {
				datum.emplace(factory().subcategoriedSkillData(id), enum_val);
			}
		}
	}
	return datum;
}

template<typename EnumType>
inline const pt::ptree DatafileParserJson::getSkillPairEnumTree(std::map<SubcategoriedSkillData, EnumType> map) {
	pt::ptree tree{};

	for (const auto& pair : map) {
		pt::ptree value_tree{};
		value_tree.put("id", pair.first.skillData().id());
		if (pair.first.subcategory()) value_tree.put("subcategory", pair.first.subcategory().value());
		value_tree.put("value", toString(map[pair.first]));
		tree.push_back(std::make_pair("", value_tree));
	}
	return tree;
}

template<GameRuleDataObject GameRuleData>
inline std::set<const GameRuleData*> DatafileParserJson::parseGameDataSetTree(boost::optional<const pt::ptree&> tree) {
	std::set<const GameRuleData*> datum{};
	if (tree) {
		for (const auto& items : tree.get()) {
			std::string id{ items.second.get_value<std::string>() };
			datum.insert(&factory().get<GameRuleData>(id));
		}
	}
	return datum;
}

template<GameRuleDataObject GameRuleData>
inline const pt::ptree DatafileParserJson::getGameDataSetTree(std::set<const GameRuleData*> set) {
	pt::ptree tree{};
	std::set<std::string> sorted_set{};
	for (const auto& item : set) {
		sorted_set.insert(item->id());
	}
	for (const auto& item : sorted_set) {
		pt::ptree value_tree{};
		value_tree.put("", item);
		tree.push_back(std::make_pair("", value_tree));
	}
	return tree;
}

template<typename EnumType>
inline std::set<EnumType> DatafileParserJson::parseEnumSetTree(boost::optional<const pt::ptree&> tree) {
	std::set<EnumType> datum{};
	if (tree) {
		for (const auto& items : tree.get()) {
			EnumType enum_val{};
			fromString(items.second.get_value<std::string>(), enum_val);
			datum.insert(enum_val);
		}
	}
	return datum;
}

template<typename EnumType>
inline const pt::ptree DatafileParserJson::getEnumSetTree(std::set<EnumType> set) {
	pt::ptree tree{};
	for (const auto& item : set) {
		pt::ptree value_tree{};
		value_tree.put("", toString(item));
		tree.push_back(std::make_pair("", value_tree));
	}
	return tree;
}

template<typename Primitive>
inline std::set<Primitive> DatafileParserJson::parseSetTree(boost::optional<const pt::ptree&> tree) {
	std::set<Primitive> datum{};
	if (tree) {
		for (const auto& items : tree.get()) {
			datum.insert(items.second.get_value<Primitive>());
		}
	}
	return datum;
}

template<typename Primitive>
inline const pt::ptree DatafileParserJson::getSetTree(std::set<Primitive> set) {
	pt::ptree tree{};
	for (const auto& item : set) {
		pt::ptree value_tree{};
		value_tree.put("", std::to_string(item));
		tree.push_back(std::make_pair("", value_tree));
	}
	return tree;
}

template<>
inline const pt::ptree DatafileParserJson::getSetTree<std::string>(std::set<std::string> set) {
	pt::ptree tree{};
	for (const auto& item : set) {
		pt::ptree value_tree{};
		value_tree.put("", item);
		tree.push_back(std::make_pair("", value_tree));
	}
	return tree;
}

template<typename EnumType>
inline std::vector<EnumType> DatafileParserJson::parseEnumVectorTree(boost::optional<const pt::ptree&> tree) {
	std::vector<EnumType> datum{};
	if (tree) {
		for (const auto& items : tree.get()) {
			EnumType enum_val{};
			fromString(items.second.get_value<std::string>(), enum_val);
			datum.push_back(enum_val);
		}
	}
	return datum;
}

template<typename EnumType>
inline const pt::ptree DatafileParserJson::getEnumVectorTree(std::vector<EnumType> vector) {
	pt::ptree tree{};
	for (const auto& item : vector) {
		pt::ptree value_tree{};
		value_tree.put("", toString(item));
		tree.push_back(std::make_pair("", value_tree));
	}
	return tree;
}

template<GameRuleDataObject GameRuleData>
inline std::vector<GameRuleDataChoice<GameRuleData>> DatafileParserJson::parseGameDataChoiceVectorTree(boost::optional<const pt::ptree&> tree) {
	std::vector<GameRuleDataChoice<GameRuleData>> datum{};
	if (tree) {
		for (const auto& choice_tree : tree.get()) {
			GameRuleDataChoice<GameRuleData> choice_data{};
			choice_data.setNumChoices(choice_tree.second.get<int>("num-choices"));

			for (const auto& list_tree : choice_tree.second.get_child("options")) {
				std::string list_id{ list_tree.second.get_value<std::string>() };
				choice_data.addOption(factory().get<GameRuleData>(list_id));
			}
			datum.push_back(choice_data);
		}
	}
	return datum;
}

template<GameRuleDataObject GameRuleData>
inline const pt::ptree DatafileParserJson::getGameDataChoiceVectorTree(std::vector<GameRuleDataChoice<GameRuleData>> vector) {
	pt::ptree tree{};
	for (const GameRuleDataChoice<GameRuleData>& item : vector) {
		pt::ptree choice_tree{};
		choice_tree.put("num-choices", item.numChoices());
		pt::ptree options_tree{};

		std::map<std::string, const GameRuleData*> sorted_options{};
		for (const GameRuleData* option : item.options()) {
			sorted_options.emplace(option->id(), option);
		}

		for (const auto& pair: sorted_options) {
			pt::ptree option_tree{};
			option_tree.put("", pair.second->id());
			options_tree.push_back(std::make_pair("", option_tree));
		}
		choice_tree.push_back(std::make_pair("options", options_tree));
		tree.push_back(std::make_pair("", choice_tree));
	}
	return tree;
}

template<GameRuleDataObject GameRuleData>
inline std::set<GameRuleDataChoice<GameRuleData>> DatafileParserJson::parseGameDataChoiceSetTree(boost::optional<const pt::ptree&> tree) {
	std::set<GameRuleDataChoice<GameRuleData>> datum{};
	if (tree) {
		for (const auto& choice_tree : tree.get()) {
			GameRuleDataChoice<GameRuleData> choice_data{};
			choice_data.setNumChoices(choice_tree.second.get<int>("num-choices"));

			for (const auto& list_tree : choice_tree.second.get_child("options")) {
				std::string list_id{ list_tree.second.get_value<std::string>() };
				choice_data.addOption(factory().get<GameRuleData>(list_id));
			}
			datum.emplace(choice_data);
		}
	}
	return datum;
}

template<GameRuleDataObject GameRuleData>
inline const pt::ptree DatafileParserJson::getGameDataChoiceSetTree(std::set<GameRuleDataChoice<GameRuleData>> set) {
	pt::ptree tree{};
	for (const GameRuleDataChoice<GameRuleData>& item : set) {
		pt::ptree choice_tree{};
		choice_tree.put("num-choices", item.numChoices());
		pt::ptree options_tree{};

		std::map<std::string, const GameRuleData*> sorted_options{};
		for (const GameRuleData* option : item.options()) {
			sorted_options.emplace(option->id(), option);
		}

		for (const auto& pair : sorted_options) {
			pt::ptree option_tree{};
			option_tree.put("", pair.second->id());
			options_tree.push_back(std::make_pair("", option_tree));
		}
		choice_tree.push_back(std::make_pair("options", options_tree));
		tree.push_back(std::make_pair("", choice_tree));
	}
	return tree;
}

template<GameRuleDataObject GameRuleData, typename EnumType>
inline std::map<GameRuleDataChoice<GameRuleData>, EnumType> DatafileParserJson::parseGameDataChoicePairEnumTree(boost::optional<const pt::ptree&> tree) {
	std::map<GameRuleDataChoice<GameRuleData>, EnumType> datum{};
	if (tree) {
		for (const auto& choice_tree : tree.get()) {
			GameRuleDataChoice<GameRuleData> choice_data{};
			choice_data.setNumChoices(choice_tree.second.get<int>("num-choices"));
			for (const auto& list_tree : choice_tree.second.get_child("options")) {
				std::string list_id{ list_tree.second.get_value<std::string>() };
				choice_data.addOption(factory().get<GameRuleData>(list_id));
			}
			EnumType enum_val{};
			fromString(choice_tree.second.get<std::string>("type"), enum_val);
			datum.emplace(choice_data, enum_val);
		}
	}
	return datum;
}

template<GameRuleDataObject GameRuleData, typename EnumType>
inline const pt::ptree DatafileParserJson::getGameDataChoicePairEnumTree(std::map<GameRuleDataChoice<GameRuleData>, EnumType> map) {
	pt::ptree tree{};
	for (const auto& pair : map) {
		pt::ptree choice_tree{};
		choice_tree.put("num-choices", pair.first.numChoices());
		choice_tree.put("type", toString(pair.second));
		pt::ptree options_tree{};
		std::map<std::string, const GameRuleData*> sorted_options{};
		for (const GameRuleData* option : pair.first.options()) {
			sorted_options.emplace(option->id(), option);
		}
		for (const auto& option_pair : sorted_options) {
			pt::ptree option_tree{};
			option_tree.put("", option_pair.second->id());
			options_tree.push_back(std::make_pair("", option_tree));
		}
		choice_tree.push_back(std::make_pair("options", options_tree));
		tree.push_back(std::make_pair("", choice_tree));
	}
	return tree;
}

template<typename EnumType>
inline std::map<GameRuleDataChoice<SubcategoriedSkillData>, EnumType> DatafileParserJson::parseSkillChoicePairEnumTree(boost::optional<const pt::ptree&> tree) {
	std::map<GameRuleDataChoice<SubcategoriedSkillData>, EnumType> datum{};
	if (tree) {
		for (const auto& choice_tree : tree.get()) {
			GameRuleDataChoice<SubcategoriedSkillData> choice_data{};
			choice_data.setNumChoices(choice_tree.second.get<int>("num-choices"));
			for (const auto& list_tree : choice_tree.second.get_child("options")) {
				std::string list_id{ list_tree.second.get<std::string>("id") };
				boost::optional<std::string> subcategory = list_tree.second.get_optional<std::string>("subcategory");
				if (subcategory) {
					choice_data.addOption(factory().subcategoriedSkillData(list_id, subcategory.get()));
				} else {
					choice_data.addOption(factory().subcategoriedSkillData(list_id));
				}
			}
			EnumType enum_val{};
			fromString(choice_tree.second.get<std::string>("type"), enum_val);
			datum.emplace(choice_data, enum_val);
		}
	}
	return datum;
}

template<typename EnumType>
inline const pt::ptree DatafileParserJson::getSkillChoicePairEnumTree(std::map<GameRuleDataChoice<SubcategoriedSkillData>, EnumType> map) {
	pt::ptree tree{};
	for (const auto& pair : map) {
		pt::ptree choice_tree{};
		choice_tree.put("num-choices", pair.first.numChoices());
		choice_tree.put("type", toString(pair.second));

		pt::ptree options_tree{};
		std::map<std::string, const SubcategoriedSkillData*> sorted_options{};
		for (const SubcategoriedSkillData* option : pair.first.options()) {
			sorted_options.emplace(option->skillData().id(), option);
		}
		for (const auto& option_pair : sorted_options) {
			pt::ptree option_tree{};
			option_tree.put("id", option_pair.second->skillData().id());
			if (option_pair.second->subcategory()) option_tree.put("subcategory", option_pair.second->subcategory().value());
			options_tree.push_back(std::make_pair("", option_tree));
		}
		choice_tree.push_back(std::make_pair("options", options_tree));
		tree.push_back(std::make_pair("", choice_tree));
	}
	return tree;
}

template<typename Primitive>
inline std::map<GameRuleDataChoice<SubcategoriedSkillData>, Primitive> DatafileParserJson::parseSkillChoicePairTree(boost::optional<const pt::ptree&> tree) {
	std::map<GameRuleDataChoice<SubcategoriedSkillData>, Primitive> datum{};
	if (tree) {
		for (const auto& choice_tree : tree.get()) {
			GameRuleDataChoice<SubcategoriedSkillData> choice_data{};
			choice_data.setNumChoices(choice_tree.second.get<int>("num-choices"));
			for (const auto& list_tree : choice_tree.second.get_child("options")) {
				std::string list_id{ list_tree.second.get<std::string>("id") };
				boost::optional<std::string> subcategory = list_tree.second.get_optional<std::string>("subcategory");
				if (subcategory) {
					choice_data.addOption(factory().subcategoriedSkillData(list_id, subcategory.get()));
				} else {
					choice_data.addOption(factory().subcategoriedSkillData(list_id));
				}
			}
			datum.emplace(choice_data, choice_tree.second.get<int>("value"));
		}
	}
	return datum;
}

template<typename Primitive>
inline const pt::ptree DatafileParserJson::getSkillChoicePairTree(std::map<GameRuleDataChoice<SubcategoriedSkillData>, Primitive> map) {
	pt::ptree tree{};
	for (const auto& pair : map) {
		pt::ptree choice_tree{};
		choice_tree.put("num-choices", pair.first.numChoices());
		choice_tree.put("value", pair.second);

		pt::ptree options_tree{};
		std::map<size_t, const SubcategoriedSkillData*> sorted_options{};
		for (const SubcategoriedSkillData* option : pair.first.options()) {
			size_t hash{ std::hash<std::string>()(option->skillData().id()) };
			hash += option->subcategory() ? std::hash<std::string>()(option->subcategory().value()) : 0;
			hash += std::hash<Primitive>()(pair.second);
			sorted_options.emplace(hash, option);
		}
		for (const auto& option_pair : sorted_options) {
			pt::ptree option_tree{};
			option_tree.put("id", option_pair.second->skillData().id());
			if (option_pair.second->subcategory()) option_tree.put("subcategory", option_pair.second->subcategory().value());
			options_tree.push_back(std::make_pair("", option_tree));
		}
		choice_tree.push_back(std::make_pair("options", options_tree));
		tree.push_back(std::make_pair("", choice_tree));
	}
	return tree;
}

template<typename EnumType>
inline std::set<EnumChoice<EnumType>> DatafileParserJson::parseEnumChoiceSetTree(boost::optional<const pt::ptree&> tree) {
	std::set<EnumChoice<EnumType>> datum{};
	if (tree) {
		for (const auto& choice_tree : tree.get()) {
			EnumChoice<EnumType> choice_data{};
			choice_data.setNumChoices(choice_tree.second.get<int>("num-choices"));
			for (const auto& list_tree : choice_tree.second.get_child("options")) {
				EnumType enum_val{};
				fromString(list_tree.second.get_value<std::string>(), enum_val);
				choice_data.addOption(enum_val);
			}
			datum.emplace(choice_data);
		}
	}
	return datum;
}
