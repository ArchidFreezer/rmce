#pragma once

#include <map>
#include <set>
#include <stdexcept>
#include <string_view>
#include <tuple>
#include <Dice.h>
#include <EnumChoice.h>
#include <GameRuleData.h>
#include <RaceData.h>
#include <SpellListData.h>
#include <SkillCategoryData.h>
/**
 * @brief Represents a set of skill rank choices from a category available in a package, including constraints on selection and rank allocation.
 */
struct CategoryMultiSkillRankChoice {
	const SkillCategoryData* category; /**< The skill category from which the player can choose to gain the skill ranks */
	int ranks; /**< The number of skill ranks that can be gained */
	int num_choices; /**< The maximum number of skills that can be chosen to gain the skill ranks */

	/** Overload the less than operator to allow this struct to be used in sorted containers */
	bool operator<(const CategoryMultiSkillRankChoice& other) const {	
		size_t this_hash{ std::hash<std::string>{}(category->id()) };
		this_hash += std::hash<int>{}(ranks);
		this_hash += std::hash<int>{}(num_choices);

		size_t that_hash{ std::hash<std::string>{}(other.category->id()) };
		that_hash += std::hash<int>{}(other.ranks);
		that_hash += std::hash<int>{}(other.num_choices);

		return (this_hash < that_hash);
	}
};

/**
 * @brief Represents a set of spell list choices available in a package, including constraints on selection and rank allocation.
 */
struct SpellListChoices {
	std::optional<const SkillCategoryData*> spell_list_category; /**< Optional category of spell lists to choose from. The default category from the profession should be used if this is not provided or it is preferable. */
	int ranks; /**< The number of ranks the package provides in the spell list */
	int num_choices; /**< The number of spell lists that can be chosen from */
	std::set<const SpellListData*> spell_lists; /**< The spell lists that can be chosen from */

	/** Overload the less than operator to allow this struct to be used in sorted containers */
	bool operator<(const SpellListChoices& other) const {
		size_t this_hash{ 0 };
		if (spell_list_category) this_hash += std::hash<std::string_view>{}(spell_list_category.value()->id());
		this_hash += std::hash<int>{}(ranks);
		this_hash += std::hash<int>{}(num_choices);
		for (const SpellListData* spell_list : spell_lists) {
			this_hash += std::hash<std::string>{}(spell_list->id());
		}
		size_t that_hash{ 0 };
		if (other.spell_list_category) that_hash += std::hash<std::string_view>{}(other.spell_list_category.value()->id());
		that_hash += std::hash<int>{}(other.ranks);
		that_hash += std::hash<int>{}(other.num_choices);
		for (const SpellListData* spell_list : other.spell_lists) {
			that_hash += std::hash<std::string>{}(spell_list->id());
		}
		return (this_hash < that_hash);
	}
};

/**
 * @brief Represents a set of spell list category choices available in a package, including constraints on selection and rank allocation.
 */
struct SpellListCategoryChoices {
	int ranks; /**< The number of ranks the package provides in the spell list */
	int num_choices; /**< The number of spell lists that can be chosen from the category */
	std::set<std::string> spell_list_categories; /**< The spell list categories that can be chosen from */
};

/**
 * @brief Represents a set of language choices available in a package, including constraints on selection and rank allocation.
 */
struct LanguageChoices {
	int ranks; /**< The number of ranks the package provides in the language */
	int num_choices; /**< The number of languages that can be chosen */
	std::set<std::string> languages; /**< The languages that can be chosen from */
};

/**
 * @class TrainingPackageData
 * @brief A training package is a group of benefits (and sometimes disadvantages) that can be developed using development
 * points
 * 
 * The skills and abilities gained through a package represent long periods of exposure or training (sometimes years). If
 * these packages are developed after apprenticeship development, the character should be required to spend time in
 * training (the time will vary from package to package).
 * 
 * There are two different types of training packages: vocational and lifestyle. Most of the time, a character may have
 * only one lifestyle training package (GM discretion). However, he can have as many vocational training packages as he can
 * afford to purchase.
 * 
 * <em>Lifestyle</em> - These training packages represent groups of skills and abilities that would be gained by living a
 * particular lifestyle. Lifestyle packages should be hard to acquire, and take long periods of time when they are allowed.
 * 
 * <em>Vocational</em> - These training packages represent specific occupations in which a character may become proficient. He
 * will gain groups of skills and abilities that would necessarily be developed if the character were in the occupation.
 */
class TrainingPackageData : public GameRuleData {
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
	TrainingPackageData() = delete;

	/**
	 * @brief Basic constructor to initialise the base class
	 * @param id Unique identifier of the training package
	 */
	TrainingPackageData(std::string_view id) : GameRuleData(id) {}

	/**
	 * @brief Set the name of the training package
	 * @param name Training package name
	 */
	void setName(std::string_view name) { name_ = name; }

	/**
	 * @brief Get the name of the training package
	 * @return Training package name as a string reference
	 */
	const std::string& name() const { return name_; }

	/**
	 * @brief Set the description of what defines the training package
	 * @param description std::string_view training package description
	 */
	void setDescription(std::string_view description) { description_ = description; }

	/**
	 * @brief Get the description of what the defines training package
	 * @return std::string reference of the description
	 */
	const std::string& description() const { return description_; }

	/**
	 * @brief Set the flavour text of the training package that may be relevant for a GM when running a character with the training package
	 * @param flavour_text std::string_view flavour text about the training package
	 */
	void setFlavourText(std::string_view flavour_text) { flavour_text_ = flavour_text; }

	/**
	 * @brief Get the flavour text of the training package that may be relevant for a GM when running a character with the training package
	 * @return std::string reference of the flavour text about the training package
	 */
	const std::string& flavourText() const { return flavour_text_; }

	/**
	 * @brief Add an additional note about the training package that may be relevant for a GM when running a character with the training package
	 * @param note std::string_view additional note about the training package
	 */
	void addNote(std::string_view note) { notes_.emplace(note); }

	/**
	 * @brief Set additional notes about the training package that may be relevant for a GM when running a character with the training package
	 * @param notes Set of additional notes about the training package
	 */
	void setNotes(std::set<std::string> notes) { notes_ = std::move(notes); }

	/**
	 * @brief Get additional notes about the training package that may be relevant for a GM when running a character with the training package
	 * @return std::string reference of the additional notes about the training package
	 */
	const std::set<std::string>& notes() const { return notes_; }

	/**
	 * @brief Set the book that the training package is defined in
	 * @param book BookData pointer to the book
	 */
	void setBook(const BookData& book) { book_ = &book; }

	/**
	 * @brief Get the book that the training package is defined in
	 *
	 * @return BookData containing the training package definition
	 */
	const BookData& book() const { return *book_; }

	/**
	 * @brief  Add a race to those who can take the package if it is restricted
	 * 
	 * Some packages are restricted to a set of one or more races. The use of this method indicates that the package is one
	 * and that only races who have been added may take the package.
	 * 
	 * @param race RaceData to add
	 */
	void addRace(const RaceData& race) { races_.insert(&race); }

	/**
	 * @brief Sets the collection of race data.
	 * @param races A set of pointers to RaceData objects to be stored.
	 */
	void setRaces(std::set <const RaceData*> races) { races_ = std::move(races); }

	/**
	 * @brief Set of races that may take the package
	 * 
	 * The set will be empty for packages that have no restrictions
	 * 
	 * @return Set of races that may take the package
	 */
	const std::set<const RaceData*> races() const { return  races_; }

	/**
	 * @brief Check whether a race can take the training package
	 * 
	 * @param race RaceData to check
	 * @return `true` if the package is available to the race
	 * @return `false` if the package is not available to the race
	 */
	bool available(const RaceData& race) {
		if (!races_.empty()) {
			for (const auto& key : races_) {
				if (key->id() == race.id()) return true;
			}
			return false;
		}
		return true;
	}

	/**
	 * @brief Add a qualifier that, if met, reduces the cost of the training package
	 * 
	 * All conditions of a qualifier must be met in order to receive the reduced cost
	 * 
	 * @param qualifier Description of the qualifier
	 * @param reduction Reduction in package cost
	 */
	void addQualifier(std::string_view qualifier, int reduction) { qualifiers_.emplace(qualifier, reduction); }

	/**
	 * @brief Set a collection of qualifiers that, if met, reduce the cost of the training package
	 *
	 * Only a single discout can apply so if a character meets the requireemnts of more than one qualifier the one with the
	 * biggest discount should be applied.
	 *
	 * @param qualifiers Map of qualifier text and associated cost reduction
	 */
	void setQualifiers(std::map<std::string, int> qualifiers) { qualifiers_ = std::move(qualifiers); }

	/**
	 * @brief Get a collection of qualifiers that, if met, reduce the cost of the training package
	 * 
	 * Only a single discout can apply so if a character meets the requireemnts of more than one qualifier the one with the
	 * biggest discount should be applied.
	 * 
	 * @return Map of qualifier text and associated cost reduction
	 */
	const std::map<std::string, int>& qualifiers() const { return qualifiers_; }

	/**
	 * @brief Set whether the package is lifestyle based
	 * 
	 * @param lifestyle `true` if the package is a lfestyle package; `false` if the package is vocational
	 */
	void setLifestyle(bool lifestyle) { lifestyle_ = lifestyle; }

	/**
	 * @brief Get whether the package is lifestyle based
	 * @return `true` if the package is lifestyle based
	 * @return `false` if the package is not lifestyle based
	 */
	bool lifestyle() const { return lifestyle_; }

	/**
	 * @brief Set the number of months to acquire the package
	 * 
	 * This is the amount of time the character needs to train before gaining the benefits of the package. This time should
	 * be modified by a percentage equal to triple the character’s SD stat bonus (expressed as a percentage).
	 * 
	 * Example: If a character has a +5 SD stat bonus, he would gain the benefits of a training package 15% earlier than
	 * someone with a +0 SD bonus (i.e., 85% of the normal time). However, if the character has a -5 SD stat bonus, he
	 * would take 15% longer to gain the benefit (i.e., 115% of the normal time). 
	 * 
	 * @param months Number of months to acquire the package
	 */
	void setTimeToAcquire(int months) { time_to_acquire_ = months; }

	/**
	 * @brief Set the number of months to acquire the package
	 *
	 * This is the amount of time the character needs to train before gaining the benefits of the package. This time should
	 * be modified by a percentage equal to triple the character’s SD stat bonus (expressed as a percentage).
	 *
	 * Example: If a character has a +5 SD stat bonus, he would gain the benefits of a training package 15% earlier than
	 * someone with a +0 SD bonus (i.e., 85% of the normal time). However, if the character has a -5 SD stat bonus, he
	 * would take 15% longer to gain the benefit (i.e., 115% of the normal time).
	 *
	 * @return Number of months to acquire the package
	 */
	int timeToAcquire() const { return time_to_acquire_; }

	/**
	 * @brief The change in starting money for a character.
	 * 
	 * If the character takes this package as part of their apprenticeship it may modify the ammount of starting money they
	 * receive. The actual ammount is based on a random roll with this value setting the maximum or minimum change. The value
	 * returned is based on an open-ended high dice roll with open ended rolls being generated if the roll result is the
	 * maximum/minimum value.
	 * 
	 * Examples:
	 * + A value of 10 will return a number between 1 and 10, open-ended on 10 which may increase the number
	 * + A value of -8 will return a number between -1 and -8, open-ended on 8, which may reduce the number
	 *
	 * @param max Maximum difference in starting money
	 */
	void setStaringMoneyModifierDice(std::string_view max) { starting_money_modifier_dice_ = max; }

	/**
	 * @brief Get the change in starting money for a character if the package is taken during apprenticeship as a dice string
	 * 
	 * The actual ammount is based on a random roll with this value setting the maximum or minimum change. The value
	 * returned is based on an open-ended high dice roll with open ended rolls being generated if the roll result is the
	 * maximum/minimum value.
	 *
	 * Examples:
	 * + A value of 10 will return a number between 1 and 10, open-ended on 10 which may increase the number
	 * + A value of -8 will return a number between -1 and -8, open-ended on 8, which may reduce the number
	 *
	 * @return Dice string representing the change in starting money for a character if the package is taken during apprenticeship
	 */
	const std::string& startingMoneyModifierDice() const { return starting_money_modifier_dice_; }

	/**
	 * @brief Get the change in starting money if the package is taken during apprenticeship
	 * @return value of change in money
	 */
	int startingMoneyChange() const {

		if (starting_money_modifier_dice_.empty()) return 0; // No change if the string is empty

		// First we parse the dice string to get the absolute value of the change and then we roll it. The sign of the change is determined by whether the original value was positive or negative.
		std::string starting_money_modifier_dice = starting_money_modifier_dice_; // Make a copy of the string to modify for parsing

		// Get whether this is an increase or decrease in starting money
		bool neg = starting_money_modifier_dice.substr(0, 1) == "-";

		if (neg) {
			// Remove the negative sign for parsing
			starting_money_modifier_dice = starting_money_modifier_dice.substr(1);
		}

		// Get the number of dice and the number of sides on the dice from the string. The format is expected to be [number of dice]d[number of sides]
		std::vector<std::string> dice_parts = tokenise(starting_money_modifier_dice, "d");

		int num_dice = 1;
		int num_sides = 10;
		int val = 0;
		if (dice_parts.size() > 1) { // We have multiple dice so no open ended rolls
			num_dice = std::stoi(dice_parts[0]);
			num_sides = std::stoi(dice_parts[1]);
			val = Dice(num_sides, 0, 0).roll(num_dice).result();
		} else { // Single die with open ended rolls
			num_sides = std::stoi(dice_parts[0]);
			val = Dice(num_sides, 1, 0).roll(true).result();
		}

		return (neg ? val * -1 : val);
	}

	/**
	 * @brief Add a special benefit or malus that may be gained by the package
	 * @param special Description of the special benefit or malus
	 * @param chance Modifier to the chance of getting the special. This is added to the d100 roll when determining whether the special is gained.
	 */
	void addSpecial(std::string_view special, int chance) { specials_.emplace_back(std::make_pair(special, chance)); }

	/**
	 * @brief Set a collection of special benefits or maluses that may be gained by the package
	 * 
	 * The chance parameter is added to the d100 roll when determining whether the special is gained. For example, if a special has a chance of 20, then it would be gained on a roll of 80 or less on a d100. If the chance was -10, then it would only be gained on a roll of 90 or less.
	 * If a special is gained then the chance value should be halved for each subsequent roll to determine whether the special is gained again.
	 * 
	 * @param specials Vector of pairs of special benefit or malus descriptions and associated chances
	 */
	void setSpecials(std::vector<std::pair<std::string, int>> specials) { specials_ = std::move(specials); }

	/**
	 * @brief Get a collection of special benefits or maluses that may be gained by the package
	 * 
	 * The chance parameter is added to the d100 roll when determining whether the special is gained. For example, if a special has a chance of 20, then it would be gained on a roll of 80 or less on a d100. If the chance was -10, then it would only be gained on a roll of 90 or less.
	 * If a special is gained then the chance value should be halved for each subsequent roll to determine whether the special is gained again.
	 *
	 * @return Vector of pairs of special benefit or malus descriptions and associated chances
	 */
	const std::vector<std::pair<std::string, int>>& specials() const { return specials_; }

	/**
	 * @brief Add a stat that receives a stat gain roll as part of the package
	 * @param stat StatType::Type stat to gain roll
	 */
	void addStatGain(StatType::Type stat) { stat_gains_.emplace(stat); }

	/**
	 * @brief Set a collection of stats that receive a stat gain roll as part of the package
	 * @param stat_gains Set of StatType::Type stats to gain rolls for
	 */
	void setStatGains(std::set<StatType::Type> stat_gains) { stat_gains_ = std::move(stat_gains); }

	/**
	 * @brief Get set of stats that receive a stat gain roll
	 * @return Set of StatType::Type stats that recieve a stat gain roll
	 */
	const std::set< StatType::Type>& statGains() const { return stat_gains_; }

	/**
	 * @brief Set whether the package provides a stat gain roll for realm stats
	 * @param realmStatGain `true` if the package provides a stat gain roll for realm stats; `false` otherwise
	 */
	void setRealmStatGain(bool realmStatGain) { realmStatGain_ = realmStatGain; }

	/**
	 * @brief Get whether the package provides a stat gain roll for realm stats
	 * @return `true` if the package provides a stat gain roll for realm stats
	 * @return `false` if the package does not provide a stat gain roll for realm stats
	 */
	bool realmStatGain() const { return realmStatGain_; }

	/**
	 * @brief Set the number of stats that may be chosen to receive a stat gain roll and the set of stats to choose from
	 * @param choices Pair of number of stats that may be chosen to receive a stat gain roll and set of StatType::Type stats to choose from
	 */
	void setStatGainChoices(EnumChoice<StatType::Type> choices) { stat_gain_choices_ = choices; }

	/**
	 * @brief Get the number of stats that may be chosen to receive a stat gain roll and the set of stats to choose from
	 * @return Pair of number of stats that may be chosen to receive a stat gain roll and set of StatType::Type stats to choose from
	 */
	const EnumChoice<StatType::Type> statGainChoices() const { return stat_gain_choices_; }

	/**
	 * @brief Set the number of ranks for a skill the package provides
	 * @param skill SubcategoriedSkillData to add the ranks to
	 * @param ranks int number of ranks
	 */
	void addSkillRank(SubcategoriedSkillData skill, int ranks) {
		if (isRankSkill(skill.skillData(), skill.subcategory())) throw InvalidSkillRank("There is already a rank set for skill " + skill.id());
		skill_ranks_.emplace(std::move(skill), ranks);
	}

	/**
	 * @brief Set a collection of skills and ranks the package provides
	 * @param skill_ranks Map of SubcategoriedSkillData and associated number of ranks these will overwrite any existing skill ranks
	 */
	void setSkillRanks(std::map<SubcategoriedSkillData, int> skill_ranks) {	skill_ranks_ = std::move(skill_ranks); }

	/**
	 * @brief Get the number of ranks for a skill the package provides
	 * @param skill SubcategoriedSkillData to get the ranks for
	 * @return number of ranks
	 */
	int skillRank(const SubcategoriedSkillData& skill) const {
		for (auto& key : skill_ranks_) {
			if (key.first.id() == skill.id()) return key.second;
		}
		return 0;
	}

	/**
	 * @brief Get the number of ranks for a skill the package provides
	 * @param skill SkillData to get the ranks for
	 * @param subcategory optional subcategory of @a skill
	 * @return number of ranks
	 */
	int skillRank(const SkillData& skill, std::optional<std::string_view> subcategory = std::nullopt) const {
		return skillRank(SubcategoriedSkillData(skill, subcategory));
	}

	/**
	 * @brief Gets a container with the skills that the package provides
	 * @return std::set of SkillData with ranks
	 */
	const std::set<SubcategoriedSkillData> skillsWithRanks() const {
		std::set<SubcategoriedSkillData> ret;
		for (auto& key : skill_ranks_) {
			const SubcategoriedSkillData data(key.first.skillData(), key.first.subcategory());
			ret.insert(data);
		}
		return ret;
	}

	/**
	 * @brief Check if the package provides skill ranks for the skill
	 * @param skill SkillData to check
	 * @param subcategory optional subcategory of @a skill
	 * @return `true` if the package provides ranks
	 * @return `false` if the package does not provide ranks
	 */
	bool isRankSkill(const SkillData& skill, std::optional<std::string_view> subcategory = std::nullopt) const {
		for (auto& key : std::views::keys(skill_ranks_)) {
			if (key.skillData().id() == skill.id() && (subcategory ? subcategory.value() == key.subcategory().value() : !key.subcategory())) return true;
		}
		return false;
	}

	/**
	 * @brief Get a collection of skills and ranks the package provides
	 * @return Map of SubcategoriedSkillData and associated number of ranks
	 */
	const std::map<SubcategoriedSkillData, int>& skillRanks() const { return skill_ranks_; }

	/**
	 * @brief Set the skill rank choices for this package.
	 * @param skill_rank_choices A set of GameRuleDataChoice<SubcategoriedSkillData> representing the skill rank choices available in the package.
	 */
	void setSkillRankChoices(std::map<GameRuleDataChoice<SubcategoriedSkillData>, int> skill_rank_choices) { skill_rank_choices_ = std::move(skill_rank_choices); }

	/**
	 * @brief Gets the skill rank choices available for this training package.
	 * @return A set of skill rank choices, where each choice contains subcategorized skill data.
	 */
	const std::map<GameRuleDataChoice<SubcategoriedSkillData>, int>& skillRankChoices() const { return skill_rank_choices_; }

	/**
	 * @brief Sets the skill category ranks for this package.
	 * @param skill_category_ranks A map associating skill category data with their corresponding rank values.
	 */
	void setSkillCategoryRanks(std::map<const SkillCategoryData*, int> skill_category_ranks) { skill_category_ranks_ = std::move(skill_category_ranks); }

	/**
	 * @brief Get a collection of skill categories and ranks the package provides
	 * @return Map of SkillCategoryData pointers and associated number of ranks
	 */
	const std::map<const SkillCategoryData*, int>& skillCategoryRanks() const { return skill_category_ranks_; }

	/**
	 * @brief Sets the skill category from which the player can choose to gain the skill ranks, the number of skill ranks that can be gained, and the number of skills that can be chosen to gain the skill ranks.
	 * 
	 * Allows the player to select one or more skills from a single category to receive the skill ranks. This is typically used for general skills where the character can select a category of skills and then one or more specific skills within that category to gain the ranks in.
	 * 
	 * @param skill_category_multi_skill_rank_choices Tuple containing a SkillCategoryData pointer representing the skill category from which the player can choose to gain the skill ranks, an integer representing the number of skill ranks that can be gained, and an integer representing the number of skills that can be chosen to gain the skill ranks.
	 */
	void setSkillCategoryMultiSkillRankChoices(std::set<CategoryMultiSkillRankChoice> skill_category_multi_skill_rank_choices) { skill_category_multi_skill_rank_choices_ = std::move(skill_category_multi_skill_rank_choices); }

	/**
	 * @brief Get the skill category from which the player can choose to gain the skill ranks, the number of skill ranks that can be gained, and the number of skills that can be chosen to gain the skill ranks.
	 * @return Tuple containing a SkillCategoryData pointer representing the skill category from which the player can choose to gain the skill ranks, an integer representing the number of skill ranks that can be gained, and an integer representing the number of skills that can be chosen to gain the skill ranks.
	 */
	const std::set<CategoryMultiSkillRankChoice>& skillCategoryMultiSkillRankChoices() { return skill_category_multi_skill_rank_choices_; }

	/**
	 * @brief Sets the skill group category and skill ranks for this package.
	 * 
	 * Allows the player to select a single category from the group to recieve the skill ranks and also a single skill from that category to receive the skill ranks.
	 * This is typically used for weapon skills where the character can select a category of weapons and then a specific weapon within that category to gain the ranks in.
	 * 
	 * @param skill_group_category_and_skill_ranks A map associating SkillGroupData pointers with their corresponding rank values.
	 */
	void setSkillGroupCategoryAndSkillRanks(std::map<const SkillGroupData*, int> skill_group_category_and_skill_ranks) { skill_group_category_and_skill_ranks_ = std::move(skill_group_category_and_skill_ranks); }

	/**
	 * @brief Gets the skill group category and skill ranks map.
	 *
	 * Allows the player to select a single category from the group to recieve the skill ranks and also a single skill from that category to receive the skill ranks.
	 * This is typically used for weapon skills where the character can select a category of weapons and then a specific weapon within that category to gain the ranks in.
	 *
	 * @return A map containing skill group data pointers as keys and their corresponding rank values as integers.
	 */
	const std::map<const SkillGroupData*, int>& skillGroupCategoryAndSkillRanks() const { return skill_group_category_and_skill_ranks_; }

	/**
	 * @brief Sets the spell list choices for this package.
	 * @param spell_list_choices A set of SpellListChoices representing the spell list choices available in the package.
	 */
	void setSpellListChoices(std::set<SpellListChoices> spell_list_choices) { spell_list_choices_ = std::move(spell_list_choices); }

	/**
	 * @brief Add spell list choices for this package.
	 * @param spell_list_choices A set of SpellListChoices representing the spell list choices to be added to the package.
	 */
	void addSpellListChoices(std::set<SpellListChoices> spell_list_choices) { spell_list_choices_.insert(spell_list_choices.begin(), spell_list_choices.end()); }

	/**
	 * @brief Get the spell list choices for this package.
	 * @return A set of SpellListChoices representing the spell list choices available in the package.
	 */
	const std::set<SpellListChoices>& spellListChoices() const { return spell_list_choices_; }

	/**
	 * @brief Sets the spell list category choices for this package.
	 * @param spell_list_category_choices A set of SpellListCategoryChoices representing the spell list category choices available in the package.
	 */
	void setSpellListCategoryChoices(std::set<SpellListCategoryChoices> spell_list_category_choices) { spell_list_category_choices_ = std::move(spell_list_category_choices); }

	/**
	 * @brief Get the spell list category choices for this package.
	 * @return A set of SpellListCategoryChoices representing the spell list category choices available in the package.
	 */
	const std::set<SpellListCategoryChoices>& spellListCategoryChoices() const { return spell_list_category_choices_; }

	/**
	 * @brief Sets the lifestyle skills for this package.
	 * @param lifestyle_skills A set of SubcategoriedSkillData representing the lifestyle skills that may gain up to 15 ranks via the package rather than the usual cap of 10.
	 */
	void setLifestyleSkills(std::set<SubcategoriedSkillData> lifestyle_skills) { lifestyle_skills_ = std::move(lifestyle_skills); }

	/**
	 * @brief Get the lifestyle skills for this package.
	 * @return A set of SubcategoriedSkillData representing the lifestyle skills that may gain up to 15 ranks via the package rather than the usual cap of 10.
	 */
	const std::set<SubcategoriedSkillData>& lifestyleSkills() const { return lifestyle_skills_; }

	/**
	 * @brief Sets the lifestyle skill categories for this package.
	 * @param lifestyle_skill_categories A set of SkillCategoryData pointers representing the skill categories for which all skills within the category may gain up to 15 ranks via the package rather than the usual cap of 10.
	 */
	void setLifestyleSkillCategories(std::set<const SkillCategoryData*> lifestyle_skill_categories) { lifestyle_skill_categories_ = std::move(lifestyle_skill_categories); }

	/**
	 * @brief Get the lifestyle skill categories for this package.
	 * @return A set of SkillCategoryData pointers representing the skill categories for which all skills within the category may gain up to 15 ranks via the package rather than the usual cap of 10.
	 */
	const std::set<const SkillCategoryData*>& lifestyleSkillCategories() const { return lifestyle_skill_categories_; }

	/**
	 * @brief Sets the lifestyle skill groups for this package.
	 * @param lifestyle_skill_groups A set of SkillGroupData pointers representing the skill groups for which all skills within the group may gain up to 15 ranks via the package rather than the usual cap of 10.
	 */
	void setLifestyleSkillGroups(std::set<const SkillGroupData*> lifestyle_skill_groups) { lifestyle_skill_groups_ = std::move(lifestyle_skill_groups); }

	/**
	 * @brief Get the lifestyle skill groups for this package.
	 * @return A set of SkillGroupData pointers representing the skill groups for which all skills within the group may gain up to 15 ranks via the package rather than the usual cap of 10.
	 */
	const std::set<const SkillGroupData*>& lifestyleSkillGroups() const { return lifestyle_skill_groups_; }

	/**
	 * @brief Sets the lifestyle skill category multi skill rank choices for this package.
	 * @param lifestyle_skill_category_multi_skill_rank_choices A set of pairs of a set of SkillCategoryData pointers representing the skill categories from which the player may select one or more to gain up to 15 ranks from the package rather than the usual cap of 10 and an integer representing the number of skill categories that can be chosen.
	 */
	void setLifestyleSkillCategoryMultiSkillRankChoices(std::set<std::pair<std::set<const SkillCategoryData*>, int>> lifestyle_skill_category_multi_skill_rank_choices) { lifestyle_skill_category_multi_skill_rank_choices_ = std::move(lifestyle_skill_category_multi_skill_rank_choices); }

	/**
	 * @brief Get the lifestyle skill category multi skill rank choices for this package.
	 * @return A set of pairs of a set of SkillCategoryData pointers representing the skill categories from which the player may select one or more to gain up to 15 ranks from the package rather than the usual cap of 10 and an integer representing the number of skill categories that can be chosen.
	 */
	const std::set<std::pair<std::set<const SkillCategoryData*>, int>>& lifestyleSkill() const { return lifestyle_skill_category_multi_skill_rank_choices_; }

	/**
	 * @brief Sets the language choices.
	 * @param language_choices A set of language choices to be stored.
	 */
	void setLanguageChoices(std::set<LanguageChoices> language_choices) { language_choices_ = std::move(language_choices); }

	/**
	 * @brief Get the language choices.
	 * @return A set of language choices representing the language choices that the player may select from to gain ranks in languages.
	 */
	const std::set<LanguageChoices>& languageChoices() const { return language_choices_; }
	
private:
	std::string name_{}; /**< Name of the training package */
	std::string description_{}; /**< General description of the training package */
	std::string flavour_text_{}; /**< Piece of text to set the flavour of the training package */
	std::set<std::string> notes_{}; /**< Additional notes about the training package that may be relevant for a GM when running a character with the training package */
	const BookData* book_{}; /**< Book that the training package is described in */
	std::set<const RaceData*> races_{}; /**< Races allowed to take the package if it is restricted, empty for all races */
	std::map<std::string, int> qualifiers_{}; /**<  Requirements, if met, that reduce the cost of the package */
	bool lifestyle_{}; /**< Whether the package is a lifestyle package */
	int time_to_acquire_{}; /**< Time, in months, to acquire the package */
	std::string starting_money_modifier_dice_{}; /**< Change in starting money a character gains during apprenticeship */
	std::vector<std::pair<std::string, int>> specials_{}; /**< Special benefits or maluses that may be gained by the package */
	std::set<StatType::Type> stat_gains_{}; /**< Stats that receive a stat gain roll */
	bool realmStatGain_{}; /**< Whether the package provides a stat gain roll for realm stats */
	EnumChoice<StatType::Type> stat_gain_choices_{}; /**< A set of stats from which the player may select one or more from to receive a stat gain roll */
	std::map<SubcategoriedSkillData, int> skill_ranks_{}; /** Number of skill ranks gained */
	std::map<GameRuleDataChoice<SubcategoriedSkillData>, int> skill_rank_choices_{}; /**< A set of skills with ranks from which the player may select one or more from to receive the skill ranks */
	std::map<const SkillCategoryData*, int> skill_category_ranks_{}; /** Number of skill category ranks gained */
	std::set<CategoryMultiSkillRankChoice> skill_category_multi_skill_rank_choices_{}; /**< A set of skill categories from which the player may select one or more to receive the skill ranks */
	std::map<const SkillGroupData*, int> skill_group_category_and_skill_ranks_{}; /** A number fo ranks that may be set on a single caltegory within the group and also a single skill within that category */
	std::set<SpellListChoices> spell_list_choices_{}; /**< A set of spell list choices that the player may select from to gain spell lists and ranks in those spell lists */
	std::set<SpellListCategoryChoices> spell_list_category_choices_{}; /**< A set of spell list category choices that the player may select from to gain ranks in spell lists within those categories */
	std::set<SubcategoriedSkillData> lifestyle_skills_{}; /**< A set of skills that may gain up to 15 ranks via the package rather than the usual cap of 10 */
	std::set<const SkillCategoryData*> lifestyle_skill_categories_{}; /**< A set of skill categories for which all skills within the category may gain up to 15 ranks via the package rather than the usual cap of 10 */
	std::set<const SkillGroupData*> lifestyle_skill_groups_{}; /**< A set of skill groups for which all skills within the group may gain up to 15 ranks via the package rather than the usual cap of 10 */
	std::set<std::pair<std::set<const SkillCategoryData*>, int>> lifestyle_skill_category_multi_skill_rank_choices_{}; /**< A set of pairs of a set of skill categories from which the player may select one or more to gain up to 15 ranks from the package rather than the usual cap of 10 */
	std::set<LanguageChoices> language_choices_{}; /**< A set of language choices that the player may select from to gain ranks in languages */
};
