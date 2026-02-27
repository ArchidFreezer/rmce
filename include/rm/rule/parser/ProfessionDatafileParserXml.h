#pragma once

#include <DatafileParserXml.h>
#include <GameRuleDataCache.h>
#include <ProfessionData.h>

namespace rm {

	/**
	 * @class ProfessionDatafileParserXml
	 * @brief Process ProfessionData objects for xml files
	 *
	 * This class can read a well-formed xml file creating ProfessionData objects from the contents, adding them to a
	 * GameRuleDataCache cache.
	 *
	 * @see ProfessionData
	 * @see GameRuleDataCache
	 */

	class ProfessionDatafileParserXml : public DatafileParserXml {
	public:
		// We need this to prevent name hiding as we have a method with the same name defined in this class
		using DatafileParserBoost::save;

		/**
		 * @brief Constructor
		 * @param cache Cache to use for ProfessionData objects
		 * @param filename Path to the datafile to parse
		 */
		ProfessionDatafileParserXml(GameRuleDataCache& cache, std::string_view filename) : DatafileParserXml(cache, "Profession", filename) {
			setRootNode("ProfessionData.professions");
		}

		/**
		 * @brief Constructor
		 * @param cache Cache to use for ProfessionData objects
		 */
		ProfessionDatafileParserXml(GameRuleDataCache& cache) : ProfessionDatafileParserXml(cache, "") {}

	private:
		/**
		 * @brief Parse a ptree into ProfessionData objects
		 *
		 * Parse a boost::ptree containing the book rule datas, convert to ProfessionData objects and store in a data cache
		 */
		void parse() override;
	};

} // namespace rm