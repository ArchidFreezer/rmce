#include <TrainingPackageData.h>

int TrainingPackageData::startingMoneyChange() const {

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


const std::set<SubcategoriedSkillData> TrainingPackageData::skillsWithRanks() const {
	std::set<SubcategoriedSkillData> ret;
	for (auto& key : skill_ranks_) {
		const SubcategoriedSkillData data(key.first.skillData(), key.first.subcategory());
		ret.insert(data);
	}
	return ret;
}

bool TrainingPackageData::isRankSkill(const SkillData& skill, std::optional<std::string_view> subcategory) const {
	for (auto& key : std::views::keys(skill_ranks_)) {
		if (key.skillData().id() == skill.id() && (subcategory ? subcategory.value() == key.subcategory().value() : !key.subcategory())) return true;
	}
	return false;
}

bool TrainingPackageData::available(const RaceData& race) {
	if (!races_.empty()) {
		for (const auto& key : races_) {
			if (key->id() == race.id()) return true;
		}
		return false;
	}
	return true;
}

bool CategoryMultiSkillRankChoice::operator<(const CategoryMultiSkillRankChoice& other) const {
	size_t this_hash{ std::hash<std::string>{}(category->id()) };
	this_hash += std::hash<int>{}(ranks);
	this_hash += std::hash<int>{}(num_choices);

	size_t that_hash{ std::hash<std::string>{}(other.category->id()) };
	that_hash += std::hash<int>{}(other.ranks);
	that_hash += std::hash<int>{}(other.num_choices);

	return (this_hash < that_hash);
}

bool SpellListChoices::operator<(const SpellListChoices& other) const {
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

bool SpellListCategoryChoices::operator<(const SpellListCategoryChoices& other) const {
	size_t this_hash{ std::hash<int>{}(ranks) };
	this_hash += std::hash<int>{}(num_choices);
	for (const SkillCategoryData* category : spell_list_categories) {
		this_hash += std::hash<std::string>{}(category->id());
	}
	size_t that_hash{ std::hash<int>{}(other.ranks) };
	that_hash += std::hash<int>{}(other.num_choices);
	for (const SkillCategoryData* category : other.spell_list_categories) {
		that_hash += std::hash<std::string>{}(category->id());
	}
	return (this_hash < that_hash);
}