#include <WeaponTypeData.h>
#include <WeaponTypeDatafileParserXml.h>

void WeaponTypeDatafileParserXml::parse() {
	std::cout << "Loading WeaponType data ... ";

	// Get the lists to parse and loop through them
	const pt::ptree& tree = ptree().get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", GameRuleData::generateId(ruleDatatype(), name));

		WeaponTypeData& ref = factory().get<WeaponTypeData>(id);
		ref.setName(name);
		ref.setNotes(v.second.get<std::string>("notes"));
		ref.setFumble(v.second.get<int>("fumble"));
		ref.setBreakage(v.second.get<int>("breakage"));
		ref.setMinStrength(v.second.get<float>("min-strength"));
		ref.setMaxStrength(v.second.get<float>("max-strength"));
		ref.setMinLength(v.second.get<float>("min-length"));
		ref.setMaxLength(v.second.get<float>("max-length"));
		ref.setMinWeight(v.second.get<float>("min-weight"));
		ref.setMaxWeight(v.second.get<float>("max-weight"));
		if (boost::optional<bool> wood = v.second.get_optional<bool>("wooden-haft")) {
			ref.setWoodenHaft(v.second.get<bool>("wooden-haft"));
		}

		// Get the book from the cache
		std::string book_id = GameRuleData::generateId("Book", v.second.get<std::string>("book"));
		ref.setBook(factory().get<BookData>(book_id));

		// skill
		std::string skill_id = GameRuleData::generateId("Skill", v.second.get<std::string>("skill"));
		ref.setSkill(factory().get<SkillData>(skill_id));

		// attack table
		std::string attack_table_id = GameRuleData::generateId("LookupTable AttackTable", v.second.get<std::string>("attack-table"));
		ref.setAttackTable(factory().get<AttackTable>(attack_table_id));

		// crits
		for (const auto& crit : v.second.get_child("crits")) {
			int mod = crit.second.get<int>("<xmlattr>.mod", 0);
			std::string crit_name = crit.second.get_value<std::string>();
			if (CriticalType::fromString(crit_name)) {
				ref.addCriticalModifier(CriticalType::fromString(crit_name).value(), mod);
			}
		}

		// ranges
		if (boost::optional<const pt::ptree&> ranges_tree = v.second.get_child_optional("ranges")) {
			for (const auto& range : ranges_tree.get()) {
				int min = range.second.get<int>("<xmlattr>.min");
				int max = range.second.get<int>("<xmlattr>.max");
				int mod = range.second.get_value<int>();
				ref.addRange(NumberRange<int>(min, max), mod);
			}
		}
		std::cout << "\tWeaponType name: " << ref.name() << std::endl;
	}
}