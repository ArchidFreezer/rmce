#pragma once
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "GameRuleDataCache.h"

namespace pt = boost::property_tree;

/**
 * @brief Exception thrown if a call is made to read from a file when the filename has not been set
 */
class FilenameNotSetException : public std::runtime_error {
public:
	/**
	 * @brief Exception constructor
	 *
	 * std::runtime_error will handle the string
	 *
	 * @param error String to display for the error
	 */
	FilenameNotSetException(const std::string& error) : std::runtime_error{ error }	{}
};

/**
 * @class DatafileParser
 * 
 * @brief Abstract class to facilitate reading and writing of game rule data to and from files.
 * 
 * This base class has the tools to assist in the processing of game rule data, such as skills, professions, etc., from
 * persistent storage in files. The class makes no assumption regarding the format of the file content other than it
 * is expected to be able to be stored in a boost::property_tree structure.
 * 
 * The class also contains a reference to a data cache where the data objects read from the file should be stored.
 */
class DatafileParser
{
public:
	/**
	 * @brief Consructor
	 * @param cache Reference to a cache object to store the data objects
	 * @param datatype String containing the name of the type of data being processed
	 * @param filename Path to the datafile to parse
	 */
	DatafileParser(GameRuleDataCache& cache, std::string_view datatype, std::string_view filename);

	/**
	 * @brief Consructor
	 * @param cache Reference to a cache object to store the data objects
	 * @param datatype String containing the name of the type of data being processed
	 */
	DatafileParser(GameRuleDataCache& cache, std::string_view datatype);

	/**
	 * @brief Default destructor
	 *
	 * This is made virtual to define the class as polymorphic as a standard practice, even though there is another virtial method.
	 */
	virtual ~DatafileParser() = default;
	
	/**
	 * @brief Getter for the type of data being processed
	 * @return String containing the name of the type of data being processed
	 */
	const std::string& ruleDatatype() { return rule_datatype_; }

	/**
	 * @brief Read game rule data from file, convert to objects and store in the game rule data cache
	 * @param filename Path to the file to read the output from
	 */
	virtual void read(const std::string& filename) = 0;

	/**
	 * @brief Read game rule data from file, convert to objects and store in the game rule data cache
	 * 
	 * The file to be read from may be set in either the constructor or via methods
	 * @see setFilename
	 */
	inline void read() { read(filename()); }

	/**
	 * @brief Write game rule data from the cache to a file
	 * @param filename Path to the file to write the output to
	 */
	virtual void save(const std::string& filename) = 0;

	/**
	 * @brief Write game rule data from the cache to a file
	 *
	 * The file to save to from may be set in either the constructor or via methods
	 * @see setFilename
	 */
	inline void save() { save(filename()); }

	/**
	 * @brief Creates a string that can be used as a unique identifier for an object
	 * 
	 * Each object requires a unique identifier and this is a helper function that can create one in a standard format. The
	 * output is a string that is a combination of the type and val parameters that only contain ucase letters, numbers and
	 * the underscore character. Any character that is not an alphanum will be converted to an underscore with consecutive
	 * underscores being discarded.
	 * 
	 * @param type The type of data object
	 * @param val The unique name of the data object
	 * @return String containing an id in standard format
	 */
	static const std::string generateId(std::string_view type, std::string_view val);

	/**
	 * @brief Get the boost ptree used when reading the datafile
	 * @return  Reference to the boost ptree
	 */
	inline pt::ptree& ptree() { return ptree_; };

	/**
	 * @brief Get the game rule data cache
	 * @return Reference to the game rule data cache
	 */
	inline GameRuleDataCache& cache() { return cache_; };

	/**
	 * @brief Get the file to parse
	 * @return Pathe to the file to parse
	 */
	inline const std::string& filename() { return filename_; };

	/**
	 * @brief Set the file to pasre
	 * @param filename Path to the file to parse
	 */
	inline void setFilename(const std::string& filename) { filename_ = filename; };

protected:
	/**
	 * @brief Parse a ptree into game data objects
	 *
	 * Parse a boost::ptree containing the language rule data, convert to objects and store in a data cache
	 */
	virtual void parse() = 0;

private:
	GameRuleDataCache& cache_; /**< Reference to a cache object to store the data objects */
	pt::ptree ptree_{}; /**< Boost ptree to use when reading structured data file files */
	std::string rule_datatype_{}; /**< Name of the type of data being processed */
	std::string filename_{}; /**< Path to the file to parse */

};


