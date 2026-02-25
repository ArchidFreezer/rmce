#pragma once
#include <iostream>
#include "DatafileParserBoost.h"

/**
 * @class DatafileParserXml
 * @brief Abstract class to parse xml files containing game rule data
 *
 * This class implements the DatafileParserBoost::read function to read the file into a boost ptree structure
 *
 * @see #DatafileParserBoost
 */
class DatafileParserXml : public DatafileParserBoost {
public:
	// We need this to prevent name hiding as we have a method with the same name defined in this class
	using DatafileParserBoost::read;

	/**
	 * @brief Constructor
	 * @param cache Cache to use for #LanguageCategoryData objects
	 * @param datatype String containing the type of data to parse
	 * @param filename Path to the datafile to parse
	 */
	DatafileParserXml(GameRuleDataCache& cache, std::string_view datatype, std::string_view filename) : DatafileParserBoost(cache, datatype, filename) {}

	/**
	 * @brief Constructor
	 * @param cache Cache to use for #LanguageCategoryData objects
	 * @param datatype String containing the type of data to parse
	 */
	DatafileParserXml(GameRuleDataCache& cache, std::string_view datatype) : DatafileParserBoost(cache, datatype) {}

	/**
	 * @brief Read game rule data from file, convert to objects and store in the game rule data cache
	 * @param filename Path to the file to read the output from
	 */
	void read(const std::string& filename) override;

	/**
	 * @brief Write language game rule data from the cache to an xml file
	 * @param filename Path to the xml file to write
	 */
	void save(const std::string& filename) override {};

	/**
	 * @brief Get the key of the root node of the json file
	 * @return String reference to the root key
	 */
	const std::string& rootNode() const { return root_node_; };

protected:
	/**
	 * @brief Set value of the root node key in the json file
	 * @param root_node value of the root node key
	 */
	void setRootNode(std::string root_node) { root_node_ = root_node; };

private:
	std::string root_node_{}; /**< Key of the root node of the json file */
};