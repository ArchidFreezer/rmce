#pragma once

#include <CultureTypeData.h>
#include <GameRuleData.h>
#include <LanguageAbility.h>
#include <ProfessionData.h>
#include <TrainingPackageData.h>

using rm::game::character::LanguageAbility;

namespace rm::rule {

	/**
	 * @class CultureData
	 * @brief A specific culture that a character may belong to.
	 *
	 * Culture definitions are split into two classes:
	 * The first is the overall culture type that defines may of the general details such as cultural beliefs and fears,
	 * preferred weapons and armour plus the basic skills that are learned during adolescence by all members of the culture.
	 *
	 * The second is the sub culture that the character belongs to which defines the hobby skills and languages, plus any
	 * preferred professions or typical training packages, etc. These sub cultures are there to add additional flavour
	 * and details. An example would be the Plains culture where a sub-cultures would define the difference between
	 * a nomadic culture and a settled culture.
	 *
	 * @see CultureTypeData
	 */
	class CultureData : public GameRuleData {
	public:
		/**
		 * @brief Delete defaulkt constructure to ensure initialisation of base class
		 */
		CultureData() = delete;

		/**
		 * @brief Basic constructor
		 * @param id Unique identifier of the culture type
		 */
		CultureData(std::string_view id) : GameRuleData(id) {}

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
		 * @brief Set the culture type that this culture is based on
		 * @param culture_type CultureTypeData parent culture type
		 */
		void setCultureType(const CultureTypeData& culture_type) { culture_type_ = &culture_type; }

		/**
		 * @brief Get the culture type that this culture is based on
		 * @return CultureTypeData parent culture type
		 */
		const CultureTypeData& cultureType() const { return *culture_type_; }

		/**
		 * @brief Set whether the culture has a low level of technical or cultural acheivements
		 * @param high_culture bool high or barbarian level culture
		 */
		void setHighCulture(bool high_culture) { high_culture_ = high_culture; }

		/**
		 * @brief Get whether the culture has a low level of technical or cultural acheivements
		 * @return `true` for a high level culture
		 * @return `false` for a low cultural or technological level
		 */
		bool highCulture() const { return high_culture_; }

		/**
		 * @brief Add the number of ranks a member of the race gets in a language during adolescence
		 * @param language LanguageAbility containing the ranks for a language
		 */
		void addLanguageAbility(LanguageAbility language) { languages_.emplace(language.language(), language); }

		/**
		 * @brief Sets the language abilities map for this object.
		 * @param languages A map associating language identifiers with their corresponding language ability levels.
		 */
		void setLanguageAbilities(std::map<std::string, const LanguageAbility> languages) { languages_ = std::move(languages); }

		/**
		 * @brief Gets the staring ability a member of the race has in a language
		 * @param language LanguageData language to get the ability for
		 * @return LanguageAbility language ability
		 */
		const LanguageAbility& languageAbility(const LanguageData& language) const { return languageAbility(language.name()); }

		/**
		 * @brief Gets the staring ability a member of the race has in a language
		 * @param language_name name of the language to get the ability for
		 * @return LanguageAbility language ability
		 */
		const LanguageAbility& languageAbility(const std::string& language_name) const { return languages_.at(language_name); }

		/**
		 * @brief Get a container with the LanguageAbility objects known during adolescence
		 * @return std::vector of LanguageAbility object references
		 */
		const std::vector<LanguageAbility>& languages() const {
			auto values = std::views::values(languages_);
			return { values.begin(), values.end() };
		}

		/**
		 * @brief Get a container with the LanguageAbility objects known during adolescence
		 * @return std::map associating language identifiers with their corresponding language ability levels
		 */
		const std::map<std::string, const LanguageAbility>& languageAbilities() const { return languages_; }

		/**
		 * @brief Add a skill to those typically taken by an adolescent of the culture
		 * @param skill SubcategoriedSkillData to add
		 */
		void addHobbySkill(const SubcategoriedSkillData& skill) { hobby_skills_.emplace(skill); }

		/**
		 * @brief Set the skills typically taken by an adolescent of the culture
		 * @param hobby_skills std::set of SubcategoriedSkillData representing the skills typically taken by an adolescent of the culture
		 */
		void setHobbySkills(std::set<SubcategoriedSkillData> hobby_skills) { hobby_skills_ = std::move(hobby_skills); }

		/**
		 * @brief Get a container with the skills typically taken by an adolescent of the culture
		 * @return std::set<SubcategoriedSkillData> typical hobby skills
		 */
		const std::set<SubcategoriedSkillData>& hobbySkills() const { return hobby_skills_; }

		/**
		 * @brief Get whether a skill is typically taken by an adolescent of the culture
		 * @param skill SubcategoriedSkillData to check
		 * @return `true` if the skill is typically taken by an adolescent of the culture
		 * @return `true` if the skill is not typically taken by an adolescent of the culture
		 */
		bool isHobbySkill(const SubcategoriedSkillData& skill) const {
			for (const auto& key : hobby_skills_) {
				if (skill.id() == key.id()) return true;
			}
			return false;
		}

		/**
		 * @brief Add a skill category to those typically taken by an adolescent of the culture
		 * @param skill_category SkillCategoryData to add
		 */
		void addHobbySkillCategory(const SkillCategoryData& skill_category) { hobby_skill_categories_.emplace(&skill_category); }

		/**
		 * @brief Set the skill categories typically taken by an adolescent of the culture
		 * @param hobby_skill_categories std::set of SkillCategoryData pointers representing the skill categories typically taken by an adolescent of the culture
		 */
		void setHobbySkillCategories(std::set<const SkillCategoryData*> hobby_skill_categories) { hobby_skill_categories_ = std::move(hobby_skill_categories); }

		/**
		 * @brief Get a container with the skill categories typically taken by an adolescent of the culture
		 * @return std::set<SkillCategoryData> skill categories
		 */
		const std::set<const SkillCategoryData*>& hobbySkillCategories() const { return hobby_skill_categories_; }

		/**
		 * @brief Get whether a skill category is typically taken by an adolescent of the culture
		 * @param skill_category SkillCategoryData to check
		 * @return `true` if the skill category is typically taken by an adolescent of the culture
		 * @return `false` if the skill category is not typically taken by an adolescent of the culture
		 */
		bool isHobbySkillCategory(const SkillCategoryData& skill_category) const {
			for (const auto& key : hobby_skill_categories_) {
				if (skill_category.id() == key->id()) return true;
			}
			return false;
		}

		/**
		 * @brief Add a profession that is typically preferred by a member of the culture
		 * @param profession ProfessionData to add
		 */
		void addPreferredProfession(const ProfessionData& profession) { preferred_professions_.emplace(&profession); }

		/**
		 * @brief Set the professions that are typically preferred by a member of the culture
		 * @param preferred_professions std::set of ProfessionData pointers representing the professions that are typically preferred by a member of the culture
		 */
		void setPreferredProfessions(std::set<const ProfessionData*> preferred_professions) { preferred_professions_ = std::move(preferred_professions); }

		/**
		 * @brief Get a container with the professions that are typically preferred by a member of the culture
		 *
		 * This is not an exclusive list by any means, but rather suggestions
		 * @return std::set<ProfessionData> professions
		 */
		const std::set<const ProfessionData*>& preferredProfessions() const { return preferred_professions_; }

		/**
		 * @brief Get whether a profession is typically preferred by a member of the culture
		 * @param profession ProfessionData to check
		 * @return `true` if the profession is typically preferred by a member of the culture
		 * @return `false` if the profession is not typically preferred by a member of the culture
		 */
		bool isPreferredProfession(const ProfessionData& profession) const {
			for (const auto& key : preferredProfessions()) {
				if (profession.id() == key->id()) return true;
			}
			return false;
		}

		/**
		 * @brief Add a profession that is typically restricted to a member of the culture
		 * @param profession ProfessionData to add
		 */
		void addRestrictedProfession(const ProfessionData& profession) { restricted_professions_.emplace(&profession); }

		/**
		 * @brief Set the professions that are typically restricted to a member of the culture
		 * @param restricted_professions std::set of ProfessionData pointers representing the professions that are typically restricted to a member of the culture
		 */
		void setRestrictedProfessions(std::set<const ProfessionData*> restricted_professions) { restricted_professions_ = std::move(restricted_professions); }

		/**
		 * @brief Get a container with the professions that are typically restricted to a member of the culture
		 *
		 * This is not an exclusive list by any means, but rather suggestions
		 * @return std::set<ProfessionData> professions
		 */
		const std::set<const ProfessionData*>& restrictedProfessions() const { return restricted_professions_; }

		/**
		 * @brief Get whether a profession is typically restricted to a member of the culture
		 * @param profession ProfessionData to check
		 * @return `true` if the profession is typically restricted to a member of the culture
		 * @return `false` if the profession is not typically restricted to a member of the culture
		 */
		bool isRestrictedProfession(const ProfessionData& profession) const {
			for (const auto& key : restricted_professions_) {
				if (profession.id() == key->id()) return true;
			}
			return false;
		}

		/**
		 * @brief Add a modifier to the development point cost of a training package for members of the culture
		 * @param training_package TrainingPackageData pointer to the training package
		 * @param modifier int modifier to the development point cost of the training package for members of the culture
		 */
		void addTrainingPackageModifier(const TrainingPackageData& training_package, float modifier) { training_package_modifiers_.emplace(&training_package, modifier); }

		/**
		 * @brief Set the modifiers to the development point cost of training packages for members of the culture
		 * @param training_package_modifiers std::map associating TrainingPackageData pointers with their corresponding int modifiers to the development point cost of the training package for members of the culture
		 */
		void setTrainingPackageModifiers(std::map<const TrainingPackageData*, float> training_package_modifiers) { training_package_modifiers_ = std::move(training_package_modifiers); }

		/**
		 * @brief Get a container with the modifiers to the development point cost of training packages for members of the culture
		 * @return std::map associating TrainingPackageData pointers with their corresponding int modifiers to the development point cost of the training package for members of the culture
		 */
		std::map<const TrainingPackageData*, float> trainingPackageModifiers() const { return training_package_modifiers_; }

		/**
		 * @brief Get the modifier to the development point cost of a training package for members of the culture
		 *
		 * The modifier returned is a float that can be multiplied by the normal development point cost of the training package to get the modified cost for members of the culture.
		 * For example, a modifier of 0.8 would indicate that members of the culture pay 80% of the normal development point cost for the training package, while a modifier of 1.2
		 * would indicate that members of the culture pay 120% of the normal development point cost for the training package.
		 *
		 * @param training_package TrainingPackageData pointer to the training package
		 * @return float modifier to the development point cost of the training package for members of the culture
		 */
		float trainingPackageModifier(const TrainingPackageData& training_package) const {
			for (const auto& key : training_package_modifiers_) {
				if (training_package.id() == key.first->id()) return key.second;
			}
			return 1.0f; // Default modifier is 100% of the normal cost
		}

	private:
		std::string name_{}; /**< Name of the culture */
		std::string description_{}; /**< General description of the culture */
		const CultureTypeData* culture_type_{}; /**< Culture type this culture is based on */
		bool high_culture_{}; /**< Whether the culture has developed */
		std::map<std::string, const LanguageAbility> languages_{}; /**< Language ranks that members of the race learn prior during their adolescence */
		std::set<SubcategoriedSkillData> hobby_skills_{}; /**< Set of skills that would typically be given skill ranks by adolescents of this culture */
		std::set<const SkillCategoryData*> hobby_skill_categories_{}; /**< Set of skill categories that would typically be given skill ranks by adolescents of this culture */
		std::set<const ProfessionData*> preferred_professions_{}; /**< Set of preferred professions for members of the culture */
		std::set<const ProfessionData*> restricted_professions_{}; /**< Set of restricted professions for members of the culture */
		std::map<const TrainingPackageData*, float> training_package_modifiers_{}; /**< Modifiers to training package development point cost that members of the culture receive */
	};

} // namespace rm::rule