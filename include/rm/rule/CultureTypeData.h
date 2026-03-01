#pragma once

#include <stdexcept>
#include <ranges>
#include <set>
#include <string_view>
#include <ArmourType.h>
#include <ClimateData.h>
#include <EnvironmentType.h>
#include <GameRuleData.h>
#include <GameRuleDataChoice.h>
#include <SubcategoriedSkillData.h>
#include <SkillCategoryData.h>
#include <WeaponTypeData.h>

using namespace rm::rule::enums;

namespace rm::rule {

	/**
	 * @class CultureTypeData
	 * @brief Definition of what comprises the primary adolescent influences on a character based on location and culture
	 *
	 * Some aspects of a character, such as the physical appearance and biological traits, are tied to the characters race, but
	 * many elements are more associated with how and where they grew up. A dwarf living in a metropolitan city has as much in
	 * common with an elf brought up in the city as they do with a dwarf in an underground cavern complex.
	 *
	 * These cultural influences manifest themselves as religious practices or instinctive biases for or against other races or
	 * cultures and what special skills they are likely to pick up during their formative years, represented in gameplay by
	 * adolescent and hobby skill ranks.
	 *
	 * Within each culture type there are specifc sub cultures, such as lower class or militant versions that further refine
	 * these. The sub culture tends to define hobby preferences or languages and these are defined in the CultureData class
	 * which needs to be used in conjunction with this class when creating a character.
	 *
	 * The character creation process would consist of first deciding the culture type and then the actual culture to apply.
	 *
	 * @see CultureData
	 */
	class CultureTypeData : public GameRuleData {
	public:

		/**
		 * @brief Thrown when there is an attempt to set an invalid skill rank
		 */
		class InvalidSkillRank : public std::runtime_error {
		public:
			/**
			 * @brief Exception constructor
			 *
			 * std::runtime_error will handle the string
			 *
			 * @param error String to display for the error
			 */
			InvalidSkillRank(const std::string& error) : std::runtime_error{ error } {}
		};

		/**
		 * @brief Delete the default constructor to ensure we initialise the base class
		 */
		CultureTypeData() = delete;

		/**
		 * @brief Basic constructor
		 * @param id Unique identifier of the culture type
		 */
		CultureTypeData(std::string_view id) : GameRuleData(id) {}

		/**
		* @brief Set the name of the race
		* @param name Race name
		*/
		void setName(std::string_view name) { name_ = name; }

		/**
		 * @brief Get the name of the race
		 * @return Race name as a string reference
		 */
		const std::string& name() const { return name_; }

		/**
		 * @brief Set the description of the race
		 * @param description std::string_view race description
		 */
		void setDescription(std::string_view description) { description_ = description; }

		/**
		 * @brief Get the description of the race
		 * @return std::string reference of the description
		 */
		const std::string& description() const { return description_; }

		/**
		 * @brief Set a description of some concepts for characters form this culture
		 * @param concepts character concept description
		 */
		void setCharacterConcept(std::string_view concepts) { character_concepts_ = concepts; }

		/**
		 * @brief Get a description of some concepts for characters form this culture
		 * @return Concepts for characters
		 */
		const std::string& characterConcept() const { return character_concepts_; }

		/**
		 * @brief Set the typical clothing worn by members of the culture
		 * @param clothing clothing description
		 */
		void setClothing(std::string_view clothing) { clothing_ = clothing; }

		/**
		 * @brief Get the typical clothing worn by members of the culture
		 * @return clothing description
		 */
		const std::string& clothing() const { return clothing_; }

		/**
		 * @brief Set the typical aspirations for members of this culture
		 * @param aspirations aspirations description
		 */
		void setAspirations(std::string_view aspirations) { aspirations_ = aspirations; }

		/**
		 * @brief Get the typical aspirations for members of this culture
		 * @return aspirations description
		 */
		const std::string& aspirations() const { return aspirations_; }

		/**
		 * @brief Set the collective fears of members of this culture
		 * @param fears collective fears
		 */
		void setFears(std::string_view fears) { fears_ = fears; }

		/**
		 * @brief Get the collective fears of members of this culture
		 * @return collective fears
		 */
		const std::string& fears() const { return fears_; }

		/**
		 * @brief Set how members of the culture typically view marriage
		 * @param marriage_patterns typical marriage patterns
		 */
		void setMarriagePatterns(std::string_view marriage_patterns) { marriage_patterns_ = marriage_patterns; }

		/**
		 * @brief Get how members of the culture typically view marriage
		 * @return typical marriage patterns
		 */
		const std::string& marriagePatterns() const { return marriage_patterns_; }

		/**
		 * @brief Set prejudices that are typical to members of the culture
		 * @param prejudices typical cultural prejudices
		 */
		void setPrejudices(std::string_view prejudices) { prejudices_ = prejudices; }

		/**
		 * @brief Get prejudices that are typical to members of the culture
		 * @return typical cultural prejudices
		 */
		const std::string& prejudices() const { return prejudices_; }

		/**
		 * @brief Set religious beliefs practiced by members of the culture
		 * @param religious_beliefs religious beliefs
		 */
		void setReligiousBeliefs(std::string_view religious_beliefs) { religion_ = religious_beliefs; }

		/**
		 * @brief Get religious beliefs practiced by members of the culture
		 * @return religious beliefs
		 */
		const std::string& religiousBeliefs() const { return religion_; }

		/**
		 * @brief Set the number of skill ranks that the character has to spend on hobbies during their adolescence
		 * @param hobby_skill_ranks number of skill ranks
		 */
		void setHobbySkillRanks(int hobby_skill_ranks) { hobby_skill_ranks_ = hobby_skill_ranks; }

		/**
		 * @brief Get the number of skill ranks that the character has to spend on hobbies during their adolescence
		 * @return number of skill ranks
		 */
		int hobbySkillRanks() const { return hobby_skill_ranks_; }

		/**
		 * @brief Set the number of ranks that the character receives on one open spell list of their realm
		 * @param spell_list_ranks number of spell list ranks
		 */
		void setSpellListRanks(int spell_list_ranks) { spell_list_ranks_ = spell_list_ranks; }

		/**
		 * @brief Get the number of skill ranks that the character receives on one open spell list of their realm
		 * @return number of spell list ranks
		 */
		int spellListRanks() const { return spell_list_ranks_; }

		/**
		 * @brief Add an armour type to the set of those preferred by the culture
		 * @param armour_type ArmourType::Type to add
		 */
		void addPreferredArmour(ArmourType::Type armour_type) { preferred_armour_.emplace(armour_type); }

		/**
		 * @brief Set the set of armour types preferred by the culture
		 * @param armours std::set of ArmourType::Type to set as preferred armours
		 */
		void setPreferredArmours(std::set<ArmourType::Type> armours) { preferred_armour_ = std::move(armours); }

		/**
		 * @brief Get a container with the armour types preferred by the culture
		 * @return std::set<ArmourType::Type> armour types
		 */
		const std::set<ArmourType::Type> preferredArmour() const { return preferred_armour_; }

		/**
		 * @brief Get whether an armour type is amongst those prefereed by the culture
		 * @param armour_type ArmourType::Type to check
		 * @return `true` if the armour type is preferred by the culture
		 * @return `true` if the armour type is not preferred by the culture
		 */
		bool isPreferredArmour(ArmourType::Type armour_type) const { return (preferred_armour_.find(armour_type) != preferred_armour_.end()); }

		/**
		 * @brief Add an weapon type to the set of those preferred by the culture
		 * @param weapon WeaponTypeData to add
		 */
		void addPreferredWeapon(WeaponTypeData& weapon) { preferred_weapons_.emplace(&weapon); }

		/**
		 * @brief Get a container with the weapons preferred by the culture
		 * @return std::set<WeaponTypeData> weapons
		 */
		const std::set<const WeaponTypeData*> preferredWeapons() const { return preferred_weapons_; }

		/**
		 * @brief Set the set of weapon types preferred by the culture
		 * @param weapons std::set of WeaponTypeData pointers to set as preferred weapons
		 */
		void setPreferredWeapons(std::set<const WeaponTypeData*> weapons) { preferred_weapons_ = std::move(weapons); }

		/**
		 * @brief Get whether an weapon type is amongst those prefereed by the culture
		 * @param weapon WeaponTypeData to check
		 * @return `true` if the weapon type is preferred by the culture
		 * @return `true` if the weapon type is not preferred by the culture
		 */
		bool isPreferredWeapon(WeaponTypeData& weapon) const {
			for (auto& key : preferred_weapons_) {
				if (weapon.id() == key->id()) return true;
			}
			return false;
		}

		/**
		 * @brief Set the number of ranks for a skill the culture provides during adolescence
		 * @param skill SubcategoriedSkillData to add the ranks to
		 * @param ranks int number of ranks
		 */
		void addSkillRank(SubcategoriedSkillData& skill, int ranks) {
			if (isRankSkill(skill.skillData(), skill.subcategory())) throw InvalidSkillRank("There is already a rank set for skill " + skill.id());
			skill_ranks_.emplace(&skill, ranks);
		}

		/**
		 * @brief Set the map of skills and ranks the culture provides during adolescence
		 * @param map Map of SubcategoriedSkillData and int number of ranks for each skill to set
		 */
		void setSkillRanks(std::map<const SubcategoriedSkillData*, int> map) {
			for (auto& key : std::views::keys(map)) {
				if (isRankSkill(key->skillData(), key->subcategory())) throw InvalidSkillRank("There is already a rank set for skill " + key->id());
			}
			skill_ranks_ = std::move(map);
		}

		/**
		 * @brief Get the map of skills and ranks the culture provides during adolescence
		 * @return Map of SubcategoriedSkillData and int number of ranks for each skill
		 */
		const std::map<const SubcategoriedSkillData*, int>& skillRanks() const { return skill_ranks_; }

		/**
		 * @brief Get the number of ranks for a skill the culture provides during adolescence
		 * @param skill SubcategoriedSkillData to get the ranks for
		 * @return number of ranks
		 */
		int skillRank(const SubcategoriedSkillData& skill) const {
			for (auto& key : skill_ranks_) {
				if (key.first->id() == skill.id()) return key.second;
			}
			return 0;
		}

		/**
		 * @brief Get the number of ranks for a skill the culture provides during adolescence
		 * @param skill SkillData to get the ranks for
		 * @param subcategory optional subcategory of @a skill
		 * @return number of ranks
		 */
		int skillRank(const SkillData& skill, std::optional<std::string_view> subcategory = std::nullopt) const {
			return skillRank(SubcategoriedSkillData(skill, subcategory));
		}

		/**
		 * @brief Gets a container with the skills that the culture provides adolescent ranks for
		 * @return std::set of SkillData with adolescent ranks
		 */
		const std::set<const SubcategoriedSkillData*> skillsWithRanks() const {
			auto keys = std::views::keys(skill_ranks_);
			return { keys.begin(), keys.end() };
		}

		/**
		 * @brief Check if the culture provides adolescent skill ranks for the skill
		 * @param skill SkillData to check
		 * @param subcategory optional subcategory of @a skill
		 * @return `true` if the culture provides adolescent ranks
		 * @return `false` if the culture does not provide adolescent ranks
		 */
		bool isRankSkill(const SkillData& skill, std::optional<std::string_view> subcategory = std::nullopt) const {
			for (auto& key : std::views::keys(skill_ranks_)) {
				if (key->skillData().id() == skill.id() && (subcategory ? subcategory.value() == key->subcategory().value() : !key->subcategory())) return true;
			}
			return false;
		}

		/**
		 * @brief Add number of skill ranks a cetegory receives during adolescence
		 * @param category SkillCategoryData to add ranks for
		 * @param ranks int adolescent ranks
		 */
		void addSkillCategoryRank(const SkillCategoryData& category, int ranks) { skill_category_ranks_.emplace(&category, ranks); }

		/**
		 * @brief Set the number of skill ranks a category receives during adolescence
		 * @param map Map of SkillCategoryData and int number of adolescent ranks to set for each category
		 */
		void setSkillCategoryRanks(std::map<const SkillCategoryData*, int> map) { skill_category_ranks_ = std::move(map); }

		/**
		 * @brief Get the number of skill ranks a category receives during adolescence
		 * @return Map of SkillCategoryData and int number of adolescent ranks for each category
		 */
		const std::map<const SkillCategoryData*, int>& skillCategoryRanks() const { return skill_category_ranks_; }

		/**
		 * @brief Get a container of all the skill categories with a adolescent ranks
		 * @return std::set of categories with adolescent ranks
		 */
		const std::set<const SkillCategoryData*> skillCategoriesWithRanks() const {
			auto keys = std::views::keys(skill_category_ranks_);
			return { keys.begin(), keys.end() };
		}

		/**
		 * @brief Check if the culture provides adolescent skill ranks for the category
		 * @param category SkillCategoryData to check
		 * @return `true` if the category has adolescent ranks
		 * @return `false` if the category does not have adolescent ranks
		 */
		bool isRankCategory(const SkillCategoryData& category) const {
			for (auto& cat : std::views::keys(skill_category_ranks_)) {
				if (cat->id() == category.id()) return true;
			}
			return false;
		}

		/**
		 * @brief Get the number of ranks a category receives during adolescence
		 * @param category SkillCategoryData to check
		 * @return int number of ranks @a category receives
		 */
		int skillCategoryRank(const SkillCategoryData& category) {
			for (auto& cat : std::views::keys(skill_category_ranks_)) {
				if (cat->id() == category.id()) return skill_category_ranks_.at(cat);
			}
			return 0;
		}

		/**
		 * @brief Add number of skill ranks a skill in the category receives during adolescence
		 * @param category SkillCategoryData to add a bonus for
		 * @param ranks int number of adolescent ranks to add to a skill in the category
		 */
		void addSkillCategorySkillRank(const SkillCategoryData& category, int ranks) { skill_category_skill_ranks_.emplace(&category, ranks); }

		/**
		 * @brief Set the number of skill ranks a skill in the category receives during adolescence
		 * @param map Map of SkillCategoryData and int number of adolescent ranks to add to a skill in the category to set for each category
		 */
		void setSkillCategorySkillRanks(std::map<const SkillCategoryData*, int> map) { skill_category_skill_ranks_ = std::move(map); }

		/**
		 * @brief Get the number of skill ranks a skill in the category receives during adolescence
		 * @return Map of SkillCategoryData and int number of adolescent ranks to add to a skill in the category for each category
		 */
		const std::map<const SkillCategoryData*, int>& skillCategorySkillRanks() const { return skill_category_skill_ranks_; }

		/**
		 * @brief Get a container of all the skill categories with a adolescent ranks to add to a skill
		 * @return std::set of categories with adolescent ranks to be added to a skill
		 */
		const std::set<const SkillCategoryData*> skillCategoriesWithSkillRanks() const {
			auto keys = std::views::keys(skill_category_skill_ranks_);
			return { keys.begin(), keys.end() };
		}

		/**
		 * @brief Check if the culture provides adolescent skill ranks to a skill in the category
		 * @param category SkillCategoryData to check
		 * @return `true` if the category has adolescent ranks applied to a skill
		 * @return `false` if the category does not have adolescent ranks applied to a skill
		 */
		bool isSkillRankCategory(const SkillCategoryData& category) const {
			for (auto& cat : std::views::keys(skill_category_skill_ranks_)) {
				if (cat->id() == category.id()) return true;
			}
			return false;
		}

		/**
		 * @brief Get the number of ranks a skill in the category receives during adolescence
		 * @param category SkillCategoryData to check
		 * @return int number of ranks a skill in @a category receives
		 */
		int skillCategorySkillRank(const SkillCategoryData& category) {
			for (auto& cat : std::views::keys(skill_category_skill_ranks_)) {
				if (cat->id() == category.id()) return skill_category_skill_ranks_.at(cat);
			}
			return 0;
		}

		/**
		 * @brief Add an climate type to the set of those required by the culture
		 * @param climate ClimateData to add
		 */
		void addRequiredClimate(ClimateData& climate) { required_climates_.emplace(&climate); }

		/**
		 * @brief Set the set of climate types required by the culture
		 * @param climates std::set of ClimateData pointers to set as required climates
		 */
		void setRequiredClimates(std::set<const ClimateData*> climates) { required_climates_ = std::move(climates); }

		/**
		 * @brief Get a container with the climates required by the culture
		 * @return std::set<ClimateData> climates
		 */
		const std::set<const ClimateData*> requiredClimates() const { return required_climates_; }

		/**
		 * @brief Get whether an climate type is amongst those required by the culture
		 * @param climate ClimateData to check
		 * @return `true` if the climate type is required by the culture
		 * @return `true` if the climate type is not required by the culture
		 */
		bool isRequiredClimate(ClimateData& climate) const {
			for (auto& key : required_climates_) {
				if (climate.id() == key->id()) return true;
			}
			return false;
		}

		/**
		 * @brief Add an environment feature to the set of those required by the culture
		 * @param feature EnvironmentType::Feature to add
		 */
		void addRequiredFeature(EnvironmentType::Feature feature) { required_features_.emplace(feature); }

		/**
		 * @brief Set the set of environment features required by the culture
		 * @param features std::set of EnvironmentType::Feature to set as required features
		 */
		void setRequiredFeatures(std::set<EnvironmentType::Feature> features) { required_features_ = std::move(features); }

		/**
		 * @brief Get a container with the environment features required by the culture
		 * @return std::set<EnvironmentType::Feature> environment features
		 */
		const std::set<EnvironmentType::Feature> requiredFeatures() const { return required_features_; }

		/**
		 * @brief Get whether an environment feature type is amongst those required by the culture
		 * @param feature EnvironmentType::Feature to check
		 * @return `true` if the environment feature is required by the culture
		 * @return `true` if the environment feature is not required by the culture
		 */
		bool isRequiredFeature(EnvironmentType::Feature feature) const { return (required_features_.find(feature) != required_features_.end()); }


		/**
		 * @brief Add an environment terrain to the set of those required by the culture
		 * @param terrain EnvironmentType::Terrain to add
		 */
		void addRequiredTerrain(EnvironmentType::Terrain terrain) { required_terrains_.emplace(terrain); }

		/**
		 * @brief Set the set of environment terrains required by the culture
		 * @param terrains std::set of EnvironmentType::Terrain to set as required terrains
		 */
		void setRequiredTerrains(std::set<EnvironmentType::Terrain> terrains) { required_terrains_ = std::move(terrains); }

		/**
		 * @brief Get a container with the environment terrains required by the culture
		 * @return std::set<EnvironmentType::Terrain> environment terrains
		 */
		const std::set<EnvironmentType::Terrain> requiredTerrains() const { return required_terrains_; }

		/**
		 * @brief Get whether an environment terrain type is amongst those required by the culture
		 * @param terrain EnvironmentType::Terrain to check
		 * @return `true` if the environment terrain is required by the culture
		 * @return `true` if the environment terrain is not required by the culture
		 */
		bool isRequiredTerrain(EnvironmentType::Terrain terrain) const { return (required_terrains_.find(terrain) != required_terrains_.end()); }


		/**
		 * @brief Add an environment vegetation to the set of those required by the culture
		 * @param vegetation EnvironmentType::Vegetation to add
		 */
		void addRequiredVegetation(EnvironmentType::Vegetation vegetation) { required_vegetations_.emplace(vegetation); }

		/**
		 * @brief Set the set of environment vegetations required by the culture
		 * @param vegetations std::set of EnvironmentType::Vegetation to set as required vegetations
		 */
		void setRequiredVegetations(std::set<EnvironmentType::Vegetation> vegetations) { required_vegetations_ = std::move(vegetations); }

		/**
		 * @brief Get a container with the environment vegetations required by the culture
		 * @return std::set<EnvironmentType::Vegetation> environment vegetations
		 */
		const std::set<EnvironmentType::Vegetation> requiredVegetations() const { return required_vegetations_; }

		/**
		 * @brief Get whether an environment vegetation type is amongst those required by the culture
		 * @param vegetation EnvironmentType::Vegetation to check
		 * @return `true` if the environment vegetation is required by the culture
		 * @return `true` if the environment vegetation is not required by the culture
		 */
		bool isRequiredVegetation(EnvironmentType::Vegetation vegetation) const { return (required_vegetations_.find(vegetation) != required_vegetations_.end()); }


		/**
		 * @brief Add an environment water source to the set of those required by the culture
		 * @param water EnvironmentType::Water source to add
		 */
		void addRequiredWaterSource(EnvironmentType::Water water) { required_water_sources_.emplace(water); }

		/**
		 * @brief Set the set of environment water sources required by the culture
		 * @param water_sources std::set of EnvironmentType::Water to set as required water sources
		 */
		void setRequiredWaterSources(std::set<EnvironmentType::Water> water_sources) { required_water_sources_ = std::move(water_sources); }

		/**
		 * @brief Get a container with the environment water sources required by the culture
		 * @return std::set<EnvironmentType::Water> environment water sources
		 */
		const std::set<EnvironmentType::Water> requiredWaterSources() const { return required_water_sources_; }

		/**
		 * @brief Get whether an environment water source is amongst those required by the culture
		 * @param water EnvironmentType::Water source to check
		 * @return `true` if the environment water source is required by the culture
		 * @return `true` if the environment water source is not required by the culture
		 */
		bool isRequiredWaterSource(EnvironmentType::Water water) const { return (required_water_sources_.find(water) != required_water_sources_.end()); }

	private:
		std::string name_{}; /**< Name of the culture type */
		std::string description_{}; /**< General description of the culture type */
		std::string character_concepts_{}; /**< Idea of a character from the cultures outlook */
		std::string clothing_{}; /**< Typical clothing of a member of the culture */
		std::string aspirations_{}; /**< Aspirations of typical members of the culture */
		std::string fears_{}; /** Collective fears of members of the culture */
		std::string marriage_patterns_{}; /** How members of teh culture typically view marriage */
		std::string prejudices_{}; /** Any prejudices that are typical to members of the culture */
		std::string religion_{}; /** Typical religious beliefs practiced by members of the culture */
		int hobby_skill_ranks_{}; /**< Number of hobby skill ranks available during adolescence */
		int spell_list_ranks_{}; /**< Number of ranks in an open spell list members of the culture receive */
		std::set<ArmourType::Type> preferred_armour_{}; /**< Armour type typically preferred by members of the culture */
		std::set<const WeaponTypeData*> preferred_weapons_{}; /**< Weapon type typically preferred by members of the culture */
		std::map<const SubcategoriedSkillData*, int> skill_ranks_{}; /** Number of skill ranks gained during adolescence */
		std::map<const SkillCategoryData*, int> skill_category_ranks_{}; /** Number of skill category ranks gained during adolescence */
		std::map<const SkillCategoryData*, int> skill_category_skill_ranks_{}; /** Number of skill ranks from a category gained during adolescence */
		std::set<const ClimateData*> required_climates_{}; /**< Set of climates, one of which the culture will live in  */
		std::set<EnvironmentType::Feature> required_features_{}; /**< Set of environment features, one of which the culture will live in  */
		std::set<EnvironmentType::Terrain> required_terrains_{}; /**< Set of environment terrains, one of which the culture will live in  */
		std::set<EnvironmentType::Vegetation> required_vegetations_{}; /**< Set of environment vegetations, one of which the culture will live in  */
		std::set<EnvironmentType::Water> required_water_sources_{}; /**< Set of environment water sources, one of which the culture will live on or next to */
	};

} // namespace rm::rule