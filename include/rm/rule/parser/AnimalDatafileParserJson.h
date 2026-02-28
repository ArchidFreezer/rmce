#pragma once

#include <DatafileParserJson.h>
#include <AnimalData.h>

namespace rm::rule::parser {

	/**
	 * @class AnimalDatafileParserJson
	 * @brief Process AnimalData objects for json files
	 *
	 * This class can read a well-formed json file creating AnimalData objects from the contents, adding them to a cache.
	 *
	 * The reverse operation is also available where the objects from the cache may be written out to a json file.
	 *
	 * @see AnimalData
	 */
	class AnimalDatafileParserJson : public DatafileParserJson {
	public:
		// We need this to prevent name hiding as we have a method with the same name defined in this class
		using DatafileParserBoost::save;

		/**
		 * @brief Constructor
		 * @param object_manager Reference to an object manager to handle the data objects
		 * @param filename Path to the datafile to parse
		 */
		AnimalDatafileParserJson(rm::GameRuleDataFactory& object_manager, std::string_view filename) : DatafileParserJson(object_manager, "Animal", filename) {
			setRootNode("animals");
		}

		/**
		 * @brief Constructor
		 * @param object_manager Reference to an object manager to handle the data objects
		 */
		AnimalDatafileParserJson(rm::GameRuleDataFactory& object_manager) : AnimalDatafileParserJson(object_manager, "") {}

		/**
		 * @brief Write animal game rule data from the cache to a json file
		 * @param filename Path to the json file to write
		 */
		void save(const std::string& filename) override { saveData<AnimalData>(filename); }

		/**
		 * @brief Populate the given boost tree with the data from a animal
		 * @param id Id of the animal to populate from
		 * @param pdatum boost tree to populate
		 */
		void populateDatum(std::string& id, pt::ptree& pdatum) override;

	private:
		/**
		 * @brief Parse a ptree into AnimalData objects
		 *
		 * Parse a boost::ptree containing the animal rule datas, convert to AnimalData objects and store in a data cache
		 */
		void parse() override;

		/**
		 * @brief Build the lookup table that maps the bonus XP code to a bonus XP value
		 *
		 * This is used to determine how much bonus XP is awarded for killing an animal based on its bonus XP code.
		 */
		void buildCreatureBonusXpTable();

		/**
		 * @brief Build the lookup table that maps the level variance code to a difference in the level of the animal from the average
		 */

		void buildLevelVarianceTable();

		/**
		 * @brief Parse an AnimalAttack object and write into a boost ptree
		 *
		 * Parse a boost::ptree containing the data for an animal attack and populate an AnimalAttack object with that data
		 *
		 * @param tree Boost ptree to populate with the data from the object
		 * @param ref Reference to the AnimalAttack object containing the data for the animal attack
		 */
		void populateAnimalAttack(pt::ptree& tree, const rm::game::AnimalAttack& attack);

		/**
		 * @brief Parse a ptree into an AnimalAttack object
		 *
		 * Parse a boost::ptree containing the data for an animal attack and populate an AnimalAttack object with that data
		 *
		 * @param tree Boost ptree containing the data for the animal attack
		 * @param ref Reference to the AnimalAttack object to populate with the data from the boost ptree
		 * @param parse_chance Whether to parse the chance value for the attack, this is used to allow parsing of attacks that are used in a context where the chance value is not relevant such as conditional attacks.
		 */
		void parseAnimalAttack(rm::game::AnimalAttack& attack, const pt::ptree& tree, bool parse_chance = true);

	};

} // namespace rm::rule::parser