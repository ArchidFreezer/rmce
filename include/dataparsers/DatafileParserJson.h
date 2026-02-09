#pragma once

#include <iostream>
#include <DatafileParser.h>

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
