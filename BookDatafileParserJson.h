#pragma once

#include "DatafileParserJson.h"
#include "GameRuleDataCache.h"

/**
 * @class BookDatafileParserJson
 * @brief Process #BookData objects for json files
 *
 * This class can read a well-formed json file creating #BookData objects from the contents, adding them to a
 * #GameRuleDataCache cache.
 *
 * The reverse operation is also available where the objects from the cache may be written out to a json file.
 *
 * @see #BookData
 * @see #GameRuleDataCache
 */

class BookDatafileParserJson : public DatafileParserJson {
public:
	/**
	 * @brief Constructor
	 * @param cache Cache to use for #BookData objects
	 */
	BookDatafileParserJson(GameRuleDataCache& cache);

	/**
	 * @brief Write book game rule data from the cache to a json file
	 * @param filename Path to the json file to write
	 */
	void save(const std::string& filename);

private:
	/**
	 * @brief Parse a ptree into #BookData objects
	 *
	 * Parse a boost::ptree containing the book rule datas, convert to #BookData objects and store in a data cache
	 */
	void parse();
};