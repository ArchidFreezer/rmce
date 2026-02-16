#include <EnumChoice.h>
#include <TrainingPackageDatafileParserJson.h>

void TrainingPackageDatafileParserJson::parse() {
	std::cout << "Loading TrainingPackage data ... ";

	// Get the Packages to parse and loop through them
	const pt::ptree& tree = ptree().get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", GameRuleData::generateId(ruleDatatype(), name));

		TrainingPackageData& ref = factory().get<TrainingPackageData>(id);
		ref.setName(name);
		ref.setDescription(v.second.get<std::string>("description"));

		boost::optional<std::string> flavour_text = v.second.get_optional<std::string>("flavour-text");
		if (flavour_text)	ref.setFlavourText(flavour_text.value());

		// Get the book from the cache
		std::string book_id = v.second.get<std::string>("book");
		ref.setBook(factory().get<BookData>(book_id));

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

		// Race restrictions
		ref.setRaces(parseGameDataSetTree<RaceData>(v.second.get_child_optional("races")));

		ref.setLifestyle(v.second.get<bool>("lifestyle", false));
		ref.setTimeToAcquire(v.second.get<int>("time-to-acquire", 0));

		// Starting money modifier dice
		boost::optional<std::string> starting_money_modifier_dice = v.second.get_optional<std::string>("starting-money-modifier-dice");
		if(starting_money_modifier_dice) ref.setStaringMoneyModifierDice(starting_money_modifier_dice.value());

		// Specials
		boost::optional<const pt::ptree&> specials_tree = v.second.get_child_optional("specials");
		if (specials_tree) ref.setSpecials(parseSpecials(specials_tree));

		// Stat gains
		// TODO - Fix this to read two tags, one for realm stat gain and one for normal stat gains, to avoid the need for the special case of "Realm" in the stat gains list
		boost::optional<const pt::ptree&> stat_gains = v.second.get_child_optional("stat-gains");
		if (stat_gains) {
			for (const auto& stat_gain : stat_gains.get()) {
				std::string stat_gain_str = stat_gain.second.get_value<std::string>();

				if (stat_gain_str == "Realm") {
					ref.setRealmStatGain(true);
					continue;
				}

				StatType::Type stat_gain_enum{};
				fromString(stat_gain_str, stat_gain_enum);
				ref.addStatGain(stat_gain_enum);
			}
		}

		// Stat gain choices
		// TODO - The original json had the choices as an array rather than a single object with a num-choices tag and an options array, so this is currently set up to read the original format. This should be changed to read the new format
		boost::optional<const pt::ptree&> stat_gain_choices_tree = v.second.get_child_optional("stat-gain-choices");
//		ref.setStatGainChoices(parseStatGainChoices(stat_gain_choices_tree));
		if (stat_gain_choices_tree) {
			std::set<EnumChoice<StatType::Type>> stat_gain_choices{ parseEnumChoiceSetTree<StatType::Type>(stat_gain_choices_tree)};
			ref.setStatGainChoices(*(stat_gain_choices.begin()));
		}

		// Skill ranks
		boost::optional<const pt::ptree&> skill_ranks_tree = v.second.get_child_optional("skill-ranks");
		if (skill_ranks_tree) { ref.setSkillRanks(parseSkillPairTree<int>(skill_ranks_tree)); }

		// Skill rank choices
		boost::optional<const pt::ptree&> skill_rank_choices_tree = v.second.get_child_optional("skill-rank-choices");
		if (skill_rank_choices_tree) { ref.setSkillRankChoices(parseSkillChoicePairTree<int>(skill_rank_choices_tree)); }

		// Skill category ranks
		boost::optional<const pt::ptree&> skill_category_ranks_tree = v.second.get_child_optional("category-ranks");
		if (skill_category_ranks_tree) { ref.setSkillCategoryRanks(parseGameDataPairTree<SkillCategoryData, int>(skill_category_ranks_tree)); }

		std::cout << "\tTrainingPackage name: " << ref.name() << std::endl;
	}

}

void TrainingPackageDatafileParserJson::populateDatum(std::string& id, pt::ptree& datum) {
	TrainingPackageData& game_data = factory().get<TrainingPackageData>(id);
	datum.put("id", game_data.id());
	datum.put("name", game_data.name());
	datum.put("description", game_data.description());
	if (!game_data.flavourText().empty())	datum.put("flavour-text", game_data.flavourText());
	datum.put("book", game_data.book().id());

	// Qualifiers
	{
		pt::ptree tree{ getQualifiersTree(game_data)};
		if (tree.size()) datum.push_back(std::make_pair("qualifiers", tree));
	}

	// Notes
	{
		pt::ptree tree{};
		for (const std::string& note : game_data.notes()) {
			pt::ptree note_tree{};
			note_tree.put("", note);
			tree.push_back(std::make_pair("", note_tree));
		}
		if (tree.size()) datum.push_back(std::make_pair("notes", tree));
	}

	// Races
	{
		pt::ptree tree{ getGameDataSetTree<RaceData>(game_data.races()) };
		if (tree.size()) datum.push_back(std::make_pair("races", tree));
	}

	// Lifestyle
	datum.put("lifestyle", game_data.lifestyle());
	// Time to acquire
	datum.put("time-to-acquire", game_data.timeToAcquire());
	// Starting money modifier dice
	if (!game_data.startingMoneyModifierDice().empty()) datum.put("starting-money-modifier-dice", game_data.startingMoneyModifierDice());

	// Specials
	{
		pt::ptree tree{ getSpecialsTree(game_data) };
		if (tree.size()) datum.push_back(std::make_pair("specials", tree));
	}

	// Stat gains
	{
		pt::ptree tree{getEnumSetTree<StatType::Type>(game_data.statGains())};
		if (tree.size()) datum.push_back(std::make_pair("stat-gains", tree));
	}

	// Realm stat gain
	if (game_data.realmStatGain()) datum.put("realm-stat-gain", true);

	// Stat gain choices
	{
		pt::ptree tree{ getStatGainChoicesTree(game_data) };
		if (tree.size()) datum.push_back(std::make_pair("stat-gain-choices", tree));
	}

	// Skill ranks
	{
		pt::ptree tree{ getSkillPairTree<int>(game_data.skillRanks()) };
		if (tree.size()) datum.push_back(std::make_pair("skill-ranks", tree));
	}

	// Skill rank choices
	{
		pt::ptree tree{ getSkillChoicePairTree<int>(game_data.skillRankChoices()) };
		if (tree.size()) datum.push_back(std::make_pair("skill-rank-choices", tree));
	}

	// Skill category ranks
	{
		pt::ptree tree{ getGameDataPairTree<SkillCategoryData ,int>(game_data.skillCategoryRanks()) };
		if (tree.size()) datum.push_back(std::make_pair("category-ranks", tree));
	}
}

std::map<std::string, int> TrainingPackageDatafileParserJson::parseQualifiers(boost::optional<const pt::ptree&> qualifiers) {
	std::map<std::string, int> quals;

	if (!qualifiers) return quals;

	for (const auto& v : qualifiers.get()) {
		std::string qualifier_text = v.second.get<std::string>("qualifier");
		int reduction = v.second.get<int>("reduction");
		quals.emplace(qualifier_text, reduction);
	}
	return quals;
}

const pt::ptree TrainingPackageDatafileParserJson::getQualifiersTree(TrainingPackageData& game_data) {
	pt::ptree tree{};
	for (const auto& [qualifier, reduction] : game_data.qualifiers()) {
		pt::ptree value_tree{};
		value_tree.put("qualifier", qualifier);
		value_tree.put("reduction", reduction);
		tree.push_back(std::make_pair("", value_tree));
	}
	return tree;
}

std::vector<std::pair<std::string, int>> TrainingPackageDatafileParserJson::parseSpecials(boost::optional<const pt::ptree&> specials) {
	std::vector<std::pair<std::string, int>> special_list;
	if (!specials) return special_list;

	for (const auto& v : specials.get()) {
		std::string special_text = v.second.get<std::string>("value");
		int chance = v.second.get<int>("chance");
		special_list.emplace_back(special_text, chance);
	}

	return special_list;
}

const pt::ptree TrainingPackageDatafileParserJson::getSpecialsTree(TrainingPackageData& game_data) {
	pt::ptree tree{};
	for (const auto& [special, reduction] : game_data.specials()) {
		pt::ptree value_tree{};
		value_tree.put("value", special);
		value_tree.put("chance", reduction);
		tree.push_back(std::make_pair("", value_tree));
	}
	return tree;
}

EnumChoice<StatType::Type> TrainingPackageDatafileParserJson::parseStatGainChoices(boost::optional<const pt::ptree&> stat_gain_choices) {
	EnumChoice<StatType::Type> choices{};
	if (!stat_gain_choices) return choices;

	choices.setNumChoices(stat_gain_choices.value().get<int>("num-choices"));
	for (const auto& option : stat_gain_choices.value().get_child("options")) {
		StatType::Type stat_gain_option_enum{};
		fromString(option.second.get_value<std::string>(), stat_gain_option_enum);
		choices.addOption(stat_gain_option_enum);
	}

	return choices;
}

const pt::ptree TrainingPackageDatafileParserJson::getStatGainChoicesTree(TrainingPackageData& game_data) {
	pt::ptree choice_tree{};
	EnumChoice<StatType::Type> stat_gain_choices = game_data.statGainChoices();
	if (!stat_gain_choices.numChoices()) return choice_tree;

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