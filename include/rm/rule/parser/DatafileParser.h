#pragma once
#include <string>

#include <GameRuleDataCache.h>
#include <GameRuleDataChoice.h>
#include <GameRuleDataFactory.h>

/**
 * @namespace rm::rule::parser
 * @brief Datafile parsers to (de)serialise game data to and from files
 */
namespace rm::rule::parser {

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
	 * @brief Abstract class to facilitate reading and writing of game rule data to and from files.
	 *
	 * This base class has the tools to assist in the processing of game rule data, such as skills, professions, etc., from
	 * persistent storage in files.
	 *
	 * The class also contains a reference to a data cache where the data objects read from the file should be stored.
	 */
	class DatafileParser {
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
			filename_{ filename },
			factory_{ GameRuleDataFactory(cache) } {}

		/**
		 * @brief Consructor
		 * @param cache Reference to a cache object to store the data objects
		 * @param datatype String containing the name of the type of data being processed
		 */
		DatafileParser(GameRuleDataCache& cache, std::string_view datatype) : DatafileParser(cache, datatype, "") {}

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
		 */
		virtual void read(const std::string& filename) = 0;

		/**
		 * @brief Read game rule data from file, convert to objects and store in the game rule data cache
		 *
		 * The file to be read from may be set in either the constructor or via methods
		 * @see setFilename
		 */
		void read() { read(filename()); }

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
		 * @brief Get the game rule factoryfactorydata cache
		 */
		GameRuleDataFactory& factory() { return factory_; };

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

	private:
		GameRuleDataCache& cache_; /**< Reference to a cache object to store the data objects */
		GameRuleDataFactory factory_; /**< Factory to act as wrapper to cache, creating objects if required */
		std::string rule_datatype_{}; /**< Name of the type of data being processed */
		std::string filename_{}; /**< Path to the file to parse */

	};

} // namespace rm::rule::parser