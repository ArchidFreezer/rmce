#include <GameRuleBoostConverter.h>
#include <TrainingPackageSerializer.h>

namespace rm::rule::parser {

void TrainingPackageSerializer::parse(pt::ptree& ptree) {
	std::cout << "Loading TrainingPackage data ..." << std::endl;

	// Get the trainingpackages to parse and loop through them
	const pt::ptree& tree = ptree.get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", manager().generateId(ruleDatatype(), name));

		// Get a TrainingPackageData object from the object manager
		TrainingPackageData& ref = manager().get<TrainingPackageData>(id);
		ref.setName(name);
		ref.setDescription(v.second.get<std::string>("description"));

		boost::optional<std::string> flavour_text = v.second.get_optional<std::string>("flavour-text");
		if (flavour_text)
			ref.setFlavourText(flavour_text.value());

		// Get the book from the cache
		std::string book_id = v.second.get<std::string>("book");
		ref.setBook(manager().get<BookData>(book_id));

		// Cost qualifiers
		ref.setQualifiers(parseQualifiers(v.second.get_child_optional("qualifiers")));

		// Notes
		boost::optional<const pt::ptree&> notes_tree = v.second.get_child_optional("notes");
		if (notes_tree) {
			std::set<std::string> notes{};
			for (const auto& note : notes_tree.get()) {
				notes.emplace(note.second.get_value<std::string>());
			}
			ref.setNotes(notes);
		}

		GameRuleBoostConverter converter{manager()};

		// Race restrictions
		ref.setRaces(converter.parseGameDataSetTree<RaceData>(v.second.get_child_optional("races")));

		ref.setLifestyle(v.second.get<bool>("lifestyle", false));
		ref.setTimeToAcquire(v.second.get<int>("time-to-acquire", 0));

		// Starting money modifier dice
		boost::optional<std::string> starting_money_modifier_dice = v.second.get_optional<std::string>("starting-money-modifier-dice");
		if (starting_money_modifier_dice)
			ref.setStaringMoneyModifierDice(starting_money_modifier_dice.value());

		// Specials
		boost::optional<const pt::ptree&> specials_tree = v.second.get_child_optional("specials");
		if (specials_tree)
			ref.setSpecials(parseSpecials(specials_tree));

		// Stat gains
		boost::optional<const pt::ptree&> stat_gains = v.second.get_child_optional("stat-gains");
		if (stat_gains)
			ref.setStatGains(converter.parseEnumSetTree<StatType::Type>(stat_gains));

		// Realm stat gain
		boost::optional<const pt::ptree&> realm_stat_gain_tree = v.second.get_child_optional("realm-stat-gain");
		if (realm_stat_gain_tree) {
			ref.setRealmStatGain(realm_stat_gain_tree.get().get_value<bool>());
		}

		// Stat gain choices
		boost::optional<const pt::ptree&> stat_gain_choices_tree = v.second.get_child_optional("stat-gain-choices");
		ref.setStatGainChoices(parseStatGainChoices(stat_gain_choices_tree));

		// Skill ranks
		boost::optional<const pt::ptree&> skill_ranks_tree = v.second.get_child_optional("skill-ranks");
		if (skill_ranks_tree) {
			ref.setSkillRanks(converter.parseSkillPairTree<int>(skill_ranks_tree));
		}

		// Skill rank choices
		boost::optional<const pt::ptree&> skill_rank_choices_tree = v.second.get_child_optional("skill-rank-choices");
		if (skill_rank_choices_tree) {
			ref.setSkillRankChoices(converter.parseSkillChoicePairTree<int>(skill_rank_choices_tree));
		}

		// Skill category ranks
		boost::optional<const pt::ptree&> skill_category_ranks_tree = v.second.get_child_optional("category-ranks");
		if (skill_category_ranks_tree) {
			ref.setSkillCategoryRanks(converter.parseGameDataPairTree<SkillCategoryData, int>(skill_category_ranks_tree));
		}

		// Skill category multi-skill rank choices
		boost::optional<const pt::ptree&> skill_category_multi_rank_choices_tree = v.second.get_child_optional("category-multi-skill-rank-choices");
		if (skill_category_multi_rank_choices_tree) {
			ref.setSkillCategoryMultiSkillRankChoices(parseCategoryMultiSkillRankChoices(skill_category_multi_rank_choices_tree));
		}

		// Skill group multi-skill rank choices
		boost::optional<const pt::ptree&> skill_group_multi_rank_choices_tree = v.second.get_child_optional("group-multi-skill-rank-choices");
		if (skill_group_multi_rank_choices_tree) {
			ref.setSkillGroupMultiSkillRankChoices(parseGroupMultiSkillRankChoices(skill_group_multi_rank_choices_tree));
		}

		// Skill group: category and skill ranks
		boost::optional<const pt::ptree&> skill_group_tree = v.second.get_child_optional("group-category-and-skill-rank-choices");
		if (skill_group_tree) {
			ref.setSkillGroupCategoryAndSkillRanks(parseSkillGroupCategoryAndSkillRankChoicesTree(skill_group_tree));
		}

		// Spell list ranks
		boost::optional<const pt::ptree&> spell_list_ranks_tree = v.second.get_child_optional("spell-list-ranks");
		if (spell_list_ranks_tree) {
			ref.setSpellListChoices(parseSpellListChoicesTree(spell_list_ranks_tree));
		}

		// Spell list category rank choices
		boost::optional<const pt::ptree&> spell_list_category_rank_choices_tree = v.second.get_child_optional("spell-list-category-rank-choices");
		if (spell_list_category_rank_choices_tree) {
			ref.setSpellListCategoryChoices(parseSpellListCategoryChoicesTree(spell_list_category_rank_choices_tree));
		}

		// Lifestyle skills
		boost::optional<const pt::ptree&> lifestyle_skills_tree = v.second.get_child_optional("lifestyle-skills");
		if (lifestyle_skills_tree) {
			ref.setLifestyleSkills(converter.parseSkillSetTree(lifestyle_skills_tree));
		}

		// Lifestyle categories
		boost::optional<const pt::ptree&> lifestyle_categories_tree = v.second.get_child_optional("lifestyle-categories");
		if (lifestyle_categories_tree) {
			ref.setLifestyleSkillCategories(converter.parseGameDataSetTree<SkillCategoryData>(lifestyle_categories_tree));
		}

		// Lifestyle groups
		boost::optional<const pt::ptree&> lifestyle_groups_tree = v.second.get_child_optional("lifestyle-groups");
		if (lifestyle_groups_tree) {
			ref.setLifestyleSkillGroups(converter.parseGameDataSetTree<SkillGroupData>(lifestyle_groups_tree));
		}

		// Lifestyle category skill choices
		boost::optional<const pt::ptree&> lifestyle_category_skill_choices_tree = v.second.get_child_optional("lifestyle-category-skill-choices");
		if (lifestyle_category_skill_choices_tree) {
			ref.setLifestyleCategorySkillChoices(converter.parseGameDataChoiceSetTree<SkillCategoryData>(lifestyle_category_skill_choices_tree));
		}

		// Language choices
		boost::optional<const pt::ptree&> language_choices_tree = v.second.get_child_optional("language-choices");
		if (language_choices_tree) {
			ref.setLanguageChoices(converter.parseGameDataChoicePairTree<LanguageData, int>(language_choices_tree));
		}

		std::cout << "\tTrainingPackage name: " << ref.name() << std::endl;
	}
	std::cout << " done" << std::endl;
}

void TrainingPackageSerializer::populateDatum(std::string key, pt::ptree& datum) {
	TrainingPackageData& game_data = manager().get<TrainingPackageData>(key);
	datum.put("id", game_data.id());
	datum.put("name", game_data.name());
	datum.put("description", game_data.description());
	if (!game_data.flavourText().empty())
		datum.put("flavour-text", game_data.flavourText());
	datum.put("book", game_data.book().id());

	// Qualifiers
	{
		pt::ptree tree{getQualifiersTree(game_data)};
		if (tree.size())
			datum.push_back(std::make_pair("qualifiers", tree));
	}

	// Notes
	{
		pt::ptree tree{};
		for (const std::string& note : game_data.notes()) {
			pt::ptree note_tree{};
			note_tree.put("", note);
			tree.push_back(std::make_pair("", note_tree));
		}
		if (tree.size())
			datum.push_back(std::make_pair("notes", tree));
	}

			GameRuleBoostConverter converter{manager()};

	// Races
	{
		        pt::ptree tree{converter.getGameDataSetTree<RaceData>(game_data.races())};
		if (tree.size())
			datum.push_back(std::make_pair("races", tree));
	}

	// Lifestyle
	datum.put("lifestyle", game_data.lifestyle());
	// Time to acquire
	datum.put("time-to-acquire", game_data.timeToAcquire());
	// Starting money modifier dice
	if (!game_data.startingMoneyModifierDice().empty())
		datum.put("starting-money-modifier-dice", game_data.startingMoneyModifierDice());

	// Specials
	{
		pt::ptree tree{getSpecialsTree(game_data)};
		if (tree.size())
			datum.push_back(std::make_pair("specials", tree));
	}

	// Stat gains
	{
		pt::ptree tree{converter.getEnumSetTree<StatType::Type>(game_data.statGains())};
		if (tree.size())
			datum.push_back(std::make_pair("stat-gains", tree));
	}

	// Realm stat gain
	if (game_data.realmStatGain())
		datum.put("realm-stat-gain", true);

	// Stat gain choices
	{
		pt::ptree tree{getStatGainChoicesTree(game_data)};
		if (tree.size())
			datum.push_back(std::make_pair("stat-gain-choices", tree));
	}

	// Skill ranks
	{
		pt::ptree tree{converter.getSkillPairTree<int>(game_data.skillRanks())};
		if (tree.size())
			datum.push_back(std::make_pair("skill-ranks", tree));
	}

	// Skill rank choices
	{
		pt::ptree tree{converter.getSkillChoicePairTree<int>(game_data.skillRankChoices())};
		if (tree.size())
			datum.push_back(std::make_pair("skill-rank-choices", tree));
	}

	// Skill category ranks
	{
		pt::ptree tree{converter.getGameDataPairTree<SkillCategoryData, int>(game_data.skillCategoryRanks())};
		if (tree.size())
			datum.push_back(std::make_pair("category-ranks", tree));
	}

	// Skill category multi-skill rank choices
	{
		pt::ptree tree{getCategoryMultiSkillRankChoicesTree(game_data)};
		if (tree.size())
			datum.push_back(std::make_pair("category-multi-skill-rank-choices", tree));
	}

	// Skill group multi-skill rank choices
	{
		pt::ptree tree{getGroupMultiSkillRankChoicesTree(game_data)};
		if (tree.size())
			datum.push_back(std::make_pair("group-multi-skill-rank-choices", tree));
	}

	// Skill group: category and skill ranks
	{
		pt::ptree tree{getSkillGroupCategoryAndSkillRankChoicesTree(game_data)};
		if (tree.size())
			datum.push_back(std::make_pair("group-category-and-skill-rank-choices", tree));
	}

	// Spell list ranks
	{
		pt::ptree tree{getSpellListChoicesTree(game_data)};
		if (tree.size())
			datum.push_back(std::make_pair("spell-list-ranks", tree));
	}

	// Spell list rank choices
	{
		pt::ptree tree{getSpellListCategoryChoicesTree(game_data)};
		if (tree.size())
			datum.push_back(std::make_pair("spell-list-category-rank-choices", tree));
	}

	// Lifestyle skills
	{
		pt::ptree tree{converter.getSkillSetTree(game_data.lifestyleSkills())};
		if (tree.size())
			datum.push_back(std::make_pair("lifestyle-skills", tree));
	}

	// Lifestyle categories
	{
		pt::ptree tree{converter.getGameDataSetTree<SkillCategoryData>(game_data.lifestyleSkillCategories())};
		if (tree.size())
			datum.push_back(std::make_pair("lifestyle-categories", tree));
	}

	// Lifestyle groups
	{
		pt::ptree tree{converter.getGameDataSetTree<SkillGroupData>(game_data.lifestyleSkillGroups())};
		if (tree.size())
			datum.push_back(std::make_pair("lifestyle-groups", tree));
	}

	// Lifestyle category skill choices
	{
		pt::ptree tree{converter.getGameDataChoiceSetTree<SkillCategoryData>(game_data.lifestyleCategorySkillChoices())};
		if (tree.size())
			datum.push_back(std::make_pair("lifestyle-category-skill-choices", tree));
	}

	// Language choices
	{
		pt::ptree tree{converter.getGameDataChoicePairTree<LanguageData, int>(game_data.languageChoices())};
		if (tree.size())
			datum.push_back(std::make_pair("language-choices", tree));
	}
}

std::string TrainingPackageSerializer::rootNode() const {
	return "training-packages";
}

	std::map<std::string, int> TrainingPackageSerializer::parseQualifiers(boost::optional<const pt::ptree&> qualifiers) {
	std::map<std::string, int> quals;

	if (!qualifiers)
		return quals;

	for (const auto& v : qualifiers.get()) {
		std::string qualifier_text = v.second.get<std::string>("qualifier");
		int reduction = v.second.get<int>("reduction");
		quals.emplace(qualifier_text, reduction);
	}
	return quals;
}

const pt::ptree TrainingPackageSerializer::getQualifiersTree(TrainingPackageData& game_data) {
	pt::ptree tree{};
	for (const auto& [qualifier, reduction] : game_data.qualifiers()) {
		pt::ptree value_tree{};
		value_tree.put("qualifier", qualifier);
		value_tree.put("reduction", reduction);
		tree.push_back(std::make_pair("", value_tree));
	}
	return tree;
}

std::vector<std::pair<std::string, int>> TrainingPackageSerializer::parseSpecials(boost::optional<const pt::ptree&> specials) {
	std::vector<std::pair<std::string, int>> special_list;
	if (!specials)
		return special_list;

	for (const auto& v : specials.get()) {
		std::string special_text = v.second.get<std::string>("value");
		int chance = v.second.get<int>("chance");
		special_list.emplace_back(special_text, chance);
	}

	return special_list;
}

const pt::ptree TrainingPackageSerializer::getSpecialsTree(TrainingPackageData& game_data) {
	pt::ptree tree{};
	for (const auto& [special, reduction] : game_data.specials()) {
		pt::ptree value_tree{};
		value_tree.put("value", special);
		value_tree.put("chance", reduction);
		tree.push_back(std::make_pair("", value_tree));
	}
	return tree;
}

EnumChoice<StatType::Type> TrainingPackageSerializer::parseStatGainChoices(boost::optional<const pt::ptree&> stat_gain_choices) {
	EnumChoice<StatType::Type> choices{};
	if (!stat_gain_choices)
		return choices;

	choices.setNumChoices(stat_gain_choices.value().get<int>("num-choices"));
	for (const auto& option : stat_gain_choices.value().get_child("options")) {
		StatType::Type stat_gain_option_enum{};
		fromString(option.second.get_value<std::string>(), stat_gain_option_enum);
		choices.addOption(stat_gain_option_enum);
	}

	return choices;
}

const pt::ptree TrainingPackageSerializer::getStatGainChoicesTree(TrainingPackageData& game_data) {
	pt::ptree choice_tree{};
	EnumChoice<StatType::Type> stat_gain_choices = game_data.statGainChoices();
	if (!stat_gain_choices.numChoices())
		return choice_tree;

	choice_tree.put("num-choices", stat_gain_choices.numChoices());
	pt::ptree options_tree{};
	std::map<std::string, StatType::Type> sorted_options{};
	for (const StatType::Type option : stat_gain_choices.options<StatType::Type>()) {
		sorted_options.emplace(toString(option), option);
	}
	for (const auto& option_pair : sorted_options) {
		pt::ptree option_tree{};
		option_tree.put("", option_pair.second);
		options_tree.push_back(std::make_pair("", option_tree));
	}
	choice_tree.push_back(std::make_pair("options", options_tree));

	return choice_tree;
}

std::set<CategoryMultiSkillRankChoice> TrainingPackageSerializer::parseCategoryMultiSkillRankChoices(boost::optional<const pt::ptree&> category_multi_skill_rank_choices) {
	std::set<CategoryMultiSkillRankChoice> choices{};
	if (!category_multi_skill_rank_choices)
		return choices;

	for (const auto& choice : category_multi_skill_rank_choices.get()) {
		CategoryMultiSkillRankChoice choice_struct{};
		std::string category_id = choice.second.get<std::string>("id");
		choice_struct.category = &manager().get<SkillCategoryData>(category_id);
		choice_struct.ranks = choice.second.get<int>("value");
		choice_struct.num_choices = choice.second.get<int>("num-choices");
		choices.emplace(choice_struct);
	}

	return choices;
}

const pt::ptree TrainingPackageSerializer::getCategoryMultiSkillRankChoicesTree(TrainingPackageData& game_data) {
	pt::ptree choice_tree{};

	for (const auto& choice : game_data.skillCategoryMultiSkillRankChoices()) {
		pt::ptree choice_struct_tree{};
		choice_struct_tree.put("id", choice.category->id());
		choice_struct_tree.put("value", choice.ranks);
		choice_struct_tree.put("num-choices", choice.num_choices);
		choice_tree.push_back(std::make_pair("", choice_struct_tree));
	}

	return choice_tree;
}

std::vector<GroupMultiSkillRankChoice> TrainingPackageSerializer::parseGroupMultiSkillRankChoices(boost::optional<const pt::ptree&> group_multi_skill_rank_choices) {
	std::vector<GroupMultiSkillRankChoice> choices{};
	if (!group_multi_skill_rank_choices)
		return choices;

	for (const auto& choice : group_multi_skill_rank_choices.get()) {
		GroupMultiSkillRankChoice choice_struct{};
		std::string group_id = choice.second.get<std::string>("id");
		choice_struct.group = &manager().get<SkillGroupData>(group_id);
		choice_struct.ranks = choice.second.get<int>("value");
		choice_struct.num_choices = choice.second.get<int>("num-choices");
		choices.emplace_back(choice_struct);
	}

	return choices;
}

const pt::ptree TrainingPackageSerializer::getGroupMultiSkillRankChoicesTree(TrainingPackageData& game_data) {
	pt::ptree choice_tree{};

	for (const auto& choice : game_data.skillGroupMultiSkillRankChoices()) {
		pt::ptree choice_struct_tree{};
		choice_struct_tree.put("id", choice.group->id());
		choice_struct_tree.put("value", choice.ranks);
		choice_struct_tree.put("num-choices", choice.num_choices);
		choice_tree.push_back(std::make_pair("", choice_struct_tree));
	}

	return choice_tree;
}

std::set<SpellListChoices> TrainingPackageSerializer::parseSpellListChoicesTree(boost::optional<const pt::ptree&> spell_list_choices) {
	std::set<SpellListChoices> choices{};
	if (!spell_list_choices)
		return choices;

	for (const auto& choice : spell_list_choices.get()) {
		SpellListChoices choice_struct{};
		boost::optional<std::string> category_id = choice.second.get_optional<std::string>("optional-category");
		if (category_id)
			choice_struct.spell_list_category = &manager().get<SkillCategoryData>(category_id.value());
		choice_struct.ranks = choice.second.get<int>("value");
		choice_struct.num_choices = choice.second.get<int>("num-choices", 1);
		boost::optional<const pt::ptree&> spell_lists_tree = choice.second.get_child_optional("options");
		if (spell_lists_tree) {
			for (const auto& spell_list : spell_lists_tree.get()) {
				std::string spell_list_id = spell_list.second.get_value<std::string>();
				choice_struct.spell_lists.emplace(&manager().get<SpellListData>(spell_list_id));
			}
		}
		choices.emplace(choice_struct);
	}

	return choices;
}

const pt::ptree TrainingPackageSerializer::getSpellListChoicesTree(TrainingPackageData& game_data) {
	pt::ptree choice_tree{};

	for (const auto& choice : game_data.spellListChoices()) {
		pt::ptree choice_struct_tree{};
		if (choice.spell_list_category)
			choice_struct_tree.put("optional-category", choice.spell_list_category.value()->id());
		choice_struct_tree.put("value", choice.ranks);
		choice_struct_tree.put("num-choices", choice.num_choices);

		// Sort the spell lists by id to ensure consistent ordering in the output json
		std::map<std::string, const SpellListData*> sorted_spell_lists{};
		for (const auto& spell_list : choice.spell_lists) {
			sorted_spell_lists.emplace(spell_list->id(), spell_list);
		}

		pt::ptree options_tree{};
		for (const auto& spell_list_pair : sorted_spell_lists) {
			pt::ptree option_tree{};
			option_tree.put("", spell_list_pair.second->id());
			options_tree.push_back(std::make_pair("", option_tree));
		}
		if (options_tree.size())
			choice_struct_tree.push_back(std::make_pair("options", options_tree));
		choice_tree.push_back(std::make_pair("", choice_struct_tree));
	}

	return choice_tree;
}

std::set<SpellListCategoryChoices> TrainingPackageSerializer::parseSpellListCategoryChoicesTree(boost::optional<const pt::ptree&> spell_list_category_choices) {
	std::set<SpellListCategoryChoices> choices{};
	if (!spell_list_category_choices)
		return choices;

	for (const auto& choice : spell_list_category_choices.get()) {
		SpellListCategoryChoices choice_struct{};
		choice_struct.ranks = choice.second.get<int>("value");
		choice_struct.num_choices = choice.second.get<int>("num-choices", 1);
		boost::optional<const pt::ptree&> spell_lists_tree = choice.second.get_child_optional("options");
		if (spell_lists_tree) {
			for (const auto& spell_list : spell_lists_tree.get()) {
				std::string spell_list_id = spell_list.second.get_value<std::string>();
				choice_struct.spell_list_categories.emplace(&manager().get<SkillCategoryData>(spell_list_id));
			}
		}
		choices.emplace(choice_struct);
	}
	return choices;
}

const pt::ptree TrainingPackageSerializer::getSpellListCategoryChoicesTree(TrainingPackageData& game_data) {
	pt::ptree choice_tree{};
	for (const auto& choice : game_data.spellListCategoryChoices()) {
		pt::ptree choice_struct_tree{};
		choice_struct_tree.put("value", choice.ranks);
		choice_struct_tree.put("num-choices", choice.num_choices);

		// Sort the spell list categories by id to ensure consistent ordering in the output json
		std::map<std::string, const SkillCategoryData*> sorted_spell_list_categories{};
		for (const auto& spell_list_category : choice.spell_list_categories) {
			sorted_spell_list_categories.emplace(spell_list_category->id(), spell_list_category);
		}

		pt::ptree options_tree{};
		for (const auto& spell_list_category_pair : sorted_spell_list_categories) {
			pt::ptree option_tree{};
			option_tree.put("", spell_list_category_pair.second->id());
			options_tree.push_back(std::make_pair("", option_tree));
		}
		if (options_tree.size())
			choice_struct_tree.push_back(std::make_pair("options", options_tree));
		choice_tree.push_back(std::make_pair("", choice_struct_tree));
	}
	return choice_tree;
}

std::vector<SkillGroupCategoryAndSkillRankChoice> TrainingPackageSerializer::parseSkillGroupCategoryAndSkillRankChoicesTree(boost::optional<const pt::ptree&> group_multi_skill_rank_choices) {
	std::vector<SkillGroupCategoryAndSkillRankChoice> choices{};
	if (!group_multi_skill_rank_choices)
		return choices;

	for (const auto& choice : group_multi_skill_rank_choices.get()) {
		SkillGroupCategoryAndSkillRankChoice choice_struct{};
		std::string group_id = choice.second.get<std::string>("id");
		choice_struct.group = &manager().get<SkillGroupData>(group_id);
		choice_struct.ranks = choice.second.get<int>("value");
		choices.emplace_back(choice_struct);
	}

	return choices;
}

const pt::ptree TrainingPackageSerializer::getSkillGroupCategoryAndSkillRankChoicesTree(TrainingPackageData& game_data) {
	pt::ptree choice_tree{};

	for (const auto& choice : game_data.skillGroupCategoryAndSkillRanks()) {
		pt::ptree choice_struct_tree{};
		choice_struct_tree.put("id", choice.group->id());
		choice_struct_tree.put("value", choice.ranks);
		choice_tree.push_back(std::make_pair("", choice_struct_tree));
	}

	return choice_tree;
}

} // namespace rm::rule::parser