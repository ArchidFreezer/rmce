#pragma once

#include <DatafileParserXml.h>
#include <ProfessionData.h>

namespace rm::rule::parser {

	/**
	 * @class ProfessionDatafileParserXml
	 * @brief Process ProfessionData objects for xml files
	 *
	 * This class can read a well-formed xml file creating ProfessionData objects from the contents, adding them to a cache.
	 *
	 * @see ProfessionData
	 */

	class ProfessionDatafileParserXml : public DatafileParserXml {
	public:
		// We need this to prevent name hiding as we have a method with the same name defined in this class
		using DatafileParserBoost::save;

		/**
		 * @brief Constructor
		 * @param object_manager Reference to an object manager to handle the data objects
		 * @param filename Path to the datafile to parse
		 */
		ProfessionDatafileParserXml(rm::GameRuleDataFactory& object_manager, std::string_view filename) : DatafileParserXml(object_manager, "Profession", filename) {
			setRootNode("ProfessionData.professions");
		}

		/**
		 * @brief Constructor
		 * @param object_manager Reference to an object manager to handle the data objects
		 */
		ProfessionDatafileParserXml(rm::GameRuleDataFactory& object_manager) : ProfessionDatafileParserXml(object_manager, "") {}

	private:
		/**
		 * @brief Parse a ptree into ProfessionData objects
		 *
		 * Parse a boost::ptree containing the book rule datas, convert to ProfessionData objects and store in a data cache
		 */
		void parse() override;
	};

} // namespace rm::rule::parser