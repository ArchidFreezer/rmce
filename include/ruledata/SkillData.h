#pragma once

#include <string_view>
#include <vector>

#include <BookData.h>
#include <GameRuleData.h>
#include <StatType.h>
#include <SkillActionType.h>
#include <SkillCategoryData.h>

/**
 * @class SkillData 
 * @brief Class representing the definition of a skill that a character may develop
 */
class SkillData : public GameRuleData {
public:
	/**
	 * @brief Constructor
	 * @param id Unique identifier for the skill
	 */
	SkillData(std::string_view id) : GameRuleData(id) {}

	/**
	 * @brief Set the name of the skill
	 * @param name Skill name
	 */
	void setName(std::string_view name) { name_ = name; }
	/**
	 * @brief Get the name of the skill
	 * @return Skill name as a string reference
	 */
	const std::string& name() const { return name_; }

	/**
	 * @brief Set the description of what the skill does
	 * @param description string_view of what the skill does
	 */
	void setDescription(std::string_view description) { description_ = description; }

	/**
	 * @brief Set what the skill does
	 * @return string reference to what the skill does
	 */
	const std::string& description() const { return description_; }

	/**
	 * @brief Set the book that the skill is defined in
	 * @param book BookData pointer to the book
	 */
	void setBook(const BookData& book) { book_ = &book; }

	/**
	 * @brief Get the book that the skill is defined in
	 * @return Reference to the BookData containing the skill definition
	 */
	const BookData& book() const { return *book_; }

	/**
	 * @brief Set the type of action type used when performing the skill
	 * @param action_type SkillActionType::Type of the skill
	 */
	void setActionType(SkillActionType::Type action_type) { action_type_ = action_type; }

	/**
	 * @brief Get the action type used when performign the skill
	 * @return SkillActionType::Type of the skill
	 */
	SkillActionType::Type actionType() const { return action_type_; }

	/**
	 * @brief Example manoeuvres of various difficulties to aid the GM
	 * 
	 * The summary uses html list tags to separate the specific difficulties
	 * @param difficulty_summary std::string_view containing summaries of exampl difficulties
	 */
	void setDifficultySummary(std::string_view difficulty_summary) { difficulty_summary_ = difficulty_summary; }

	/**
	 * @brief Get some example manoeuvres of various difficulties to aid the GM
	 * 
	 * The summary uses html list tags to separate the specific difficulties
	 * @return std::string containing the summary text
	 */
	const std::string& difficultySummary() const { return difficulty_summary_; }

	/**
	 * @brief Additional information concerning maneuver resolution or other uses for the skill.
	 * 
	 * These also describes how to determine the difficulty of a maneuver.
	 * 
	 * @param notes std::string_view skill notes to set
	 */
	void setNotes(std::string_view notes) { notes_ = notes; }

	/**
	 * @brief Additional information concerning maneuver resolution or other uses for the skill.
	 *
	 * These also describes how to determine the difficulty of a maneuver.
	 * 
	 * @return string reference to the notes
	 */
	const std::string& notes() const { return notes_; }

	/**
	 * @brief Sets if the skill is typically a restricted skill
	 * 
	 * Some skill are extremely difficult for some races or professions to learn and this is represented in game by the use
	 * of the SkillDevelopmentType::Type.kRestricted development type. If this variable is set to `true` then it is expected
	 * that this is the default for any race and/or profession for this skill.
	 * 
	 * @param restricted `true` if the skill is typically restricted; `false` otherwise
	 */
	void setIsRestricted(bool restricted) { restricted_ = restricted; }

	/**
	 * @brief Sets if the skill is typically a restricted skill
	 *
	 * Some skill are extremely difficult for some races or professions to learn and this is represented in game by the use
	 * of the SkillDevelopmentType::Type.kRestricted development type.
	 * 
	 * @return `true` if the skill is restricted for all races and professions
	 * @return `false` if the skill is generally not restricted
	 */
	bool restricted() const { return restricted_; }

	/**
	 * @brief Set the skill category the skill belongs to
	 * @param category SkillCategoryData category
	 */
	void setCategory(const SkillCategoryData& category) { category_ = &category; }

	/**
	 * @brief Get the skill category the skill belongs to
	 * @return SkillCategoryData reference to the category
	 */
	const SkillCategoryData& category() const { return *category_; }

	/**
	 * @brief Set whether a character can focus their skill proficiencies more finely.
	 * 
	 * Many skills (e.g., History, Religion, etc.) allow a character either to develop a broad understanding of an entire field
	 * or to develop a more precise understanding of a sub-field in which they may become an expert.
	 * 
	 * @param can_specialise `true` if the skill can be specialised; `false` otherwise
	 */
	void setCanSpecialise(bool can_specialise) { can_specialise_ = can_specialise; }

	/**
	 * @brief Get whether a character can focus their skill proficiencies more finely.
	 *
	 * Many skills (e.g., History, Religion, etc.) allow a character either to develop a broad understanding of an entire field
	 * or to develop a more precise understanding of a sub-field in which they may become an expert.
	 * 
	 * @return `true` if the skill can be specialised
	 * @return `false` if the skill cannot be specialised
	 */
	bool canSpecialise() const { return can_specialise_; }

	/**
	 * @brief Whether the character must pick a subcategory of the skill
	 * 
	 * Many skills require that a character develop the skill for each sub-category of the skill (e.g., Weapons, Riding,
	 * Metal-crafts, etc.). This variable defines whether that is the case for the skill.
	 * 
	 * @param mandatory_subcategory `true` if a sub-category must be selected; `false` otherwise
	 */
	void setMandatorySubcategory(bool mandatory_subcategory) { mandatory_subcategory_ = mandatory_subcategory; }

	/**
	 * @brief Whether the character must pick a subcategory of the skill
	 *
	 * Many skills require that a character develop the skill for each sub-category of the skill (e.g., Weapons, Riding,
	 * Metal-crafts, etc.). This variable defines whether that is the case for the skill.
	 * 
	 * @return `true` if a sub-category must be selected
	 * @return `false` if a sub-category need not be selected
	 */
	bool mandatorySubcategory() const { return mandatory_subcategory_; }

	/**
	 * @brief Add a subcategory type to the list of those available for selection
	 * 
	 * Many skills require that a character develop the skill for each sub-category of the skill (e.g., Weapons, Riding,
	 * Metal-crafts, etc.). This values adds one that is available for selection.
	 *
	 * @param subcategory std::string type to add to the skill
	 */
	void addSubcategory(std::string subcategory) { subcategories_.push_back(subcategory); }

	/**
	 * @brief Gets a collection of the subcategories available for a skill
	 * 
	 * @return std::vectore reference containing the list of available subcategory types.
	 */
	const std::vector<std::string>& getSubcategories() const { return subcategories_; }

	/**
	 * @brief Add stat that applies stat bonus to the skill
	 *
	 * There are 3 stats associated with each skill and duplicates are allowed
	 *
	 * @param stat whose bonus should be applied to the skill
	 * @returns Number of stats associated withthe category after the operation
	 * @throws TooManyStatsException if attempting to add a stat when 3 already have been applied
	 * @throws UsingCharacterRealmStatsException if attempting to add a stat when using character realm stats
	 * @see setUseRealmStats()
	 */
	int addStat(StatType::Type stat);

	/**
	 * @brief Gets the number of stats currently associated with the category
	 *
	 * Up to 3 stats bonuses may be applied to skills in a skill category. This function returns the current number applied
	 * @return Number of stats applied
	 */
	const int numberOfStats() const { return stats_.size(); }

	/**
	 * @brief Remove a stats from those currently associated with the category
	 *
	 * Remove a stat from the list of those whose bonus is currently applied to skill in the category.
	 * If the stat is not in the currently applied then the function does nothing
	 *
	 * @param stat Stat to remove
	 * @return `true` if the stat was removed
	 * @return `false` if the stat was not found or not removed
	 */
	bool removeStat(StatType::Type stat);

	/**
	 * @brief Empties the list of stats associated with the category
	 */
	void clearStats() { stats_.clear(); }

	/**
	 * @brief Gets the stats that that provide a bonus to the category and skills within it
	 *
	 * A stat may appear more than once in the list
	 *
	 * @return std::vector of stats
	 */
	const std::vector<StatType::Type>& stats() { return stats_; }

	/**
	 * @brief Number of exhaustion points expended when the skill is used at a normal pace
	 * 
	 * Normal pace is typically defined as walking pace. If a value of 0.0f is set then there are no additional
	 * exhaustion points expended when using the skill.
	 * 
	 * @param cost float number of exhaustion points
	 */
	void setExhaustionCost(float cost) { exhaustion_cost_ = cost; }

	/**
	 * @brief Gets the exhaustion points expended per round when the skill is used at a normal pace
	 *
	 * Normal pace is typically defined as walking pace
	 *
	 * @return float exhaustion points expended per round; 0.0f if no additional points are expended
	 */
	float exhaustionCost() const { return exhaustion_cost_; }

	/**
	 * @brief Multipler applied to the characters base movement rate when using this skill.
	 * 
	 * If this value is 0 then the character may not normally move when using the skill.
	 * @param distance_multiplier multiplier to the characters base rate
	 */
	void setDistanceMultiplier(float distance_multiplier) { distance_multiplier_ = distance_multiplier; }

	/**
	 * @brief Get the multipler applied to the characters base movement rate when using this skill.
	 *
	 * @return `float` multiplier to the characters base rate
	 * @return 0.0f the character cannot normally move using this skill
	 */
	float distanceMultiplier() const { return distance_multiplier_; }

	/**
	 * @brief Thrown when attempting to add a skill category stats when the category is set to use character realm stats
	 *
	 * It is invalid to have both skill category stats and also to use the characters realm stats as these are mutually
	 * exclusive options.
	 */
	class UsingCharacterRealmStatsException : public std::runtime_error {
	public:
		/**
		 * @brief Exception constructor
		 *
		 * std::runtime_error will handle the string
		 *
		 * @param error String to display for the error
		 */
		UsingCharacterRealmStatsException(const std::string& error) : std::runtime_error{ error }{}
	};

private:
	std::string name_{}; /**< Name of the skill as seen in-game */
	std::string description_{}; /**< What the skill is used for */
	const BookData* book_{}; /** Game rule book that the skill appears in */
	SkillActionType::Type action_type_{}; /**< Type of skill action performed when usng the skill */
	std::string difficulty_summary_{}; /**< Description of the difficulties of some use cases using the skill */
	std::string notes_{}; /**< Additional informatio on the skill and its usage */
	bool restricted_{}; /**< Is the SkillDevelopmentType normally kRestricted */
	const SkillCategoryData* category_{}; /** The skill category the skill belongs to */
	bool can_specialise_{}; /** Whether the character can select a speciality for the skill */
	bool mandatory_subcategory_{}; /**< Whether the character must select a specific type of the this skills focus */
	std::vector<std::string> subcategories_{}; /**< Definition of the types of subcategories the skill has */
	std::vector<StatType::Type> stats_{}; /**< Stats providing a bonus to the skill */
	float exhaustion_cost_{}; /** The number of exhaution points expended per round when using the skill at normal pace */
	float distance_multiplier_{}; /** Multiplier for the distance moved when using this skill */
};