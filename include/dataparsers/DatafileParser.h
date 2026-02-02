#pragma once
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <GameRuleDataCache.h>
#include <GameRuleDataChoice.h>

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
	DatafileParser(GameRuleDataCache& cache, std::string_view datatype, std::string_view filename) :
		cache_{ cache },
		rule_datatype_{ datatype },
		filename_{ filename } {}

	/**
	 * @brief Consructor
	 * @param cache Reference to a cache object to store the data objects
	 * @param datatype String containing the name of the type of data being processed
	 */
	DatafileParser(GameRuleDataCache& cache, std::string_view datatype) :	DatafileParser(cache, datatype, "") {}

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
	const std::string& ruleDatatype() const { return rule_datatype_; }

	/**
	 * @brief Read game rule data from file, convert to objects and store in the game rule data cache
	 * @param filename Path to the file to read the output from
	 * @param id_only `true` if the game rule object should be populated with its id only; `false` otherwise
	 */
	virtual void read(const std::string& filename, bool id_only) = 0;

	/**
	 * @brief Read game rule data from file, convert to objects and store in the game rule data cache
	 * 
	 * The file to be read from may be set in either the constructor or via methods
	 * @param id_only `true` if the game rule object should be populated with its id only; `false` otherwise
	 * @see setFilename
	 */
	void read(bool id_only) { read(filename(), id_only); }

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
	void save() { save(filename()); }

	/**
	 * @brief Clears all children from the boost ptree
	 * 
	 * This forces the datafile to be re-read
	 */
	void resetPtree() { ptree_.clear(); };

	/**
	 * @brief Get the boost ptree used when reading the datafile
	 * @return  Reference to the boost ptree
	 */
	pt::ptree& ptree() { return ptree_; };

	/**
	 * @brief Get the game rule data cache
	 * @return Reference to the game rule data cache
	 */
	GameRuleDataCache& cache() { return cache_; };

	/**
	 * @brief Get the file to parse
	 * @return Pathe to the file to parse
	 */
	const std::string& filename() const { return filename_; };

	/**
	 * @brief Set the file to pasre
	 * @param filename Path to the file to parse
	 */
	void setFilename(const std::string& filename) { filename_ = filename; };

protected:
	/**
	 * @brief Parse a ptree into game data objects
	 *
	 * Parse a boost::ptree containing the language rule data, convert to objects and store in a data cache
	 * @param id_only `true` if the game rule object should be populated with its id only; `false` otherwise
	 */
	virtual void parse(bool id_only) = 0;

	/**
	 * @brief Process a GameRuleDataChoice object into a boost pt::ptree
	 *
	 * Utility function to simplify the parsing code, taking a GameRuleDataChoice<@a T> object and creating its
	 * representation as a boost pt::ptree ready for writing to file.
	 *
	 * @tparam T GameRuleData object type being parsed
	 * @param game_data_choice A pointer to the choice object to process
	 * @param[in,out] tree boost pt::ptree to populate
	 */
	template<GameRuleDataObject T>
	void populateGameRuleDataChoice(const GameRuleDataChoice<T>* game_data_choice, pt::ptree& tree);

private:
	GameRuleDataCache& cache_; /**< Reference to a cache object to store the data objects */
	pt::ptree ptree_{}; /**< Boost ptree to use when reading structured data file files */
	std::string rule_datatype_{}; /**< Name of the type of data being processed */
	std::string filename_{}; /**< Path to the file to parse */

};

template<GameRuleDataObject T>
inline void DatafileParser::populateGameRuleDataChoice(const GameRuleDataChoice<T>* game_data_choice, pt::ptree& tree) {

	// We want the same json output each time so we sort the data before we write them
	std::set<std::string> option_set{};
	for (auto& data : game_data_choice->options()) {
		option_set.insert(data->id());
	}

	// Short circuit if we have no options
	if (option_set.empty()) return;

	tree.put("num-choices", game_data_choice->numChoices());

	// Add the array container
	pt::ptree options_tree{};
	for (auto& data : option_set) {  // Loop through the sorted categories
		// Create array elements
		pt::ptree option_tree{};
		option_tree.put("", data);
		// Add the elements to the array conatiner
		options_tree.push_back(std::make_pair("", option_tree));
	}
	// Add the array container to the main tree
	tree.push_back(std::make_pair("options", options_tree));
}