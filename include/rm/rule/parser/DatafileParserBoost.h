#pragma once
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <DatafileParser.h>

namespace rm::rule::parser {

	namespace pt = boost::property_tree;

	/**
	 * @class DatafileParserBoost
	 *
	 * @brief Abstract class to facilitate reading and writing of game rule data to and from files.
	 *
	 * This base class has the tools to assist in the processing of game rule data, such as skills, professions, etc., from
	 * persistent storage in files. The class makes no assumption regarding the format of the file content other than it
	 * is expected to be able to be stored in a boost::property_tree structure.
	 *
	 * The class also contains a reference to a data cache where the data objects read from the file should be stored.
	 */
	class DatafileParserBoost : public DatafileParser {
	public:
		/**
		 * @brief Consructor
		 * @param object_manager Reference to an object manager to handle the data objects
		 * @param datatype String containing the name of the type of data being processed
		 * @param filename Path to the datafile to parse
		 */
		DatafileParserBoost(rm::PersistentObjectManager& object_manager, std::string_view datatype, std::string_view filename) : DatafileParser(object_manager, datatype, filename) {}

		/**
		 * @brief Consructor
		 * @param object_manager Reference to an object manager to handle the data objects
		 * @param datatype String containing the name of the type of data being processed
		 */
		DatafileParserBoost(rm::PersistentObjectManager& object_manager, std::string_view datatype) : DatafileParserBoost(object_manager, datatype, "") {}

		/**
		 * @brief Default destructor
		 *
		 * This is made virtual to define the class as polymorphic as a standard practice, even though there is another virtial method.
		 */
		virtual ~DatafileParserBoost() = default;

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

	protected:
		/**
		 * @brief Parse a ptree into game data objects
		 *
		 * Parse a boost::ptree containing the language rule data, convert to objects and store in a data cache
		 */
		virtual void parse() = 0;

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
		pt::ptree ptree_{}; /**< Boost ptree to use when reading structured data file files */

	};

	template<GameRuleDataObject T>
	inline void DatafileParserBoost::populateGameRuleDataChoice(const GameRuleDataChoice<T>* game_data_choice, pt::ptree& tree) {

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

} // namespace rm::rule::parser