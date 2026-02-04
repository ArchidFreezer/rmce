#include <ProfessionData.h>
#include <ProfessionDatafileParserXml.h>
#include <SkillDevelopmentType.h>

void ProfessionDatafileParserXml::parse() {
	std::cout << "Loading Profession data ... ";

	// Get the lists to parse and loop through them
	const pt::ptree& tree = ptree().get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", GameRuleData::generateId(ruleDatatype(), name));

			ProfessionData& ref = factory().get<ProfessionData>(id);
			ref.setName(name);
			ref.setDescription(v.second.get<std::string>("description"));

			// Get the book from the cache
			std::string book_id = GameRuleData::generateId("Book", v.second.get<std::string>("book"));
			ref.setBook(factory().get<BookData>(book_id));

			// Spell user type
			std::string spell_user_type = v.second.get<std::string>("spell-user-type");
			ref.setSpellUserType(SpellUserType::fromString(spell_user_type).value());
			
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
							base_lists.addOption(factory().get<SpellListData>(list_id));
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
						ref.setSkillBonus(factory().subcategoriedSkillData(skill_id, subcategory.value()), bonus);
					} else {
						ref.setSkillBonus(factory().subcategoriedSkillData(skill_id), bonus);
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
							ref.setSkillDevelopmentType(factory().subcategoriedSkillData(skill_id), SkillDevelopmentType::fromString(skill_type_id).value());
						}
					}
				}
			}

			// Skill development type choices
			if (boost::optional<const pt::ptree&> skill_development_type_choices = v.second.get_child_optional("skill-type-choices")) {
				for (const auto& skill_development_type_choice : skill_development_type_choices.get()) {
					GameRuleDataChoice<SubcategoriedSkillData> choice{};

					// make sure we have a valid development type first
					std::string skill_type_id = skill_development_type_choice.second.get<std::string>("skill-type");
					if (SkillDevelopmentType::fromString(skill_type_id)) {
						choice.setNumChoices(skill_development_type_choice.second.get<int>("num-choices"));
						for (const auto& skill_name_tree : skill_development_type_choice.second.get_child("skills")) {
							std::string skill_id = GameRuleData::generateId("Skill", skill_name_tree.second.get_value<std::string>());
							boost::optional<std::string> subcategory = skill_name_tree.second.get_optional<std::string>("<xmlattr>.subcategory");

							SubcategoriedSkillData& sub_skill = (subcategory ? factory().subcategoriedSkillData(skill_id, subcategory.value()) : factory().subcategoriedSkillData(skill_id));
							choice.addOption(sub_skill);
						}
						ref.addSkillDevelopmentTypeChoice(std::move(choice), SkillDevelopmentType::fromString(skill_type_id).value());
					}
				}
			}

			// Skill subcategory development type choices
			if (boost::optional<const pt::ptree&> skill_subcategory_development_type_choices = v.second.get_child_optional("skill-subcategory-type-choice")) {
				for (const auto& skill_subcategory_development_type_choice : skill_subcategory_development_type_choices.get()) {
					GameRuleDataChoice<SkillData> choice{};

					// make sure we have a valid development type first
					std::string skill_type_id = skill_subcategory_development_type_choice.second.get<std::string>("skill-type");
					if (SkillDevelopmentType::fromString(skill_type_id)) {
						choice.setNumChoices(skill_subcategory_development_type_choice.second.get<int>("num-choices"));
						for (const auto& skill_name_tree : skill_subcategory_development_type_choice.second.get_child("skills")) {
							std::string skill_id = GameRuleData::generateId("Skill", skill_name_tree.second.get_value<std::string>());
							choice.addOption(factory().get<SkillData>(skill_id));
						}
						ref.addSkillSubcategoryDevelopmentTypeChoice(std::move(choice), SkillDevelopmentType::fromString(skill_type_id).value());
					}
				}
			}

			// Get skill category profession bonuses
			if (boost::optional<const pt::ptree&> skill_category_bonuses = v.second.get_child_optional("skill-category-bonuses")) {
				for (const auto& skill_category_bonus : skill_category_bonuses.get()) {
					std::string skill_category_id{ GameRuleData::generateId("SkillCategory", skill_category_bonus.second.get<std::string>("<xmlattr>.category")) };
					int bonus = skill_category_bonus.second.get_value<int>();
					if (bonus) ref.addSkillCategoryProfessionBonus(factory().get<SkillCategoryData>(skill_category_id), bonus);
				}
			}

			// Get skill category special bonuses
			if (boost::optional<const pt::ptree&> skill_category_bonuses = v.second.get_child_optional("skill-category-skill-bonuses")) {
				for (const auto& skill_category_bonus : skill_category_bonuses.get()) {
					std::string skill_category_id{ GameRuleData::generateId("SkillCategory", skill_category_bonus.second.get<std::string>("<xmlattr>.category")) };
					int bonus = skill_category_bonus.second.get_value<int>();
					if (bonus) ref.addSkillCategorySpecialBonus(factory().get<SkillCategoryData>(skill_category_id), bonus);
				}
			}

			// Get skill category development types
			if (boost::optional<const pt::ptree&> skill_category_development_types = v.second.get_child_optional("skill-category-skill-modifiers")) {
				for (const auto& skill_category_development_type : skill_category_development_types.get()) {
					std::string skill_category_id{ GameRuleData::generateId("SkillCategory", skill_category_development_type.second.get<std::string>("<xmlattr>.category")) };
					std::string skill_type_id = skill_category_development_type.second.get_value<std::string>();
					if (SkillDevelopmentType::fromString(skill_type_id)) {
						ref.addSkillCategorySkillDevelopmentType(factory().get<SkillCategoryData>(skill_category_id), SkillDevelopmentType::fromString(skill_type_id).value());
					}
				}
			}

			// Get skill category development costs
			if (boost::optional<const pt::ptree&> skill_category_development_costs = v.second.get_child_optional("skill-category-costs")) {
				for (const auto& skill_category_development_cost : skill_category_development_costs.get()) {
					std::string skill_category_id{ GameRuleData::generateId("SkillCategory", skill_category_development_cost.second.get<std::string>("<xmlattr>.category")) };
					std::string cost = skill_category_development_cost.second.get_value<std::string>();
					ref.addSkillCategoryDevelopmentCost(factory().get<SkillCategoryData>(skill_category_id), SkillDevelopmentCost(cost));
				}
			}

			// Skill category skill development choices
			if (boost::optional<const pt::ptree&> skill_category_development_type_choices = v.second.get_child_optional("skill-category-skill-modifier-choices")) {
				for (const auto& skill_category_development_type_choice : skill_category_development_type_choices.get()) {
					GameRuleDataChoice<SkillCategoryData> choice{};

					// make sure we have a valid development type first
					std::string skill_type_id = skill_category_development_type_choice.second.get<std::string>("skill-type");
					if (SkillDevelopmentType::fromString(skill_type_id)) {
						choice.setNumChoices(skill_category_development_type_choice.second.get<int>("num-choices"));
						for (const auto& category_name_tree : skill_category_development_type_choice.second.get_child("categories")) {
							std::string category_name = category_name_tree.second.get_value<std::string>();
							std::string category_id = GameRuleData::generateId("SkillCategory", category_name);
							choice.addOption(factory().get<SkillCategoryData>(category_id));
						}
						ref.addSkillCategorySkillDevelopmentTypeChoice(std::move(choice), SkillDevelopmentType::fromString(skill_type_id).value());
					}
				}
			}

			// Skill group skill development choices
			if (boost::optional<const pt::ptree&> skill_group_development_type_choices = v.second.get_child_optional("skill-group-skill-modifier-choices")) {
				for (const auto& skill_group_development_type_choice : skill_group_development_type_choices.get()) {
					GameRuleDataChoice<SkillGroupData> choice{};

					// make sure we have a valid development type first
					std::string skill_type_id = skill_group_development_type_choice.second.get<std::string>("skill-type");
					if (SkillDevelopmentType::fromString(skill_type_id)) {
						choice.setNumChoices(skill_group_development_type_choice.second.get<int>("num-choices"));
						for (const auto& group_name_tree : skill_group_development_type_choice.second.get_child("groups")) {
							std::string group_name = group_name_tree.second.get_value<std::string>();
							std::string group_id = GameRuleData::generateId("SkillGroup", group_name);
							choice.addOption(factory().get<SkillGroupData>(group_id));
						}
						ref.addSkillGroupSkillDevelopmentTypeChoice(std::move(choice), SkillDevelopmentType::fromString(skill_type_id).value());
					}
				}
			}

			// Get skill group profession bonuses
			if (boost::optional<const pt::ptree&> skill_group_bonuses = v.second.get_child_optional("skill-group-bonuses")) {
				for (const auto& skill_group_bonus : skill_group_bonuses.get()) {
					int bonus = skill_group_bonus.second.get_value<int>();
					std::string group_id{ GameRuleData::generateId("SkillGroup", skill_group_bonus.second.get<std::string>("<xmlattr>.group"))};
					if (bonus) ref.addSkillGroupProfessionBonus(factory().get<SkillGroupData>(group_id), bonus);
				}
			}

			// Get skill group special bonuses
			if (boost::optional<const pt::ptree&> skill_group_bonuses = v.second.get_child_optional("skill-group-skill-bonuses")) {
				for (const auto& skill_group_bonus : skill_group_bonuses.get()) {
					int bonus = skill_group_bonus.second.get_value<int>();
					std::string group_id{ GameRuleData::generateId("SkillGroup", skill_group_bonus.second.get<std::string>("<xmlattr>.group")) };
					if (bonus) ref.addSkillGroupSpecialBonus(factory().get<SkillGroupData>(group_id), bonus);
				}
			}

			// Skill group skill development types
			if (boost::optional<const pt::ptree&> skill_group_skill_modifiers = v.second.get_child_optional("skill-group-skill-modifiers")) {
				for (const auto& skill_group_skill_modifier : skill_group_skill_modifiers.get()) {
					std::string skill_type_id = skill_group_skill_modifier.second.get_value<std::string>();
					if (SkillDevelopmentType::fromString(skill_type_id)) {
						std::string group_id{ GameRuleData::generateId("SkillGroup", skill_group_skill_modifier.second.get<std::string>("<xmlattr>.group")) };
						ref.addSkillGroupSkillDevelopmentType(factory().get<SkillGroupData>(group_id), SkillDevelopmentType::fromString(skill_type_id).value());
					}
				}
			}

			std::cout << "\tProfession name: " << ref.name() << std::endl;
	}
}