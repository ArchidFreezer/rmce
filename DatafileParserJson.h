#pragma once

#include "DatafileParser.h"

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
	/**
	 * @brief Constructor
	 * @param cache Cache to use for #LanguageCategoryData objects
	 * @param datatype String containing the type of data to parse
	 */
	DatafileParserJson(GameRuleDataCache& cache, const std::string& datatype);
	/**
	 * @brief Default destructor
	 *
	 * This is made virtual to define the class as polymorphic as a standard practice, even though there is another virtial method.
	 */
	virtual ~DatafileParserJson() = default;

	/**
	 * @brief Read game rule data from a json file, convert to objects and store in the game rule data cache
	 * @param filename Path to the file to read
	 */
	void read(const std::string& filename);

	/**
	 * @brief Get the key of the root node of the json file
	 * @return String reference to the root key
	 */
	const std::string& rootNode() { return root_node_; };

protected:
	std::string root_node_{}; /**< Key of the root node of the json file */
};