#pragma once

#include <iostream>
#include <DatafileParser.h>
#include <LanguageAbility.h>

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
	template<class T>
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
	 * @tparam U Primitive type of the game data values
	 * @tparam T Type of the game data objects being parsed, must be derived from GameRuleData
	 * @param tree Boost ptree containing the map of game data objects and values, with the game data objects represented by their ids
	 * @return Map of pointers to the game data objects and the values, with the game data objects retrieved from the cache using their ids
	 */
	template<GameRuleDataObject T, typename U>
	inline std::map<const T*, U> parseGameDataPairTree(boost::optional<const pt::ptree&> tree);

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
	 * @tparam U Primitive type of the game data values
	 * @tparam T Game data object type, must be derived from GameRuleData
	 * @param map Map of pointers to game data objects and values, with the game data objects retrieved from the cache using their ids
	 * @return Boost ptree containing the map of game data objects and values, with the game data objects represented by their ids
	 */
	template<GameRuleDataObject T, typename U>
	inline const pt::ptree getGameDataPairTree(std::map<const T*, U> map);

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
	 * @tparam U Primitive type of the skill values
	 * @param tree Boost ptree containing the map of skills and values, with the skills represented by their ids and optional subcategories
	 * @return Map of SubcategoriedSkillData and the values, with the skills retrieved from the cache using their ids and optional subcategories
	 */
	template<typename U>
	inline std::map<SubcategoriedSkillData, U> parseSkillPairTree(boost::optional<const pt::ptree&> tree);

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
	 * @tparam U Primitive type of the skill values
	 * @param map Map of SubcategoriedSkillData and values, with the skills retrieved from the cache using their ids and optional subcategories
	 * @return Boost ptree containing the map of skills and values, with the skills represented by their ids and optional subcategories
	 */
	template<typename U>
	inline const pt::ptree getSkillPairTree(std::map<SubcategoriedSkillData, U> map);

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
	 * @tparam T Game data object type, must be derived from GameRuleData
	 * @param tree Boost ptree containing the set of game data objects, with the game data objects represented by their ids
	 * @return Set of the game data objects, with the game data objects retrieved from the cache using their ids
	 */
	template<GameRuleDataObject T>
	inline std::set<const T*> parseGameDataSetTree(boost::optional<const pt::ptree&> tree);

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
	 * @tparam T Game data object type, must be derived from GameRuleData
	 * @param set Set of game data objects, with the game data objects retrieved from the cache using their ids
	 * @return Boost ptree containing the set of game data objects, with the game data objects represented by their ids
	 */
	template<GameRuleDataObject T>
	inline const pt::ptree getGameDataSetTree(std::set<const T*> set);

	/**
	 * @brief Parse a boost ptree containing a set of skills into a std::set of SubcategoriedSkillData
	 * @param tree Boost ptree containing the set of skills, with the skills represented by their ids and optional subcategories
	 * @return Set of SubcategoriedSkillData, with the skills retrieved from the cache using their ids and optional subcategories
	 */
	const std::set<SubcategoriedSkillData> parseSkillSetTree(boost::optional<const pt::ptree&> tree);

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
	const std::map<std::string, const LanguageAbility> parseLanguageAbilityMapTree(boost::optional<const pt::ptree&> tree);

	/**
	 * @brief Parse a std::map of LanguageAbility into a boost ptree containing a map of language abilities
	 * @param map Map of LanguageAbility, with the languages retrieved from the cache using their ids
	 * @return Boost ptree containing the map of language abilities, with the languages represented by their ids
	 */
	const pt::ptree getLanguageAbilityMapTree(std::map<std::string, const LanguageAbility> map);

private:
	std::string root_node_{}; /**< Key of the root node of the json file */
};

template<class T>
inline void DatafileParserJson::saveData(const std::string& filename) {
	if (filename.empty()) return;

	// Main tree
	pt::ptree tree;

	// Tree of spell lists
	pt::ptree data;

	std::set<std::string> keys{};
	factory().keys<T>(keys);

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

template<GameRuleDataObject T, typename U>
inline std::map<const T*, U> DatafileParserJson::parseGameDataPairTree(boost::optional<const pt::ptree&> tree) {
	std::map<const T*, U> datum{};

	if (tree) {
		for (const auto& items : tree.get()) {
			std::string id{ items.second.get<std::string>("id") };
			datum.emplace(&factory().get<T>(id), items.second.get<U>("value"));
		}
	}
	return datum;
}

template<GameRuleDataObject T, typename U>
inline const pt::ptree DatafileParserJson::getGameDataPairTree(std::map<const T*, U> map) {
	pt::ptree tree{};

	std::map<std::string, const T*> sorted_map{};
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

template<typename U>
inline std::map<SubcategoriedSkillData, U> DatafileParserJson::parseSkillPairTree(boost::optional<const pt::ptree&> tree) {
	std::map<SubcategoriedSkillData, U> datum{};
	if (tree) {
		for (const auto& items : tree.get()) {
			std::string id{ items.second.get<std::string>("id") };
			boost::optional<std::string> subcategory = items.second.get_optional<std::string>("subcategory");
			if (subcategory) {
				datum.emplace(factory().subcategoriedSkillData(id, subcategory.get()), items.second.get<U>("value"));
			} else {
				datum.emplace(factory().subcategoriedSkillData(id), items.second.get<U>("value"));
			}
		}
	}
	return datum;
}

template<typename U>
inline const pt::ptree DatafileParserJson::getSkillPairTree(std::map<SubcategoriedSkillData, U> map) {
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

template<GameRuleDataObject T>
inline std::set<const T*> DatafileParserJson::parseGameDataSetTree(boost::optional<const pt::ptree&> tree) {
	std::set<const T*> datum{};
	if (tree) {
		for (const auto& items : tree.get()) {
			std::string id{ items.second.get_value<std::string>() };
			datum.insert(&factory().get<T>(id));
		}
	}
	return datum;
}

template<GameRuleDataObject T>
inline const pt::ptree DatafileParserJson::getGameDataSetTree(std::set<const T*> set) {
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
