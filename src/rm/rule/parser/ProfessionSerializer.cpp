#include <GameRuleBoostConverter.h>
#include <ProfessionSerializer.h>

namespace rm::rule::parser {

void ProfessionSerializer::parse(pt::ptree& ptree) {
	std::cout << "Loading Profession data ..." << std::endl;

	// Get the professions to parse and loop through them
	const pt::ptree& tree = ptree.get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", manager().generateId(ruleDatatype(), name));

		// Get a ProfessionData object from the object manager
		ProfessionData& ref = manager().get<ProfessionData>(id);
		ref.setName(name);
		ref.setDescription(v.second.get<std::string>("description"));
		ref.setSpellUserType(SpellUserType::fromString(v.second.get<std::string>("spell-user-type")).value());

		// Get the book from the cache
		std::string book_id = v.second.get<std::string>("book");
		ref.setBook(manager().get<BookData>(book_id));

		GameRuleBoostConverter converter{manager()};

		// Get the spell power realms
		ref.setRealms(converter.parseEnumSetTree<RealmType::Type>(v.second.get_child("realms")));

		// Get the primary stats
		ref.setStats(converter.parseEnumVectorTree<StatType::Type>(v.second.get_child("stats")));

		// Get the base spell list choices
		ref.setBaseSpellListChoices(converter.parseGameDataChoiceSetTree<SpellListData>(v.second.get_child_optional("base-spell-list-choices")));

		// Get skill bonuses
		ref.setSkillBonuses(converter.parseSkillPairTree<int>(v.second.get_child_optional("skill-bonuses")));

		// Skill development types
		ref.setSkillDevelopmentTypes(converter.parseSkillPairEnumTree<SkillDevelopmentType::Type>(v.second.get_child_optional("skill-development-types")));

		// Skill development type choices
		ref.setSkillDevelopmentTypeChoices(converter.parseSkillChoicePairEnumTree<SkillDevelopmentType::Type>(v.second.get_child_optional("skill-development-type-choices")));

		// Skill subcategory development type choices
		ref.setSkillSubcategoryDevelopmentTypeChoices(converter.parseGameDataChoicePairEnumTree<SkillData, SkillDevelopmentType::Type>(v.second.get_child_optional("skill-subcategory-development-type-choices")));

		// Skill category profession bonus
		ref.setSkillCategoryProfessionBonuses(converter.parseGameDataPairTree<SkillCategoryData, int>(v.second.get_child_optional("skill-category-profession-bonuses")));

		// Skill category special bonus
		ref.setSkillCategorySpecialBonuses(converter.parseGameDataPairTree<SkillCategoryData, int>(v.second.get_child_optional("skill-category-special-bonuses")));

		// Skill category development types
		ref.setSkillCategorySkillDevelopmentTypes(converter.parseGameDataPairEnumTree<SkillCategoryData, SkillDevelopmentType::Type>(v.second.get_child_optional("skill-category-skill-development-types")));

		// Skill category development type choices
		ref.setSkillCategorySkillDevelopmentTypeChoices(converter.parseGameDataChoicePairEnumTree<SkillCategoryData, SkillDevelopmentType::Type>(v.second.get_child_optional("skill-category-skill-development-type-choices")));

		// Skill category costs
		if (boost::optional<const pt::ptree&> category_costs = v.second.get_child_optional("skill-category-costs")) {
			for (const auto& category_cost : category_costs.get()) {
				std::string category_id{category_cost.second.get<std::string>("category")};
				std::string cost{category_cost.second.get<std::string>("cost")};
				ref.addSkillCategoryDevelopmentCost(manager().get<SkillCategoryData>(category_id), std::move(rm::game::character::SkillDevelopmentCost(cost)));
			}
		}

		// Skill group profession bonus
		ref.setSkillGroupProfessionBonuses(converter.parseGameDataPairTree<SkillGroupData, int>(v.second.get_child_optional("skill-group-profession-bonuses")));

		// Skill group special bonus
		ref.setSkillGroupSpecialBonuses(converter.parseGameDataPairTree<SkillGroupData, int>(v.second.get_child_optional("skill-group-special-bonuses")));

		// Skill group skill development types
		ref.setSkillGroupSkillDevelopmentTypes(converter.parseGameDataPairEnumTree<SkillGroupData, SkillDevelopmentType::Type>(v.second.get_child_optional("skill-group-skill-development-types")));

		// Skill group development type choices
		ref.setSkillGroupSkillDevelopmentTypeChoices(converter.parseGameDataChoicePairEnumTree<SkillGroupData, SkillDevelopmentType::Type>(v.second.get_child_optional("skill-group-skill-development-type-choices")));

		std::cout << "\tProfession name: " << ref.name() << std::endl;
	}
	std::cout << " done" << std::endl;
}

void ProfessionSerializer::populateDatum(std::string key, pt::ptree& datum) {
	ProfessionData& game_data = manager().get<ProfessionData>(key);
	datum.put("id", game_data.id());
	datum.put("name", game_data.name());
	datum.put("description", game_data.description());
	datum.put("book", game_data.book().value()->id());
	datum.put("spell-user-type", SpellUserType::toString(game_data.spellUserType()));

	GameRuleBoostConverter converter{manager()};

	// Realms
	{
		pt::ptree tree{converter.getEnumSetTree<RealmType::Type>(game_data.realms())};
		if (tree.size())
			datum.push_back(std::make_pair("realms", tree));
	}

	// Stats
	{
		pt::ptree tree{converter.getEnumVectorTree<StatType::Type>(game_data.stats())};
		if (tree.size())
			datum.push_back(std::make_pair("stats", tree));
	}

	// Base spell list choices
	{
		pt::ptree tree{converter.getGameDataChoiceSetTree<SpellListData>(game_data.baseSpellListChoices())};
		if (tree.size())
			datum.push_back(std::make_pair("base-spell-list-choices", tree));
	}

	// Skill bonuses
	{
		pt::ptree tree{converter.getSkillPairTree<int>(game_data.skillBonuses())};
		if (tree.size())
			datum.push_back(std::make_pair("skill-bonuses", tree));
	}

	// Skill development types
	{
		pt::ptree tree{converter.getSkillPairEnumTree<SkillDevelopmentType::Type>(game_data.skillDevelopmentTypes())};
		if (tree.size())
			datum.push_back(std::make_pair("skill-development-types", tree));
	}

	// Skill development type choices
	{
		pt::ptree tree{converter.getSkillChoicePairEnumTree<SkillDevelopmentType::Type>(game_data.skillDevelopmentTypeChoices())};
		if (tree.size())
			datum.push_back(std::make_pair("skill-development-type-choices", tree));
	}

	// Skill subcategory development type choices
	{
		pt::ptree tree{converter.getGameDataChoicePairEnumTree<SkillData, SkillDevelopmentType::Type>(game_data.skillSubcategoryDevelopmentTypeChoices())};
		if (tree.size())
			datum.push_back(std::make_pair("skill-subcategory-development-type-choices", tree));
	}

	// Skill category profession bonus
	{
		pt::ptree tree{converter.getGameDataPairTree<SkillCategoryData, int>(game_data.skillCategoryProfessionBonuses())};
		if (tree.size())
			datum.push_back(std::make_pair("skill-category-profession-bonuses", tree));
	}

	// Skill category special bonus
	{
		pt::ptree tree{converter.getGameDataPairTree<SkillCategoryData, int>(game_data.skillCategorySpecialBonuses())};
		if (tree.size())
			datum.push_back(std::make_pair("skill-category-special-bonuses", tree));
	}

	// Skill category development types
	{
		pt::ptree tree{converter.getGameDataPairEnumTree<SkillCategoryData, SkillDevelopmentType::Type>(game_data.skillCategorySkillDevelopmentTypes())};
		if (tree.size())
			datum.push_back(std::make_pair("skill-category-skill-development-types", tree));
	}

	// Skill category development type choices
	{
		pt::ptree tree{converter.getGameDataChoicePairEnumTree<SkillCategoryData, SkillDevelopmentType::Type>(game_data.skillCategorySkillDevelopmentTypeChoices())};
		if (tree.size())
			datum.push_back(std::make_pair("skill-category-skill-development-type-choices", tree));
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
		if (skill_category_costs_tree.size())
			datum.push_back(std::make_pair("skill-category-costs", skill_category_costs_tree));
	}

	// Skill group profession bonus
	{
		pt::ptree tree{converter.getGameDataPairTree<SkillGroupData, int>(game_data.skillGroupProfessionBonuses())};
		if (tree.size())
			datum.push_back(std::make_pair("skill-group-profession-bonuses", tree));
	}

	// Skill group special bonus
	{
		pt::ptree tree{converter.getGameDataPairTree<SkillGroupData, int>(game_data.skillGroupSpecialBonuses())};
		if (tree.size())
			datum.push_back(std::make_pair("skill-group-special-bonuses", tree));
	}

	// Skill group skill development types
	{
		pt::ptree tree{converter.getGameDataPairEnumTree<SkillGroupData, SkillDevelopmentType::Type>(game_data.skillGroupSkillDevelopmentTypes())};
		if (tree.size())
			datum.push_back(std::make_pair("skill-group-skill-development-types", tree));
	}

	// Skill group development type choices
	{
		pt::ptree tree{converter.getGameDataChoicePairEnumTree<SkillGroupData, SkillDevelopmentType::Type>(game_data.skillGroupSkillDevelopmentTypeChoices())};
		if (tree.size())
			datum.push_back(std::make_pair("skill-group-skill-development-type-choices", tree));
	}
}

std::string ProfessionSerializer::rootNode() const {
	return "professions";
}

} // namespace rm::rule::parser