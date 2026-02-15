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
		if (tree.size()) datum.push_back(std::make_pair("games", tree));
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