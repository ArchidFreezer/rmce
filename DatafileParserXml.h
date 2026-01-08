#pragma once

#include "DatafileParser.h"

/**
 * @class DatafileParserXml
 * @brief Abstract class to parse xml files containing game rule data
 *
 * This class implements the DatafileParser::read function to read the file into a boost ptree structure
 *
 * @see #DatafileParser
 */
class DatafileParserXml : DatafileParser {
public:
	/**
	 * @brief Read game rule data from an xml file, convert to objects and store in the game rule data cache
	 * @param filename Path to the file to read
	 */
	void read(const std::string& filename);
	/**
	 * @brief Write language game rule data from the cache to an xml file
	 * @param filename Path to the xml file to write
	 */
	void save(const std::string& filename);
};