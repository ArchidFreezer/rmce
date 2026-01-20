#pragma once

#include <DatafileParserJson.h>
#include <GameRuleDataCache.h>
#include <BookData.h>

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
	// We need this to prevent name hiding as we have a method with the same name defined in this class
	using DatafileParser::save;

	/**
	 * @brief Constructor
	 * @param cache Cache to use for #BookData objects
	 * @param filename Path to the datafile to parse
	 */
	BookDatafileParserJson(GameRuleDataCache& cache, std::string_view filename);

	/**
	 * @brief Constructor
	 * @param cache Cache to use for #BookData objects
	 */
	BookDatafileParserJson(GameRuleDataCache& cache);

	/**
	 * @brief Write book game rule data from the cache to a json file
	 * @param filename Path to the json file to write
	 */
	void save(const std::string& filename) override { saveData<BookData>(filename); }


	/**
	 * @brief Populate the given boost tree with the data from a book
	 * @param id Id of the book to populate from
	 * @param pdatum boost tree to populate
	 */
	void populateDatum(std::string& id, pt::ptree& pdatum) override;

private:
	/**
	 * @brief Parse a ptree into #BookData objects
	 *
	 * Parse a boost::ptree containing the book rule datas, convert to #BookData objects and store in a data cache
	 * @param id_only `true` if the game rule object should be populated with its id only; `false` otherwise
	 */
	void parse(bool id_only) override;
};