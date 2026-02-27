#include <ProfessionDatefileParserJson.h>

namespace rm::rule::parser {

	void ProfessionDatafileParserJson::parse() {
		std::cout << "Loading Profession data ... ";

		// Get the lists to parse and loop through them
		const pt::ptree& tree = ptree().get_child(rootNode());
		for (const auto& v : tree) {
			std::string name = v.second.get<std::string>("name");
			std::string id = v.second.get("id", GameRuleData::generateId(ruleDatatype(), name));

			ProfessionData& ref = factory().get<ProfessionData>(id);
			ref.setName(name);
			ref.setDescription(v.second.get<std::string>("description"));
			ref.setSpellUserType(rule::enums::SpellUserType::fromString(v.second.get<std::string>("spell-user-type")).value());

			// Get the book from the cache
			std::string book_id = v.second.get<std::string>("book");
			ref.setBook(factory().get<BookData>(book_id));

			// Get the spell power realms
			ref.setRealms(parseEnumSetTree<rule::enums::RealmType::Type>(v.second.get_child("realms")));

			// Get the primary stats
			ref.setStats(parseEnumVectorTree<rule::enums::StatType::Type>(v.second.get_child("stats")));

			// Get the base spell list choices
			ref.setBaseSpellListChoices(parseGameDataChoiceSetTree<SpellListData>(v.second.get_child_optional("base-spell-list-choices")));

			// Get skill bonuses
			ref.setSkillBonuses(parseSkillPairTree<int>(v.second.get_child_optional("skill-bonuses")));

			// Skill development types
			ref.setSkillDevelopmentTypes(parseSkillPairEnumTree<rule::enums::SkillDevelopmentType::Type>(v.second.get_child_optional("skill-development-types")));

			// Skill development type choices
			ref.setSkillDevelopmentTypeChoices(parseSkillChoicePairEnumTree<rule::enums::SkillDevelopmentType::Type>(v.second.get_child_optional("skill-development-type-choices")));

			// Skill subcategory development type choices
			ref.setSkillSubcategoryDevelopmentTypeChoices(parseGameDataChoicePairEnumTree<SkillData, rule::enums::SkillDevelopmentType::Type>(v.second.get_child_optional("skill-subcategory-development-type-choices")));

			// Skill category profession bonus
			ref.setSkillCategoryProfessionBonuses(parseGameDataPairTree<SkillCategoryData, int>(v.second.get_child_optional("skill-category-profession-bonuses")));

			// Skill category special bonus
			ref.setSkillCategorySpecialBonuses(parseGameDataPairTree<SkillCategoryData, int>(v.second.get_child_optional("skill-category-special-bonuses")));

			// Skill category development types
			ref.setSkillCategorySkillDevelopmentTypes(parseGameDataPairEnumTree<SkillCategoryData, rule::enums::SkillDevelopmentType::Type>(v.second.get_child_optional("skill-category-skill-development-types")));

			// Skill category development type choices
			ref.setSkillCategorySkillDevelopmentTypeChoices(parseGameDataChoicePairEnumTree<SkillCategoryData, rule::enums::SkillDevelopmentType::Type>(v.second.get_child_optional("skill-category-skill-development-type-choices")));

			// Skill category costs
			if (boost::optional<const pt::ptree&> category_costs = v.second.get_child_optional("skill-category-costs")) {
				for (const auto& category_cost : category_costs.get()) {
					std::string category_id{ category_cost.second.get<std::string>("category") };
					std::string cost{ category_cost.second.get<std::string>("cost") };
					ref.addSkillCategoryDevelopmentCost(factory().get<SkillCategoryData>(category_id), std::move(SkillDevelopmentCost(cost)));
				}
			}

			// Skill group profession bonus
			ref.setSkillGroupProfessionBonuses(parseGameDataPairTree<SkillGroupData, int>(v.second.get_child_optional("skill-group-profession-bonuses")));

			// Skill group special bonus
			ref.setSkillGroupSpecialBonuses(parseGameDataPairTree<SkillGroupData, int>(v.second.get_child_optional("skill-group-special-bonuses")));

			// Skill group skill development types
			ref.setSkillGroupSkillDevelopmentTypes(parseGameDataPairEnumTree<SkillGroupData, rule::enums::SkillDevelopmentType::Type>(v.second.get_child_optional("skill-group-skill-development-types")));

			// Skill group development type choices
			ref.setSkillGroupSkillDevelopmentTypeChoices(parseGameDataChoicePairEnumTree<SkillGroupData, rule::enums::SkillDevelopmentType::Type>(v.second.get_child_optional("skill-group-skill-development-type-choices")));

			std::cout << "\tProfession name: " << ref.name() << std::endl;
		}

	}

	void ProfessionDatafileParserJson::populateDatum(std::string& id, pt::ptree& datum) {
		ProfessionData& game_data = factory().get<ProfessionData>(id);
		datum.put("id", game_data.id());
		datum.put("name", game_data.name());
		datum.put("description", game_data.description());
		datum.put("book", game_data.book().value()->id());
		datum.put("spell-user-type", rule::enums::SpellUserType::toString(game_data.spellUserType()));

		// Realms
		{
			pt::ptree tree{ getEnumSetTree <rule::enums::RealmType::Type>(game_data.realms()) };
			if (tree.size()) datum.push_back(std::make_pair("realms", tree));
		}

		// Stats
		{
			pt::ptree tree{ getEnumVectorTree<rule::enums::StatType::Type>(game_data.stats()) };
			if (tree.size()) datum.push_back(std::make_pair("stats", tree));
		}

		// Base spell list choices
		{
			pt::ptree tree{ getGameDataChoiceSetTree<SpellListData>(game_data.baseSpellListChoices()) };
			if (tree.size()) datum.push_back(std::make_pair("base-spell-list-choices", tree));
		}

		// Skill bonuses
		{
			pt::ptree tree{ getSkillPairTree<int>(game_data.skillBonuses()) };
			if (tree.size()) datum.push_back(std::make_pair("skill-bonuses", tree));
		}

		// Skill development types
		{
			pt::ptree tree{ getSkillPairEnumTree<rule::enums::SkillDevelopmentType::Type>(game_data.skillDevelopmentTypes()) };
			if (tree.size()) datum.push_back(std::make_pair("skill-development-types", tree));
		}

		// Skill development type choices
		{
			pt::ptree tree{ getSkillChoicePairEnumTree <rule::enums::SkillDevelopmentType::Type>(game_data.skillDevelopmentTypeChoices()) };
			if (tree.size()) datum.push_back(std::make_pair("skill-development-type-choices", tree));
		}

		// Skill subcategory development type choices
		{
			pt::ptree tree{ getGameDataChoicePairEnumTree <SkillData, rule::enums::SkillDevelopmentType::Type>(game_data.skillSubcategoryDevelopmentTypeChoices()) };
			if (tree.size()) datum.push_back(std::make_pair("skill-subcategory-development-type-choices", tree));
		}

		// Skill category profession bonus
		{
			pt::ptree tree{ getGameDataPairTree<SkillCategoryData, int>(game_data.skillCategoryProfessionBonuses()) };
			if (tree.size()) datum.push_back(std::make_pair("skill-category-profession-bonuses", tree));
		}

		// Skill category special bonus
		{
			pt::ptree tree{ getGameDataPairTree<SkillCategoryData, int>(game_data.skillCategorySpecialBonuses()) };
			if (tree.size()) datum.push_back(std::make_pair("skill-category-special-bonuses", tree));
		}

		// Skill category development types
		{
			pt::ptree tree{ getGameDataPairEnumTree<SkillCategoryData, rule::enums::SkillDevelopmentType::Type>(game_data.skillCategorySkillDevelopmentTypes()) };
			if (tree.size()) datum.push_back(std::make_pair("skill-category-skill-development-types", tree));
		}

		// Skill category development type choices
		{
			pt::ptree tree{ getGameDataChoicePairEnumTree <SkillCategoryData, rule::enums::SkillDevelopmentType::Type>(game_data.skillCategorySkillDevelopmentTypeChoices()) };
			if (tree.size()) datum.push_back(std::make_pair("skill-category-skill-development-type-choices", tree));
		}

		// Skill category costs
		{
			pt::ptree skill_category_costs_tree{};
			std::map<std::string, const SkillCategoryData*> categories{};
			for (auto& skill_category : game_data.skillCategoriesWithCost()) {
				categories.emplace(skill_category->id(), skill_category);
			}
			for (const auto& skill_category : categories) {
				pt::ptree skill_category_cost_tree{};
				skill_category_cost_tree.put("category", skill_category.first);
				skill_category_cost_tree.put("cost", game_data.skillCategoryDevelopmentCost(*skill_category.second).toString());
				skill_category_costs_tree.push_back(std::make_pair("", skill_category_cost_tree));
			}
			if (skill_category_costs_tree.size()) datum.push_back(std::make_pair("skill-category-costs", skill_category_costs_tree));
		}

		// Skill group profession bonus
		{
			pt::ptree tree{ getGameDataPairTree<SkillGroupData, int>(game_data.skillGroupProfessionBonuses()) };
			if (tree.size()) datum.push_back(std::make_pair("skill-group-profession-bonuses", tree));
		}

		// Skill group special bonus
		{
			pt::ptree tree{ getGameDataPairTree<SkillGroupData, int>(game_data.skillGroupSpecialBonuses()) };
			if (tree.size()) datum.push_back(std::make_pair("skill-group-special-bonuses", tree));
		}

		// Skill group skill development types
		{
			pt::ptree tree{ getGameDataPairEnumTree<SkillGroupData, rule::enums::SkillDevelopmentType::Type>(game_data.skillGroupSkillDevelopmentTypes()) };
			if (tree.size()) datum.push_back(std::make_pair("skill-group-skill-development-types", tree));
		}

		// Skill group development type choices
		{
			pt::ptree tree{ getGameDataChoicePairEnumTree <SkillGroupData, rule::enums::SkillDevelopmentType::Type>(game_data.skillGroupSkillDevelopmentTypeChoices()) };
			if (tree.size()) datum.push_back(std::make_pair("skill-group-skill-development-type-choices", tree));
		}

	}

} // namespace rm::rule::parser