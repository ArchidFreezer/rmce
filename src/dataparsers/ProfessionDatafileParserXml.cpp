#include <ProfessionData.h>
#include <ProfessionDatafileParserXml.h>
#include <SkillDevelopmentType.h>

void ProfessionDatafileParserXml::parse(bool id_only) {
	std::cout << "Loading Profession data ... ";
	std::cout << (id_only ? "[Pass 1]" : "[Pass 2]") << std::endl;

	int count{ 0 };

	// Get the lists to parse and loop through them
	const pt::ptree& tree = ptree().get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", GameRuleData::generateId(ruleDatatype(), name));

		if (id_only) {
			// We create a Data object and reference it with as a unique_ptr to allow us to use move semantics to transfer ownership
			// to the cache when we add it
			std::unique_ptr<ProfessionData> datum = std::make_unique<ProfessionData>(id);
			cache().add<ProfessionData>(std::move(datum), id);
		} else {
			ProfessionData& ref = cache().get<ProfessionData>(id);
			ref.setName(name);
			ref.setDescription(v.second.get<std::string>("description"));

			// Get the book from the cache
			std::string book_id = GameRuleData::generateId("Book", v.second.get<std::string>("book"));
			ref.setBook(cache().get<BookData>(book_id));

			// Get the power realms
			for (const auto& realms_tree : v.second.get_child("realms")) {
				std::string realm_name = realms_tree.second.get_value<std::string>();
				ref.addRealm(RealmType::fromString(realm_name).value());
			}

			// Get the prime stats
			for (const auto& stats_tree : v.second.get_child("stats")) {
				std::string stat_name = stats_tree.second.get_value<std::string>();
				ref.addStat(StatType::fromString(stat_name).value());
			}

			// Get base spell lists if any exist
			if (boost::optional<const pt::ptree&> base_spell_lists = v.second.get_child_optional("base-spell-lists")) {
				for (const auto& base_spell_list : base_spell_lists.get()) {
					GameRuleDataChoice<SpellListData> base_lists{};
					base_lists.setNumChoices(6);
					for (const auto& element : base_spell_list.second) {
						std::string key = element.first;
						int num_choices{};
						if (key == "num-choices") {
							num_choices = element.second.get_value<int>();
							if (num_choices != -1) base_lists.setNumChoices(num_choices);
						}
						if (key == "spell-list") {
							std::string list_id = GameRuleData::generateId("SpellList", element.second.get_value<std::string>());
							base_lists.addOption(cache().get<SpellListData>(list_id));
						}
					}
					ref.addBaseSpellListChoice(std::move(base_lists));
				}
			}

			// Skill Bonuses
			if (boost::optional<const pt::ptree&> skill_bonuses = v.second.get_child_optional("skill-bonuses")) {
				for (const auto& skill_bonus : skill_bonuses.get()) {
					// Get development type to set the skill as and then add the skills to the appropriate container
					int bonus = skill_bonus.second.get_value<int>();
					std::string skill_id = GameRuleData::generateId("Skill", skill_bonus.second.get<std::string>("<xmlattr>.skill"));
					boost::optional<std::string> subcategory = skill_bonus.second.get_optional<std::string>("<xmlattr>.subcategory");
					if (subcategory) {
						ref.setSkillBonus(SubcategoriedSkillData(cache().get<SkillData>(skill_id), subcategory.value()), bonus);
					} else {
						ref.setSkillBonus(SubcategoriedSkillData(cache().get<SkillData>(skill_id)), bonus);
					}
				}
			}

			// Skill development types
			if (boost::optional<const pt::ptree&> skill_modifiers = v.second.get_child_optional("skill-modifiers")) {
				for (const auto& skill_modifier : skill_modifiers.get()) {
					// The tag hold multiple types of modifiers so ignore any we are not interested in
					if( skill_modifier.second.get <int> ("num-choices") != -1) continue;
					std::string skill_type_id = skill_modifier.second.get<std::string>("skill-type");
					if (SkillDevelopmentType::fromString(skill_type_id)) {
						for (const auto& skill_tree : skill_modifier.second.get_child("skills")) {
							std::string skill_id = GameRuleData::generateId("Skill", skill_tree.second.get_value<std::string>());
							ref.setSkillDevelopmentType(SubcategoriedSkillData(cache().get<SkillData>(skill_id)), SkillDevelopmentType::fromString(skill_type_id).value());
						}
					}
				}
			}

			// Get skill category bonuses
			if (boost::optional<const pt::ptree&> skill_category_bonuses = v.second.get_child_optional("skill-category-bonuses")) {
				for (const auto& skill_category_bonus : skill_category_bonuses.get()) {
					std::string skill_category_id{ GameRuleData::generateId("SkillCategory", skill_category_bonus.second.get<std::string>("<xmlattr>.category")) };
					int bonus = skill_category_bonus.second.get_value<int>();
					if (bonus) ref.addSkillCategoryBonus(cache().get<SkillCategoryData>(skill_category_id), bonus);
				}
			}

			// Get skill category development types
			if (boost::optional<const pt::ptree&> skill_category_development_types = v.second.get_child_optional("skill-category-skill-modifiers")) {
				for (const auto& skill_category_development_type : skill_category_development_types.get()) {
					std::string skill_category_id{ GameRuleData::generateId("SkillCategory", skill_category_development_type.second.get<std::string>("<xmlattr>.category")) };
					std::string skill_type_id = skill_category_development_type.second.get_value<std::string>();
					if (SkillDevelopmentType::fromString(skill_type_id)) {
						ref.addSkillCategorySkillDevelopmentType(cache().get<SkillCategoryData>(skill_category_id), SkillDevelopmentType::fromString(skill_type_id).value());
					}
				}
			}

			// Get skill group bonuses
			if (boost::optional<const pt::ptree&> skill_group_bonuses = v.second.get_child_optional("skill-group-bonuses")) {
				for (const auto& skill_group_bonus : skill_group_bonuses.get()) {
					int bonus = skill_group_bonus.second.get_value<int>();
					if (bonus) ref.addSkillGroupBonus(skill_group_bonus.second.get<std::string>("<xmlattr>.group"), bonus);
				}
			}

			// Skill group skill development types
			if (boost::optional<const pt::ptree&> skill_group_skill_modifiers = v.second.get_child_optional("skill-group-skill-modifiers")) {
				for (const auto& skill_group_skill_modifier : skill_group_skill_modifiers.get()) {
					std::string skill_type_id = skill_group_skill_modifier.second.get_value<std::string>();
					if (SkillDevelopmentType::fromString(skill_type_id)) {
						ref.addSkillGroupSkillDevelopmentType(skill_group_skill_modifier.second.get<std::string>("<xmlattr>.group"), SkillDevelopmentType::fromString(skill_type_id).value());
					}
				}
			}

			std::cout << "\tProfession name: " << ref.name() << std::endl;
		}
	}
}