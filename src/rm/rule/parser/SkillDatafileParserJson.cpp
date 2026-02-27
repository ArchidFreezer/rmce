#include <SkillDatafileParserJson.h>
#include <StatType.h>

namespace rm::rule::parser {

	void SkillDatafileParserJson::parse() {
		std::cout << "Loading Skill data ... ";

		// Get the lists to parse and loop through them
		const pt::ptree& tree = ptree().get_child(rootNode());
		for (const auto& v : tree) {
			std::string name = v.second.get<std::string>("name");
			std::string id = v.second.get("id", GameRuleData::generateId(ruleDatatype(), name));

			SkillData& ref = factory().get<SkillData>(id);
			ref.setName(name);
			ref.setDescription(v.second.get<std::string>("description"));
			ref.setDifficultySummary(v.second.get<std::string>("difficulties-summary"));
			ref.setNotes(v.second.get<std::string>("notes"));
			ref.setIsRestricted(v.second.get<bool>("is-restricted"));
			ref.setCanSpecialise(v.second.get<bool>("can-specialise"));
			ref.setMandatorySubcategory(v.second.get<bool>("mandatory-subcategory"));
			ref.setExhaustionCost(v.second.get<float>("exhaustion"));
			ref.setDistanceMultiplier(v.second.get<float>("distance-multiplier"));

			// Get the book from the cache
			std::string book = v.second.get<std::string>("book");
			ref.setBook(factory().get<BookData>(book));

			// Get the skill action type
			SkillActionType::Type action_type{};
			fromString(v.second.get<std::string>("action"), action_type);
			ref.setActionType(action_type);

			// Get the skill category from the cache
			std::string category = v.second.get<std::string>("category");
			ref.setCategory(factory().get<SkillCategoryData>(category));

			// Get the stats if any are defined
			ref.setStats(parseEnumVectorTree<StatType::Type>(v.second.get_child_optional("stats")));

			// Get the Subcategories if any are defined
			ref.setSubcategories(parseSetTree<std::string>(v.second.get_child_optional("subcategories")));

			std::cout << "\tSkill name: " << ref.name() << std::endl;
		}
	}

	void SkillDatafileParserJson::populateDatum(std::string& id, pt::ptree& datum) {
		SkillData& game_data = factory().get<SkillData>(id);

		datum.put("id", game_data.id());
		datum.put("name", game_data.name());
		datum.put("category", game_data.category().id());
		datum.put("description", game_data.description());
		datum.put("book", game_data.book().id());
		datum.put("action", toString(game_data.actionType()));
		datum.put("difficulties-summary", game_data.difficultySummary());
		datum.put("notes", game_data.notes());
		datum.put("is-restricted", game_data.restricted());
		datum.put("can-specialise", game_data.canSpecialise());
		datum.put("mandatory-subcategory", game_data.mandatorySubcategory());

		// Subcategories
		{
			pt::ptree tree{ getSetTree<std::string>(game_data.subcategories()) };
			if (!tree.empty()) datum.push_back(std::make_pair("subcategories", tree));
		}

		// Stats
		{
			pt::ptree tree{ getEnumVectorTree<StatType::Type>(game_data.stats()) };
			if (tree.size()) datum.push_back(std::make_pair("stats", tree));
		}

		datum.put("exhaustion", game_data.exhaustionCost());
		datum.put("distance-multiplier", game_data.distanceMultiplier());
	}

} // namespace rm::rule::parser