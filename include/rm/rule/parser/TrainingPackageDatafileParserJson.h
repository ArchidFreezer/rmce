#pragma once

#include <DatafileParserJson.h>
#include <GameRuleDataCache.h>
#include <TrainingPackageData.h>

namespace rm::rule::parser {

	/**
	 * @class TrainingPackageDatafileParserJson
	 * @brief Process TrainingPackageData objects for json files
	 *
	 * This class can read a well-formed json file creating TrainingPackageData objects from the contents, adding them to a
	 * GameRuleDataCache cache.
	 *
	 * The reverse operation is also available where the objects from the cache may be written out to a json file.
	 *
	 * @see TrainingPackageData
	 * @see GameRuleDataCache
	 */

	class TrainingPackageDatafileParserJson : public DatafileParserJson {
	public:
		// We need this to prevent name hiding as we have a method with the same name defined in this class
	//	using DatafileParserBoost::save;

		/**
		 * @brief Constructor
		 * @param cache Cache to use for TrainingPackageData objects
		 * @param filename Path to the datafile to parse
		 */
		TrainingPackageDatafileParserJson(GameRuleDataCache& cache, std::string_view filename) : DatafileParserJson(cache, "Trainingpackage", filename) {
			setRootNode("training-packages");
		}

		/**
		 * @brief Constructor
		 * @param cache Cache to use for TrainingPackageData objects
		 */
		TrainingPackageDatafileParserJson(GameRuleDataCache& cache) : TrainingPackageDatafileParserJson(cache, "") {}

		/**
		 * @brief Write trainingpackage game rule data from the cache to a json file
		 * @param filename Path to the json file to write
		 */
		void save(const std::string& filename) override { saveData<TrainingPackageData>(filename); }

		/**
		 * @brief Populate the given boost tree with the data from a trainingpackage
		 * @param id Id of the trainingpackage to populate from
		 * @param pdatum boost tree to populate
		 */
		void populateDatum(std::string& id, pt::ptree& pdatum) override;

	private:
		/**
		 * @brief Parse a ptree into TrainingPackageData objects
		 *
		 * Parse a boost::ptree containing the trainingpackage rule data, convert to TrainingPackageData objects and store in a data cache
		 */
		void parse() override;

		/**
		 * @brief Parse a boost ptree containing a map of qualifiers and reductions into a std::map of the same
		 *
		 * The boost ptree expected by this function should be derived from the following json format:
		 * @code{.json}
		 * "qualifiers": [
		 *   {
		 *     "qualifier": "description of the qualifier",
		 *     "reduction": reduction in package cost if the qualifier is met
		 *   }
		 * ]
		 * @endcode
		 *
		 * @param qualifiers Boost ptree containing the map of qualifiers and reductions
		 * @return Map of qualifiers and reductions
		 */
		std::map<std::string, int> parseQualifiers(boost::optional<const pt::ptree&> qualifiers);

		/**
		 * @brief Parse a std::map of qualifiers and reductions into a boost ptree
		 *
		 * The boost ptree created by this function will generate the following json format:
		 * @code{.json}
		 * "qualifiers": [
		 *   {
		 *     "qualifier": "description of the qualifier",
		 *     "reduction": reduction in package cost if the qualifier is met
		 *   }
		 * ]
		 * @endcode
		 *
		 * @param map Map of qualifiers and reductions
		 * @return Boost ptree containing the map of qualifiers and reductions
		 */
		const pt::ptree getQualifiersTree(TrainingPackageData& game_data);

		/**
		 * @brief Parses special entries from a property tree into a vector of string-integer pairs.
		 * @param specials An optional reference to a property tree containing the special entries to parse. If not present, an empty vector is returned.
		 * @return A vector of pairs, where each pair contains a string key and an integer value representing the parsed special entries.
		 */
		std::vector<std::pair<std::string, int>> parseSpecials(boost::optional<const pt::ptree&> specials);

		/**
		 * @brief Converts a vector of string-integer pairs representing special entries into a property tree.
		 * @param specials A vector of pairs, where each pair contains a string key and an integer value representing the special entries to convert.
		 * @return A property tree representation of the special entries, where each entry is represented as a child node with the key and value.
		 */
		const pt::ptree getSpecialsTree(TrainingPackageData& game_data);

		/**
		 * @brief Parses stat gain choices from a property tree into a vector of string-integer pairs.
		 * @param stat_gain_choices An optional reference to a property tree containing the stat gain choices to parse. If not present, an empty vector is returned.
		 * @return A vector of pairs, where each pair contains a string key and an integer value representing the parsed stat gain choices.
		 */
		EnumChoice<rule::enums::StatType::Type> parseStatGainChoices(boost::optional<const pt::ptree&> stat_gain_choices);

		/**
		 * @brief Converts a vector of string-integer pairs representing stat gain choices into a property tree.
		 * @param stat_gain_choices A vector of pairs, where each pair contains a string key and an integer value representing the stat gain choices to convert.
		 * @return A property tree representation of the stat gain choices, where each entry is represented as a child node with the key and value.
		 */
		const pt::ptree getStatGainChoicesTree(TrainingPackageData& game_data);

		/**
		 * @brief Parses category multi skill rank choices from a property tree into a set of CategoryMultiSkillRankChoice objects.
		 * @param category_multi_skill_rank_choices An optional reference to a property tree containing the category multi skill rank choices to parse. If not present, an empty set is returned.
		 * @return A set of CategoryMultiSkillRankChoice objects representing the parsed category multi skill rank choices.
		 */
		std::set<CategoryMultiSkillRankChoice> parseCategoryMultiSkillRankChoices(boost::optional<const pt::ptree&> category_multi_skill_rank_choices);

		/**
		 * @brief Converts a set of CategoryMultiSkillRankChoice objects representing category multi skill rank choices into a property tree.
		 * @param category_multi_skill_rank_choices A set of CategoryMultiSkillRankChoice objects representing the category multi skill rank choices to convert.
		 * @return A property tree representation of the category multi skill rank choices, where each entry is represented as a child node with the relevant data.
		 */
		const pt::ptree getCategoryMultiSkillRankChoicesTree(TrainingPackageData& game_data);

		/**
		 * @brief Parses group multi skill rank choices from a property tree into a set of GroupMultiSkillRankChoice objects.
		 * @param group_multi_skill_rank_choices An optional reference to a property tree containing the group multi skill rank choices to parse. If not present, an empty set is returned.
		 * @return A vector of GroupMultiSkillRankChoice objects representing the parsed group multi skill rank choices.
		 */
		std::vector<GroupMultiSkillRankChoice> parseGroupMultiSkillRankChoices(boost::optional<const pt::ptree&> group_multi_skill_rank_choices);

		/**
		 * @brief Converts a set of GroupMultiSkillRankChoice objects representing group multi skill rank choices into a property tree.
		 * @param group_multi_skill_rank_choices A set of GroupMultiSkillRankChoice objects representing the group multi skill rank choices to convert.
		 * @return A property tree representation of the group multi skill rank choices, where each entry is represented as a child node with the relevant data.
		 */
		const pt::ptree getGroupMultiSkillRankChoicesTree(TrainingPackageData& game_data);

		/**
		 * @brief Parses spell list choices from a property tree and returns them as a set.
		 * @param spell_list_choices An optional reference to a property tree containing spell list choice data to parse. If empty, an empty set is returned.
		 * @return A set of SpellListChoices objects parsed from the input property tree.
		 */
		std::set<SpellListChoices> parseSpellListChoicesTree(boost::optional<const pt::ptree&> spell_list_choices);

		/**
		 * @brief Converts a set of SpellListChoices objects representing spell list choices into a property tree.
		 * @param spell_list_choices A set of SpellListChoices objects representing the spell list choices to convert.
		 * @return A property tree representation of the spell list choices, where each entry is represented as a child node with the relevant data.
		 */
		const pt::ptree getSpellListChoicesTree(TrainingPackageData& game_data);

		/**
		 * @brief Parses spell list category choices from a property tree and returns them as a set.
		 * @param spell_list_category_choices An optional reference to a property tree containing spell list category choice data to parse. If empty, an empty set is returned.
		 * @return A set of SpellListCategoryChoices objects parsed from the input property tree.
		 */
		std::set<SpellListCategoryChoices> parseSpellListCategoryChoicesTree(boost::optional<const pt::ptree&> spell_list_category_choices);

		/**
		 * @brief Converts a set of SpellListCategoryChoices objects representing spell list category choices into a property tree.
		 * @param spell_list_category_choices A set of SpellListCategoryChoices objects representing the spell list category choices to convert.
		 * @return A property tree representation of the spell list category choices, where each entry is represented as a child node with the relevant data.
		 */
		const pt::ptree getSpellListCategoryChoicesTree(TrainingPackageData& game_data);

		/**
		 * @brief Parses skill group category and skill rank choices from a property tree and returns them as a vector.
		 * @param skill_group_category_and_skill_rank_choices An optional reference to a property tree containing skill group category and skill rank choice data to parse. If empty, an empty vector is returned.
		 * @return A vector of SkillGroupCategoryAndSkillRankChoice objects parsed from the input property tree.
		 */
		std::vector< SkillGroupCategoryAndSkillRankChoice> parseSkillGroupCategoryAndSkillRankChoicesTree(boost::optional<const pt::ptree&> skill_group_category_and_skill_rank_choices);

		/**
		 * @brief Converts a vector of SkillGroupCategoryAndSkillRankChoice objects representing skill group category and skill rank choices into a property tree.
		 * @param skill_group_category_and_skill_rank_choices A vector of SkillGroupCategoryAndSkillRankChoice objects representing the skill group category and skill rank choices to convert.
		 * @return A property tree representation of the skill group category and skill rank choices, where each entry is represented as a child node with the relevant data.
		 */
		const pt::ptree getSkillGroupCategoryAndSkillRankChoicesTree(TrainingPackageData& game_data);
	};

} // namespace rm::rule::parser