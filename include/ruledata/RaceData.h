#pragma once

#include <string>
#include <map>
#include <set>
#include <ranges>

#include <BookData.h>
#include <CreatureSizeType.h>
#include <CriticalTableType.h>
#include <GameRuleData.h>
#include <LanguageAbility.h>
#include <SkillCategoryData.h>
#include <SubcategoriedSkillData.h>
#include <SkillProgressionTypeData.h>
#include <GameRuleDataChoice.h>
#include <StatType.h>

/**
 * @brief Thrown when there is an attempt to set a skill as everymen when it is already set as resticted or vise-versa
 *
 * A skill cannot be both everymen and restricted for the same race
 */
class InvalidSkillDevelopment : public std::runtime_error{
public:
	/**
	 * @brief Exception constructor
	 *
	 * std::runtime_error will handle the string
	 *
	 * @param error String to display for the error
	 */
	InvalidSkillDevelopment(const std::string& error) : std::runtime_error{ error } {}
};

/**
 * @brief Thrown when there is an attempt to set an invalid skill bonus
 */
class InvalidSkillBonus : public std::runtime_error {
public:
	/**
	 * @brief Exception constructor
	 *
	 * std::runtime_error will handle the string
	 *
	 * @param error String to display for the error
	 */
	InvalidSkillBonus(const std::string& error) : std::runtime_error{ error } {}
};

/**
 * @class RaceData 
 * @brief Class representing the data of a race
 * 
 * This data is used during the character creation process
 */
class RaceData : public GameRuleData {
public:

	/**
	 * @brief Delete the defauilt constructor to ensure we initialise the base class
	 */
	RaceData() = delete;

	/**
	 * @brief Basic constructor
	 * @param id Unique identifier of the race
	 */
	RaceData(std::string_view id) : GameRuleData(id) {}

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
	 * @brief Set the book that the race is defined in
	 * @param book BookData pointer to the book
	 */
	void setBook(const BookData& book) { book_.emplace(&book); }

	/**
	 * @brief Get the book that the race is defined in
	 * 
	 * The book is stored as a pointer and may not have been initialised so it is considered optional. A check should be made
	 * before using the value to determine if the book has been set yet:
	 * @code
	 * if (race.book()) {                        // Check if the book has been set
	 *   const BookData* book = race.book().value();   // Get the book pointer
	 *   // Use the book
	 * }
	 * @endcode
	 * or
	 * @code
	 * std::cout << (race.book() ? race.book().value()->name() : "Book not set") << std::endl;
	 * @endcode
	 * @return Pointer to the BookData containing the race definition
	 */
	const std::optional<const BookData*> book() const { return book_; }

	/**
	 * @brief Set whether the race has developed artistic and intellectual pursuits
	 * @param high_culture `true` if the race has high culture; `false` otherwise
	 */
	void setHighCulture(bool high_culture) { high_culture_ = high_culture; }

	/**
	 * @brief Get whether the has developed artistic and intellectual pursuits
	 * @return `true` if the race has high culture
	 * @return `false` if the race has no high culture
	 */
	bool highCulture() const { return high_culture_; }

	/**
	 * @brief Set the size of the race
	 * 
	 * The CreatureSize comes into play if a member of the race is performing an attack that would resolve on a
	 * SpecialAttackTable such as a bite or similar.
	 * 
	 * @param size CreatureSizeType::Type of the race
	 */
	void setSize(CreatureSizeType::Type size) { size_ = size; }

	/**
	 * @brief Get the size of the race
	 *
	 * The CreatureSize comes into play if a member of the race is performing an attack that would resolve on a
	 * SpecialAttackTable such as a bite or similar.
	 *
	 * @return CreatureSizeType::Type of the race
	 */
	CreatureSizeType::Type size() const { return size_; }

	/**
	 * @brief Set the critical table type used for attacks against this race
	 * @param critical_table_type CriticalTableType::Type to resolve critical rolls against
	 */
	void setCriticalTableType(CriticalTableType::Type critical_table_type) { critical_table_type_ = critical_table_type; }

	/**
	 * @brief Set the critical table type used for attacks against this race
	 * @return CriticalTableType::Type to resolve critical rolls against
	 */
	CriticalTableType::Type criticalTableType() const { return critical_table_type_; }

	/**
	 * @brief Sets the relative rate that this race recovers from injuries
	 * 
	 * Some races recover from injuries at different rates than others and this value is a multipler to the average time it
	 * takes to recover from injuries.
	 * 
	 * @param recovery_multiplier float multiplier to standard recovery times
	 */
	void setRecoveryMultiplier(float recovery_multiplier) { recovery_multipler_ = recovery_multiplier; }

	/**
	 * @brief Get the relative rate that this race recovers from injuries
	 *
	 * Some races recover from injuries at different rates than others and this value is a multipler to the average time it
	 * takes to recover from injuries.
	 *
	 * @return float multiplier to standard recovery times
	 */
	float recoveryMultipler() const { return recovery_multipler_; }

	/**
	 * @brief Sets the number of background options members of this race have
	 * 
	 * During the character development process each character is allowed to spend background options on additional
	 * development activities with the number being based on the race.
	 * 
	 * @param num_background_options int number of background option points
	 */
	void setNumBackgroundOptions(int num_background_options) { num_background_options_ = num_background_options; }

	/**
	 * @brief Sets the number of background options members of this race have
	 *
	 * During the character development process each character is allowed to spend background options on additional
	 * development activities with the number being based on the race.
	 *
	 * @return int Number of background option points
	 */
	int numBackgroundOption() const { return num_background_options_; }

	/**
	 * @brief Sets any bonus the race has to exhaustion points
	 * 
	 * Some races have exceptional endurance and gain additional exhaustion points over and above those their stats provide
	 * @param exhaustion_bonus number of bonus exhaustion points
	 */
	void setExhaustionBonus(int exhaustion_bonus) { exhaustion_bonus_ = exhaustion_bonus; }

	/**
	 * @brief Gets any bonus the race has to exhaustion points
	 *
	 * Some races have exceptional endurance and gain additional exhaustion points over and above those their stats provide
	 * 
	 * @return number of bonus exhaustion points
	 */
	int exhaustionBonus() const { return exhaustion_bonus_; }

	/**
	 * @brief Sets the racial type used for stat loss
	 * 
	 * Upon death characters can start to lose stat points, initially temporary stats and then potential. The chance/rate at
	 * which they lose stats is based on the racial type. The scale is 1 to 5 with higher numbers losing stats faster.
	 * 
	 * @param stat_loss_racial_type int racial type used for stat loss
	 */
	void setStatLossRacialType(int stat_loss_racial_type) { stat_loss_racial_type_ = stat_loss_racial_type; }

	/**
	 * @brief Gets the racial type used for stat loss
	 *
	 * Upon death characters can start to lose stat points, initially temporary stats and then potential. The chance/rate at
	 * which they lose stats is based on the racial type. The scale is 1 to 5 with higher numbers losing stats faster.
	 *
	 * @return int racial type used for stat loss
	 */
	int statLossRacialType() const { return stat_loss_racial_type_; }

	/**
	 * @brief Set the number of hours sleep/meditation a member of this races needs to fully rest
	 * @param required_sleep int hours sleep/meditation required
	 */
	void setRequiredSleep(int required_sleep) { required_sleep_ = required_sleep; }

	/**
	 * @brief Get the number of hours sleep/meditation a member of this races needs to fully rest
	 * @return int number of hours sleep/meditation required
	 */
	int requiredSleep() const { return required_sleep_; }

	/**
	 * @brief Get how many days a member of the race can go without sleep before suffering penalties
	 * 
	 * Typically a character needs to sleep once every 24 hours. Without this they start to incur penalties. Some races
	 * have greater endurance and are able to last longer without sleep. This value is the number of days that a member
	 * of the race can go without sleep before incurring penalties.
	 * 
	 * @param required_sleep_frequency int number of days
	 */
	void setRequiredSleepFrequency(int required_sleep_frequency) { required_sleep_frequency_ = required_sleep_frequency; }

	/**
	 * @brief Get how many days a member of the race can go without sleep before suffering penalties
	 *
	 * Typically a character needs to sleep once every 24 hours. Without this they start to incur penalties. Some races
	 * have greater endurance and are able to last longer without sleep. This value is the number of days that a member
	 * of the race can go without sleep before incurring penalties.
	 *
	 * @return int number of days
	 */
	int requiredSleepFrequency() const { return required_sleep_frequency_; }

	/**
	 * @brief Sets the number of rounds after death that the soul of a member of this race departs the body.
	 * 
	 * Once a character dies if the physical damage that caused the death is not healed within a certain length of time the
	 * soul leaves the body. Once this happens healing the physical body may prevent things such as stat loss but the
	 * character cannot be brought back to life without a @e Lifegiving spell. If the physical body has not been healed the
	 * soul will not return even via spells.
	 * 
	 * @param soul_departure number of rounds before the soul departs
	 */
	void setSoulDeparture(int soul_departure) { soul_departure_ = soul_departure; }

	/**
	 * @brief gets the number of rounds after death that the soul of a member of this race departs the body.
	 *
	 * Once a character dies if the physical damage that caused the death is not healed within a certain length of time the
	 * soul leaves the body. Once this happens healing the physical body may prevent things such as stat loss but the
	 * character cannot be brought back to life without a @e Lifegiving spell. If the physical body has not been healed the
	 * soul will not return even via spells.
	 *
	 * @return number of rounds before the soul departs
	 */
	int soulDeparture() const { return soul_departure_;	}

	/**
	 * @brief Sets the modifier to members of this race compared to humans
	 * 
	 * Some races are generally slender or stout in build when compared to common men. This value is used to indicate this.
	 * The value is only used during character creation when creating the characters weight. It has no bearing on gameplay
	 * outside this.
	 * 
	 * The value is typically only has a value or between -2 (more slender) to 2 (more stout) and a number over 2 should be
	 * an extreme case, with -1 or 1 being by far the most common value outside 0.
	 * 
	 * @param build_modifier variance from human physical build shape
	 */
	void setBuildModifier(int build_modifier) { build_modifier_ = build_modifier; }

	/**
	 * @brief Gets the modifier to members of this race compared to humans
	 *
	 * Some races are generally slender or stout in build when compared to common men. This value is used to indicate this.
	 * The value is only used during character creation when creating the characters weight. It has no bearing on gameplay
	 * outside this.
	 *
	 * The value is typically only has a value or between -2 (more slender) to 2 (more stout) and a number over 2 should be
	 * an extreme case, with -1 or 1 being by far the most common value outside 0.
	 *
	 * @return variance from human physical build shape
	 */
	int buildModifier() const { return build_modifier_; }

	/**
	 * @brief Set the average male height in inches
	 * @param average_male_height average male height in inches
	 */
	void setAverageMaleHeight(int average_male_height) { average_male_height_ = average_male_height; }

	/**
	 * @brief get the average male height in inches
	 * @return average male height in inches
	 */
	int averageMaleHeight() const { return average_male_height_; }

	/**
	 * @brief Set the average female height in inches
	 * @param average_female_height average height in inches
	 */
	void setAverageFemaleHeight(int average_female_height) { average_female_height_ = average_female_height; }

	/**
	 * @brief get the average female height in inches
	 * @return average female height in inches
	 */
	int averageFemaleHeight() const { return average_female_height_; }

	/**
	 * @brief Sets how many years a member of the race can expect to live given an uneventful life
	 * 
	 * For many races it would be extremely rare for a member of the race to reach this value given their culture, e.g. Orcs
	 * rarely live up to, never mind beyond middle age due to their violent society.
	 * 
	 * @param average_lifespan number of years an average member of the race could life to
	 */
	void setAverageLifespan(int average_lifespan) { average_lifespan_ = average_lifespan; }

	/**
	 * @brief Gets how many years a member of the race can expect to live given an uneventful life
	 *
	 * For many races it would be extremely rare for a member of the race to reach this value given their culture, e.g. Orcs
	 * rarely live up to, never mind beyond middle age due to their violent society.
	 *
	 * @return number of years an average member of the race could life to
	 */
	int averageLifespan() const { return average_lifespan_; }

	/**
	 * @brief set the multiplier for automated weight generation formula for males
	 * 
	 * The formula to generate the weight of a character takes a height and build as inputs to generate a random weight.
	 * This formula assumes a creature skeleton size and shape, plus body mass of a common man which does not work for
	 * many races. This number is used to adjust the output to give values that better approximate the game books weight
	 * tables.
	 * 
	 * This value tends to increase the greater the difference in height from he race to a common man height.
	 * 
	 * @param male_weight_modifier weight generation modifier
	 */
	void setMaleWeightModifier(int male_weight_modifier) { male_weight_modifier_ = male_weight_modifier; }

	/**
	 * @brief Get the multiplier for automated weight generation formula for males
	 *
	 * The formula to generate the weight of a character takes a height and build as inputs to generate a random weight.
	 * This formula assumes a creature skeleton size and shape, plus body mass of a common man which does not work for
	 * many races. This number is used to adjust the output to give values that better approximate the game books weight
	 * tables.
	 *
	 * This value tends to increase the greater the difference in height from he race to a common man height.
	 *
	 * @return weight generation modifier
	 */
	int maleWeightModifier() const { return male_weight_modifier_; }

	/**
	 * @brief set the multiplier for automated weight generation formula for females
	 *
	 * The formula to generate the weight of a character takes a height and build as inputs to generate a random weight.
	 * This formula assumes a creature skeleton size and shape, plus body mass of a common man which does not work for
	 * many races. This number is used to adjust the output to give values that better approximate the game books weight
	 * tables.
	 *
	 * This value tends to increase the greater the difference in height from he race to a common man height.
	 *
	 * @param female_weight_modifier weight generation modifier
	 */
	void setFemaleWeightModifier(int female_weight_modifier) { female_weight_modifier_ = female_weight_modifier; }

	/**
	 * @brief Get the multiplier for automated weight generation formula for females
	 *
	 * The formula to generate the weight of a character takes a height and build as inputs to generate a random weight.
	 * This formula assumes a creature skeleton size and shape, plus body mass of a common man which does not work for
	 * many races. This number is used to adjust the output to give values that better approximate the game books weight
	 * tables.
	 *
	 * This value tends to increase the greater the difference in height from he race to a common man height.
	 *
	 * @return weight generation modifier
	 */
	int femaleWeightModifier() const { return female_weight_modifier_; }

	/**
	 * @brief Set the bonus progression for body development skill ranks
	 * 
	 * When adding ranks into the body development skill the bonus received per rank depends on the number of ranks already
	 * allocated and the character race. This defines the progression of these.
	 * 
	 * @param arms_progression SkillProgressionTypeData pointer to the progression
	 */
	void setArmsProgression(const SkillProgressionTypeData& arms_progression) { arms_progression_ = &arms_progression; }

	/**
	 * @brief Get the bonus progression for body development skill ranks
	 *
	 * When adding ranks into the body development skill the bonus received per rank depends on the number of ranks already
	 * allocated and the character race. This defines the progression of these.
	 *
	 * @return Reference to the SkillProgressionTypeData containing the progression
	 */
	const SkillProgressionTypeData& armsProgression() const { return *arms_progression_; }

	/**
	 * @brief Set the bonus progression for arcane power point skill ranks
	 *
	 * When adding ranks into the arcane power point skill the bonus received per rank depends on the number of ranks already
	 * allocated and the character race. This defines the progression of these.
	 *
	 * @param arcane_progression SkillProgressionTypeData pointer to the progression
	 */
	void setArcaneProgression(const SkillProgressionTypeData& arcane_progression) { arcane_progression_ = &arcane_progression; }

	/**
	 * @brief Get the bonus progression for arcane power point skill ranks
	 *
	 * When adding ranks into the arcane power point skill the number of power points received per rank depends on the number
	 * of ranks already allocated and the character race. This defines the progression of these.
	 *
	 * @return Reference to the SkillProgressionTypeData containing the progression
	 */
	const SkillProgressionTypeData& arcaneProgression() const { return *arcane_progression_; }

	/**
	 * @brief Set the bonus progression for channeling power point skill ranks
	 *
	 * When adding ranks into the channeling power point skill the bonus received per rank depends on the number of ranks already
	 * allocated and the character race. This defines the progression of these.
	 *
	 * @param channeling_progression SkillProgressionTypeData pointer to the progression
	 */
	void setChannelingProgression(const SkillProgressionTypeData& channeling_progression) { channeling_progression_ = &channeling_progression; }

	/**
	 * @brief Get the bonus progression for channeling power point skill ranks
	 *
	 * When adding ranks into the channeling power point skill the number of power points received per rank depends on the number
	 * of ranks already allocated and the character race. This defines the progression of these.
	 *
	 * @return Reference to the SkillProgressionTypeData containing the progression
	 */
	const SkillProgressionTypeData& channelingProgression() const { return *channeling_progression_; }

	/**
	 * @brief Set the bonus progression for essence power point skill ranks
	 *
	 * When adding ranks into the essence power point skill the bonus received per rank depends on the number of ranks already
	 * allocated and the character race. This defines the progression of these.
	 *
	 * @param essence_progression SkillProgressionTypeData pointer to the progression
	 */
	void setEssenceProgression(const SkillProgressionTypeData& essence_progression) { essence_progression_ = &essence_progression; }

	/**
	 * @brief Get the bonus progression for essence power point skill ranks
	 *
	 * When adding ranks into the essence power point skill the number of power points received per rank depends on the number
	 * of ranks already allocated and the character race. This defines the progression of these.
	 *
	 * @return Reference to the SkillProgressionTypeData containing the progression
	 */
	const SkillProgressionTypeData& essenceProgression() const { return *essence_progression_; }

	/**
	 * @brief Set the bonus progression for mentalism power point skill ranks
	 *
	 * When adding ranks into the mentalism power point skill the bonus received per rank depends on the number of ranks already
	 * allocated and the character race. This defines the progression of these.
	 *
	 * @param mentalism_progression SkillProgressionTypeData pointer to the progression
	 */
	void setMentalismProgression(const SkillProgressionTypeData& mentalism_progression) { mentalism_progression_ = &mentalism_progression; }

	/**
	 * @brief Get the bonus progression for mentalism power point skill ranks
	 *
	 * When adding ranks into the mentalism power point skill the number of power points received per rank depends on the number
	 * of ranks already allocated and the character race. This defines the progression of these.
	 *
	 * @return Reference to the SkillProgressionTypeData containing the progression
	 */
	const SkillProgressionTypeData& mentalismProgression() const { return *mentalism_progression_; }

	/**
	 * @brief Add the number of ranks a member of the race gets in a language during childhood
	 * @param language LanguageAbility containing the ranks for a language
	 */
	void setStartingLanguageAbility(LanguageAbility language) { starting_languages_.emplace(language.language(), language); }

	/**
	 * @brief Gets the staring ability a member of the race has in a language
	 * @param language LanguageData language to get the ability for
	 * @return LanguageAbility language ability
	 */
	const LanguageAbility& startingLanguageAbility(const LanguageData& language) const { return startingLanguageAbility(language.name()); }
	
	/**
	 * @brief Gets the staring ability a member of the race has in a language
	 * @param language_name name of the language to get the ability for
	 * @return LanguageAbility language ability
	 */
	const LanguageAbility& startingLanguageAbility(const std::string language_name) const { return starting_languages_.at(language_name); }
	
	/**
	 * @brief Get a container with the LanguageAbility objects known during childhood
	 * @return std::vector of LanguageAbility object references
	 */
	const std::vector<LanguageAbility> startingLanguages() const {
		auto values = std::views::values(starting_languages_);
		return { values.begin(), values.end() };
	}
	
	/**
	 * @brief Add the number of ranks a member of the race gets in a language during childhood
	 * @param language LanguageAbility containing the ranks for a language
	 */
	void setAdolescentLanguageAbility(LanguageAbility language) { adolescent_languages_.emplace(language.language(), language); }

	/**
	 * @brief Gets the staring ability a member of the race has in a language
	 * @param language LanguageData language to get the ability for
	 * @return LanguageAbility language ability
	 */
	const LanguageAbility& adolescentLanguageAbility(const LanguageData& language) const { return adolescentLanguageAbility(language.name()); }

	/**
	 * @brief Gets the staring ability a member of the race has in a language
	 * @param language_name name of the language to get the ability for
	 * @return LanguageAbility language ability
	 */
	const LanguageAbility& adolescentLanguageAbility(const std::string language_name) const { return adolescent_languages_.at(language_name); }

	/**
	 * @brief Get a container with the LanguageAbility objects known during childhood
	 * @return std::set of LanguageAbility object references
	 */
	const std::vector<LanguageAbility> adolescentLanguages() const {
		auto values = std::views::values(adolescent_languages_);
		return { values.begin(), values.end() };
	}
	
	/**
	 * @brief Sets the racial bonus for a stat
	 * 
	 * @param stat StatType::Type to set the bonus for
	 * @param bonus value of bonus
	 */
	void setStatBonus(StatType::Type stat, int bonus) { stat_bonuses_.emplace(stat, bonus); }

	/**
	 * @brief Get the bonus that the race provides to a stat
	 * @param stat StatType::Type to get the bonus for
	 * @return bonus value
	 */
	int statBonus(StatType::Type stat) const { return stat_bonuses_.count(stat) ? stat_bonuses_.at(stat) : 0; }

	/**
	 * @brief Gets a container with the stats that the race has a bonus for
	 * @return std::set of StatType::Type with bonuses
	 */
	const std::set<StatType::Type> statsWithBonus() const {
		auto keys = std::views::keys(stat_bonuses_);
		return { keys.begin(), keys.end() };
	}

	/**
	 * @brief Add a skill that is considered everyman for the race
	 * @param skill SubcategoriedSkillData unique pointer to the everyman skill
	 * @throw InvalidSkillDevelopment If the skill is set as restricted
	 * @see SkillDevelopmentType
	 */
	void addEverymanSkill(SubcategoriedSkillData skill) {
		if (isRestrictedSkill(skill.skillData(), skill.subcategory())) throw InvalidSkillDevelopment("Cannot set " + skill.id() + " as everyman as it is already set as restricted");
		if (isEverymanSkill(skill.skillData(), skill.subcategory())) throw InvalidSkillDevelopment("Cannot set " + skill.id() + " as everyman as it is already defined");
		everyman_skills_.insert(std::move(skill));
	}

	/**
	 * @brief Get a container of the skills that are considered everyman
	 * @return std::set container of everyman skills
	 * @see SkillDevelopmentType
	 */
	const std::set<SubcategoriedSkillData> everymanSkills() const { 
		std::set<SubcategoriedSkillData> ret;
		for (auto& skill : everyman_skills_) {
			const SubcategoriedSkillData data(skill.skillData(), skill.subcategory());
			ret.insert(data);
		}
		return ret; 
	}

	/**
	 * @brief Check if a skill is everyman for the race
	 * @param other Skill to check
	 * @param subcategory optional subcategory of @a other
	 * @return `true` if the skill is everyman
	 * @return `false` if the skill is not everyman
	 * @see SkillDevelopmentType
	 */
	bool isEverymanSkill(const SkillData& other, std::optional<std::string_view> subcategory = std::nullopt) const {
		for (auto& skill : everyman_skills_) {
			if (skill.skillData().id() == other.id() && (subcategory ? subcategory.value() == skill.subcategory() : !skill.subcategory())) return true;
		}
		return false;
	}

	/**
	 * @brief Add a skill that is considered restricted for the race
	 * @param skill SubcategoriedSkillData unique pointer to the restricted skill
	 * @throw InvalidSkillDevelopment If the skill is set as restricted
	 * @see SkillDevelopmentType
	 */
	void addRestrictedSkill(SubcategoriedSkillData skill) {
		if (isEverymanSkill(skill.skillData(), skill.subcategory())) throw InvalidSkillDevelopment("Cannot set " + skill.id() + " as restricted as it is already set as everyman");
		if (isRestrictedSkill(skill.skillData(), skill.subcategory())) throw InvalidSkillDevelopment("Cannot set " + skill.id() + " as restricted as it is already defined");
		restricted_skills_.insert(std::move(skill));
	}

	/**
	 * @brief Get a container of the skills that are considered restricted
	 * @return std::set container of restricted skills
	 */
	const std::set<SubcategoriedSkillData> restrictedSkills() const {
		std::set<SubcategoriedSkillData> ret;
		for (auto& skill : restricted_skills_) {
			const SubcategoriedSkillData data(skill.skillData(), skill.subcategory());
			ret.insert(data);
		}
		return ret;
	}

	/**
	 * @brief Check if a skill is restricted for the race
	 * @param other Skill to check
	 * @param subcategory optional subcategory of @a other
	 * @return `true` if the skill is restricted
	 * @return `false` if the skill is not restricted
	 * @see SkillDevelopmentType
	 */
	bool isRestrictedSkill(const SkillData& other, std::optional<std::string_view> subcategory = std::nullopt) const {
		for (auto& skill : restricted_skills_) {
			if (skill.skillData().id() == other.id() && (subcategory ? subcategory.value() == skill.subcategory() : !skill.subcategory())) return true;
		}
		return false;
	}

	/**
	 * @brief Add a skill category that is considered everyman for the race
	 * @param category SkillCategoryData reference to the everyman skill
	 * @throw InvalidSkillDevelopment If the skill is set as restricted
	 * @see SkillDevelopmentType
	 */
	void addEverymanSkillCategory(const SkillCategoryData& category) {
		if (isRestrictedSkillCategory(category)) throw InvalidSkillDevelopment("Cannot set " + category.id() + " as everyman as it is already set as restricted");
		if (isEverymanSkillCategory(category)) throw InvalidSkillDevelopment("Cannot set " + category.id() + " as everyman as it is already defined");
		everyman_skill_categories_.insert(&category);
	}

	/**
	 * @brief Sets the everyman skill categories
	 * @param categories A set of pointers to SkillCategoryData to mark as everyman.
	 */
	void setEverymanSkillCategories(const std::set<const SkillCategoryData*>& categories) {
		everyman_skill_categories_ = categories;
	}

	/**
	 * @brief Get a container of the skill categories that are considered everyman
	 * @return std::set container of everyman skill categories
	 */
	const std::set<const SkillCategoryData*> everymanSkillCategories() const { return everyman_skill_categories_; }

	/**
	 * @brief Check if a skill category is everyman for the race
	 * @param other Skill category to check
	 * @return `true` if the skill category is everyman
	 * @return `false` if the skill category is not everyman
	 * @see SkillDevelopmentType
	 */
	bool isEverymanSkillCategory(const SkillCategoryData& other) const {
		for (auto& category : everyman_skill_categories_) {
			if (category->id() == other.id()) return true;
		}
		return false;
	}

	/**
	 * @brief Add a skill category that is considered restricted for the race
	 * @param category SkillCategoryData reference to the restricted skill
	 * @throw InvalidSkillDevelopment If the skill is set as restricted
	 * @see SkillDevelopmentType
	 */
	void addRestrictedSkillCategory(const SkillCategoryData& category) {
		if (isEverymanSkillCategory(category)) throw InvalidSkillDevelopment("Cannot set " + category.id() + " as restricted as it is already set as everyman");
		if (isRestrictedSkillCategory(category)) throw InvalidSkillDevelopment("Cannot set " + category.id() + " as restricted as it is already defined");
		restricted_skill_categories_.insert(&category);
	}

	/**
	 * @brief Sets the restricted skill categories
	 * @param categories A set of pointers to SkillCategoryData to mark as restricted.
	 */
	void setRestrictedSkillCategories(const std::set<const SkillCategoryData*>& categories) {
		restricted_skill_categories_ = categories;
	}

	/**
	 * @brief Get a container of the skill categories that are considered restricted
	 * @return std::set container of restricted skill categories
	 * @see SkillDevelopmentType
	 */
	const std::set<const SkillCategoryData*> restrictedSkillCategories() const { return restricted_skill_categories_; }

	/**
	 * @brief Check if a skill category is restricted for the race
	 * @param other Skill category to check
	 * @return `true` if the skill category is restricted
	 * @return `false` if the skill category is not restricted
	 * @see SkillDevelopmentType
	 */
	bool isRestrictedSkillCategory(const SkillCategoryData& other) const {
		for (auto& category : restricted_skill_categories_) {
			if (category->id() == other.id()) return true;
		}
		return false;
	}

	/**
	 * @brief Set the bonus the race has with a skill
	 * @param skill SubcategoriedSkillData to set the bonus for
	 * @param bonus int bonus value
	 */
	void setSkillBonus(SubcategoriedSkillData skill, int bonus) {
		if (isBonusSkill(skill.skillData(), skill.subcategory())) throw InvalidSkillBonus("There is already a bonus set for skill " + skill.id());
		skill_bonuses_.emplace(std::move(skill), bonus); 
	}

	/**
	 * @brief Sets the skill bonuses the race provides
	 */
	void setSkillBonuses(const std::map<SubcategoriedSkillData, int>& bonuses) {
		skill_bonuses_ = bonuses;
	}

	/**
	 * @brief Returns a read-only reference to the map of skill bonuses.
	 * @return A const reference to the internal std::map<SubcategoriedSkillData, int> that maps each SubcategoriedSkillData to its integer bonus.
	 */
	const std::map<SubcategoriedSkillData, int>& skillBonuses() const { return skill_bonuses_; }

	/**
	 * @brief Get the bonus that the race provides to a skill
	 * @param skill SubcategoriedSkillData to get the bonus for
	 * @return bonus value
	 */
	int skillBonus(const SubcategoriedSkillData& skill) const {
		for (auto& key : skill_bonuses_) {
			if (key.first.id() == skill.id()) return key.second;
		}
		return 0;
	}

	/**
	 * @brief Get the bonus that the race provides to a skill
	 * @param skill SkillData to get the bonus for
	 * @param subcategory optional subcategory of @a skill
	 * @return bonus value
	 */
	int skillBonus(const SkillData& skill, std::optional<std::string_view> subcategory = std::nullopt) const {
		return skillBonus(SubcategoriedSkillData(skill, subcategory));
	}

	/**
	 * @brief Gets a container with the skill that the race has a bonus for
	 * @return std::set of SkillData with bonuses
	 */
	const std::set<SubcategoriedSkillData> skillsWithBonus() const {
		std::set<SubcategoriedSkillData> ret;
		for (auto& key : skill_bonuses_) {
			const SubcategoriedSkillData data(key.first.skillData(), key.first.subcategory());
			ret.insert(data);
		}
		return ret;
	}

	/**
	 * @brief Check if there is a racial bonus for a skill
	 * @param skill SkillData to check
	 * @param subcategory optional subcategory of @a skill
	 * @return `true` if the skill has a bonus
	 * @return `false` if the skill does not have a bonus
	 */
	bool isBonusSkill(const SkillData& skill, std::optional<std::string_view> subcategory = std::nullopt) const {
		for (auto& key : std::views::keys(skill_bonuses_)) {
			if (key.skillData().id() == skill.id() && (subcategory ? subcategory.value() == key.subcategory().value() : !key.subcategory())) return true;
		}
		return false;
	}

	/**
	 * @brief Add a choice that defines the skill categories that a character may select one or more skills from to become everyman
	 * @param choice GameRuleDataChoice choice definition
	 * @see SkillDevelopmentType
	 */
	void addCategoryEverymanSkillChoice(GameRuleDataChoice<SkillCategoryData> choice) { category_everyman_skill_choices_.push_back(std::move(choice)); }

	/**
	 * @brief Get the number of choices a character has to make regarding everyman skills in skill cetegories
	 * 
	 * During character development it is sometimes necessary to choose one or more skills from a skill category to have
	 * everyman development. This returns how many choices need to be made. This is not the number of options to be selected
	 * in a single choice.
	 * @return Number of choices to be make
	 * @see SkillDevelopmentType
	 */
	int numCategoryEverymanSkillChoices() const { return category_everyman_skill_choices_.size(); }

	/**
	 * @brief Get a container with the choices the character needs to make to select one or more skills within a skill
	 * category that may be made everyme skills
	 * @return vector of GameRuleDataChoice objects with the choices to be made
	 * @see SkillDevelopmentType
	 */
	const std::vector<GameRuleDataChoice<SkillCategoryData>>& categoryEverymanSkillChoices() const { return category_everyman_skill_choices_; }

private:
	std::string name_{}; /**< Name of the race */
	std::string description_{}; /**< General description of the race */
	std::optional<const BookData*> book_{std::nullopt}; /**< Book that the race is described in */
	bool high_culture_{true}; /**< Whether the race has developed artistic and intellectual pursuits */
	CreatureSizeType::Type size_{ CreatureSizeType::Type::kMedium}; /**< Size of unarmed attacks made by the race */
	CriticalTableType::Type critical_table_type_{ CriticalTableType::Type::kNormal}; /**< The type of critical table attacks on this race are rolled against */
	float recovery_multipler_{ 1.0f }; /**< Multiplier to standard healing recovery rates */
	int num_background_options_{}; /**< Number of background options members of this race have */
	int exhaustion_bonus_{0}; /**< Bonus exhaustion points the race has */
	int stat_loss_racial_type_{}; /**< Racial type used on stat loss table */
	int required_sleep_{8}; /**< Number of hours of sleep required to fully refresh a member of this race */
	int required_sleep_frequency_{ 1 }; /**< How many days a member of the race can go without sleep before suffering penalties */
	int soul_departure_{}; /**< How many rounds after death before a characters soul departs their body */
	int build_modifier_{0}; /**< How far form a normal human build are members of this race */
	int average_male_height_{}; /**< Average height in inches of a male member of the race */
	int average_female_height_{}; /**< Average height in inches of a female member of the race */
	int average_lifespan_{}; /**< Average lifespan, in years, of a member of the race */
	float male_weight_modifier_{ 1.0f }; /**< Multiplier for automated weight generation formula for males */
	float female_weight_modifier_{ 1.0f }; /**< Multiplier for automated weight generation formula for females */
	const SkillProgressionTypeData* arms_progression_{}; /**< Skill progression for body development ranks */
	const SkillProgressionTypeData* arcane_progression_{}; /**< Skill progression for arcane power point ranks */
	const SkillProgressionTypeData* channeling_progression_{}; /**< Skill progression for channeling power point ranks */
	const SkillProgressionTypeData* essence_progression_{}; /**< Skill progression for essence power point ranks */
	const SkillProgressionTypeData* mentalism_progression_{}; /**< Skill progression for mentalism power point ranks */
	std::map<std::string, const LanguageAbility> starting_languages_{}; /**< Language ranks that members of the race learn prior to their adolescence */
	std::map<std::string, const LanguageAbility> adolescent_languages_{}; /**< Language ranks available to members of the race during adolescence */
	std::map<StatType::Type, int> stat_bonuses_{}; /**< Racial stats bonuses */
	std::set<SubcategoriedSkillData> everyman_skills_{}; /**< Skills that are considered everyman for the race */
	std::set<SubcategoriedSkillData> restricted_skills_{}; /**< Skills that are considered restricted for the race */
	std::set<const SkillCategoryData*> everyman_skill_categories_{}; /**< Skill categories that are considered everyman for the race */
	std::set<const SkillCategoryData*> restricted_skill_categories_{}; /**< Skill categories that are considered restricted for the race */
	std::map<SubcategoriedSkillData, int> skill_bonuses_{}; /** Racial skill bonuses */
	std::vector<GameRuleDataChoice<SkillCategoryData>> category_everyman_skill_choices_{}; /** Set of skill categories that the character may select one or more skill from to become everyman */
};
