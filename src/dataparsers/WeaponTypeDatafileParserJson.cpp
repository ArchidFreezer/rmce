#include <WeaponTypeDatafileParserJson.h>

void WeaponTypeDatafileParserJson::populateDatum(std::string& id, pt::ptree& datum) {
	WeaponTypeData& game_data = cache().get<WeaponTypeData>(id);
	datum.put("id", game_data.id());
	datum.put("name", game_data.name());
	datum.put("notes", game_data.notes());
	datum.put("skill", game_data.skill().id());
	datum.put("book", game_data.book().id());
	datum.put("attack-table", game_data.attackTable().id());
	datum.put("fumble", game_data.fumble());
	datum.put("breakage", game_data.breakage());
	datum.put("min-length", game_data.minLength());
	datum.put("max-length", game_data.maxLength());
	datum.put("min-strength", game_data.minStrength());
	datum.put("max-strength", game_data.maxStrength());
	datum.put("min-weight", game_data.minWeight());
	datum.put("max-weight", game_data.maxWeight());
	datum.put("wooden-haft", game_data.woodenHaft());

	pt::ptree pcriticals;
	for (auto& critical : game_data.criticals()) {
		pt::ptree pcritical;
		pcritical.put("critical", toString(critical));
		pcritical.put("modifier", game_data.criticalModifier(critical));
		pcriticals.push_back(std::make_pair("", pcritical));
	}
	datum.push_back(std::make_pair("criticals", pcriticals));
	
	pt::ptree pranges;
	for (auto& range: game_data.ranges()) {
		pt::ptree prange;
		prange.put("min", range.min());
		prange.put("max", range.max());
		prange.put("modifier", game_data.rangeModifier(range.min()));
		pranges.push_back(std::make_pair("", prange));
	}
	if (!pranges.empty()) datum.push_back(std::make_pair("ranges", pranges));
	
}

void WeaponTypeDatafileParserJson::parse(bool id_only) {
	std::cout << "Loading WeaponType data ... ";
	std::cout << (id_only ? "[Pass 1]" : "[Pass 2]") << std::endl;

	// Get the lists to parse and loop through them
	const pt::ptree& tree = ptree().get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", GameRuleData::generateId(ruleDatatype(), name));

		if (id_only) {
			// We create a Data object and reference it with as a unique_ptr to allow us to use move semantics to transfer ownership
			// to the cache when we add it
			std::unique_ptr<WeaponTypeData> datum = std::make_unique<WeaponTypeData>(id);
			cache().add<WeaponTypeData>(std::move(datum), id);
		} else {
			WeaponTypeData& ref = cache().get<WeaponTypeData>(id);
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
			std::string book_id = v.second.get<std::string>("book");
			ref.setBook(cache().get<BookData>(book_id));

			// skill
			std::string skill_id = v.second.get<std::string>("skill");
			ref.setSkill(cache().get<SkillData>(skill_id));

			// attack table
			std::string attack_table_id = v.second.get<std::string>("attack-table");
			ref.setAttackTable(cache().get<AttackTable>(attack_table_id));

			// crits
			for (const auto& crit : v.second.get_child("criticals")) {
				int mod = crit.second.get<int>("modifier");
				std::string crit_name = crit.second.get<std::string>("critical");
				if (CriticalType::fromString(crit_name)) {
					ref.addCriticalModifier(CriticalType::fromString(crit_name).value(), mod);
				}
			}

			// ranges
			if (boost::optional<const pt::ptree&> ranges_tree = v.second.get_child_optional("ranges")) {
				for (const auto& range : ranges_tree.get()) {
					int min = range.second.get<int>("min");
					int max = range.second.get<int>("max");
					int mod = range.second.get<int>("modifier");
					ref.addRange(NumberRange<int>(min, max), mod);
				}
			}
			std::cout << "\tWeaponType name: " << ref.name() << std::endl;
		}
	}

}