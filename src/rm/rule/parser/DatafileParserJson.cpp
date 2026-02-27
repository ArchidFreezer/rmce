#include <DatafileParserJson.h>

namespace rm::rule::parser {

	void DatafileParserJson::read(const std::string& filename) {

		if (filename.empty()) throw FilenameNotSetException("You are attempting to read file a file without setting the filename first.");

		try {
			// Read the file and place the contents into the boost ptree if it is not already populated
			if (ptree().size() < 1)	pt::read_json(filename, ptree());

			// Call the virtual method to parse the ptree into data objects
			parse();
		} catch (const pt::json_parser::json_parser_error& err) {
			std::cerr << err.what();
		}

	}

	std::set<SubcategoriedSkillData> DatafileParserJson::parseSkillSetTree(boost::optional<const pt::ptree&> tree) {
		std::set<SubcategoriedSkillData> datum{};
		if (tree) {
			for (const auto& items : tree.get()) {
				std::string id{ items.second.get<std::string>("id") };
				boost::optional<std::string> subcategory = items.second.get_optional<std::string>("subcategory");
				if (subcategory) {
					datum.insert(factory().subcategoriedSkillData(id, subcategory.get()));
				} else {
					datum.insert(factory().subcategoriedSkillData(id));
				}
			}
		}
		return datum;
	}

	const pt::ptree DatafileParserJson::getSkillSetTree(std::set<SubcategoriedSkillData> set) {
		pt::ptree tree{};
		for (const auto& item : set) {
			pt::ptree value_tree{};
			value_tree.put("id", item.skillData().id());
			if (item.subcategory()) value_tree.put("subcategory", item.subcategory().value());
			tree.push_back(std::make_pair("", value_tree));
		}
		return tree;
	}

	std::map<std::string, const LanguageAbility> DatafileParserJson::parseLanguageAbilityMapTree(boost::optional<const pt::ptree&> tree) {
		std::map<std::string, const LanguageAbility> datum{};
		if (tree) {
			for (const auto& items : tree.get()) {
				std::string language_name{ items.second.get<std::string>("language") };
				LanguageAbility ability(factory().get<LanguageData>(language_name));
				boost::optional<int> somantic = items.second.get_optional<int>("somantic");
				if (somantic) { ability.updateSomanticRanks(somantic.get()); }
				boost::optional<int> spoken = items.second.get_optional<int>("spoken");
				if (spoken) { ability.updateSpokenRanks(spoken.get()); }
				boost::optional<int> written = items.second.get_optional<int>("written");
				if (written) { ability.updateWrittenRanks(written.get()); }
				datum.emplace(language_name, ability);
			}
		}
		return datum;
	}

	const pt::ptree DatafileParserJson::getLanguageAbilityMapTree(std::map<std::string, const LanguageAbility> map) {
		pt::ptree tree{};
		for (const auto& pair : map) {
			pt::ptree value_tree{};
			value_tree.put("language", pair.second.languageId());
			if (pair.second.somantic()) value_tree.put("somantic", pair.second.somantic());
			if (pair.second.spoken()) value_tree.put("spoken", pair.second.spoken());
			if (pair.second.written()) value_tree.put("written", pair.second.written());
			tree.push_back(std::make_pair("", value_tree));
		}
		return tree;
	}

	std::set<GameRuleDataChoice<SubcategoriedSkillData>> DatafileParserJson::parseSkillChoiceSetTree(boost::optional<const pt::ptree&> tree) {
		std::set<GameRuleDataChoice<SubcategoriedSkillData>> datum{};
		if (tree) {
			for (const auto& choice_tree : tree.get()) {
				GameRuleDataChoice<SubcategoriedSkillData> choice_data{};
				choice_data.setNumChoices(choice_tree.second.get<int>("num-choices"));

				for (const auto& options_tree : choice_tree.second.get_child("options")) {
					std::string skill_id{ options_tree.second.get<std::string>("id") };
					boost::optional<std::string> subcategory = options_tree.second.get_optional<std::string>("subcategory");
					if (subcategory) {
						choice_data.addOption(factory().subcategoriedSkillData(skill_id, subcategory.get()));
					} else {
						choice_data.addOption(factory().subcategoriedSkillData(skill_id));
					}
				}
				datum.emplace(choice_data);
			}
		}
		return datum;
	}

	const pt::ptree DatafileParserJson::getSkillChoiceSetTree(std::set<GameRuleDataChoice<SubcategoriedSkillData>> set) {
		pt::ptree tree{};
		for (const GameRuleDataChoice<SubcategoriedSkillData>& item : set) {
			pt::ptree choice_tree{};
			choice_tree.put("num-choices", item.numChoices());
			pt::ptree options_tree{};

			std::map<std::string, const SubcategoriedSkillData*> sorted_options{};
			for (const SubcategoriedSkillData* option : item.options()) {
				sorted_options.emplace(option->id(), option);
			}

			for (const auto& pair : sorted_options) {
				pt::ptree option_tree{};
				option_tree.put("id", pair.second->id());
				if (pair.second->subcategory()) option_tree.put("subcategory", pair.second->subcategory().value());
				options_tree.push_back(std::make_pair("", option_tree));
			}
			choice_tree.push_back(std::make_pair("options", options_tree));
			tree.push_back(std::make_pair("", choice_tree));
		}
		return tree;
	}

} // namespace rm::rule::parser