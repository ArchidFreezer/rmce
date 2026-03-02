#include <NumberMatcherFactory.h>
#include <SpecialAttackTableSerializer.h>

namespace rm::rule::parser {

void SpecialAttackTableSerializer::parse(pt::ptree& ptree) {
	std::cout << "Loading SpecialAttackTable data ..." << std::endl;

	// Get the specialattacktables to parse and loop through them
	const pt::ptree& tree = ptree.get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", manager().generateId(ruleDatatype(), name));

		// Get a SpecialAttackTableData object from the object manager
		rm::rule::table::SpecialAttackTable& table = manager().get<rm::rule::table::SpecialAttackTable>(id);
		table.setName(name);
		table.setLimit(AttackSizeType::kSmall, v.second.get<int>("small"));
		table.setLimit(AttackSizeType::kMedium, v.second.get<int>("medium"));
		table.setLimit(AttackSizeType::kLarge, v.second.get<int>("large"));
		table.setLimit(AttackSizeType::kHuge, v.second.get<int>("huge"));

		rm::rule::table::NumberMatcherFactory matchers;

		if (boost::optional<const pt::ptree&> pmods = v.second.get_child_optional("modified-rows")) {
			for (const auto& pmod : pmods.get()) {
				rm::rule::table::TableRow<std::string> row{};
				int min = pmod.second.get<int>("min");
				int max = pmod.second.get<int>("max");
				for (int i{1}; i < 21; i++) {
					row.addCell(pmod.second.get<std::string>("at" + std::to_string(i)));
				}
				table.addRow(matchers.matcher(min, max), row);
			}
		}

		if (boost::optional<const pt::ptree&> pumods = v.second.get_child_optional("unmodified-rows")) {
			for (const auto& pumod : pumods.get()) {
				rm::rule::table::TableRow<std::string> row{};
				int min = pumod.second.get<int>("min");
				int max = pumod.second.get<int>("max");
				for (int i{1}; i < 21; i++) {
					row.addCell(pumod.second.get<std::string>("at" + std::to_string(i)));
				}
				table.addUnmodifiedRow(matchers.matcher(min, max), row);
			}
		}

		std::cout << "\tSpecialAttackTable name: " << table.name() << std::endl;
	}
	std::cout << " done" << std::endl;
}

void SpecialAttackTableSerializer::populateDatum(std::string key, pt::ptree& datum) {
	rm::rule::table::SpecialAttackTable& game_data = manager().get<rm::rule::table::SpecialAttackTable>(key);
	datum.put("id", game_data.id());
	datum.put("name", game_data.name());
	datum.put("small", game_data.limit(AttackSizeType::kSmall));
	datum.put("medium", game_data.limit(AttackSizeType::kMedium));
	datum.put("large", game_data.limit(AttackSizeType::kLarge));
	datum.put("huge", game_data.limit(AttackSizeType::kHuge));

	int max_row{0};

	pt::ptree mrows; // Modified rows
	// The game data stores the ranges as pointers, which is essentially a random sort. We want them ordered so derefence
	// the pointers and place them into a set which will order them as we want
	std::set<archid::NumberRange<int>> ranges{};
	for (auto& m : game_data.modified()) {
		ranges.insert(*m.first);
		max_row = std::max(max_row, m.first->max());
	}
	for (auto& range : ranges) {
		pt::ptree prow;

		int min = range.min();

		prow.put("min", min);
		prow.put("max", range.max());
		prow.put("at1", game_data.cell(ArmourType::kAT1, AttackSizeType::kHuge, min));
		prow.put("at2", game_data.cell(ArmourType::kAT2, AttackSizeType::kHuge, min));
		prow.put("at3", game_data.cell(ArmourType::kAT3, AttackSizeType::kHuge, min));
		prow.put("at4", game_data.cell(ArmourType::kAT4, AttackSizeType::kHuge, min));
		prow.put("at5", game_data.cell(ArmourType::kAT5, AttackSizeType::kHuge, min));
		prow.put("at6", game_data.cell(ArmourType::kAT6, AttackSizeType::kHuge, min));
		prow.put("at7", game_data.cell(ArmourType::kAT7, AttackSizeType::kHuge, min));
		prow.put("at8", game_data.cell(ArmourType::kAT8, AttackSizeType::kHuge, min));
		prow.put("at9", game_data.cell(ArmourType::kAT9, AttackSizeType::kHuge, min));
		prow.put("at10", game_data.cell(ArmourType::kAT10, AttackSizeType::kHuge, min));
		prow.put("at11", game_data.cell(ArmourType::kAT11, AttackSizeType::kHuge, min));
		prow.put("at12", game_data.cell(ArmourType::kAT12, AttackSizeType::kHuge, min));
		prow.put("at13", game_data.cell(ArmourType::kAT13, AttackSizeType::kHuge, min));
		prow.put("at14", game_data.cell(ArmourType::kAT14, AttackSizeType::kHuge, min));
		prow.put("at15", game_data.cell(ArmourType::kAT15, AttackSizeType::kHuge, min));
		prow.put("at16", game_data.cell(ArmourType::kAT16, AttackSizeType::kHuge, min));
		prow.put("at17", game_data.cell(ArmourType::kAT17, AttackSizeType::kHuge, min));
		prow.put("at18", game_data.cell(ArmourType::kAT18, AttackSizeType::kHuge, min));
		prow.put("at19", game_data.cell(ArmourType::kAT19, AttackSizeType::kHuge, min));
		prow.put("at20", game_data.cell(ArmourType::kAT20, AttackSizeType::kHuge, min));
		mrows.push_back(std::make_pair("", prow));
	}

	pt::ptree umrows; // Unmodified rows
	// The game data stores the ranges as pointers, which is essentially a random sort. We want them ordered so derefence
	// the pointers and place them into a set which will order them as we want
	ranges.clear(); // Reset so we can reuse the container
	for (auto& m : game_data.unmodified()) {
		ranges.insert(*m.first);
	}
	for (auto& range : ranges) {
		pt::ptree prow;

		int min = range.min();

		prow.put("min", min);
		prow.put("max", range.max());
		prow.put("at1", game_data.cell(ArmourType::kAT1, AttackSizeType::kHuge, min, min));
		prow.put("at2", game_data.cell(ArmourType::kAT2, AttackSizeType::kHuge, min, min));
		prow.put("at3", game_data.cell(ArmourType::kAT3, AttackSizeType::kHuge, min, min));
		prow.put("at4", game_data.cell(ArmourType::kAT4, AttackSizeType::kHuge, min, min));
		prow.put("at5", game_data.cell(ArmourType::kAT5, AttackSizeType::kHuge, min, min));
		prow.put("at6", game_data.cell(ArmourType::kAT6, AttackSizeType::kHuge, min, min));
		prow.put("at7", game_data.cell(ArmourType::kAT7, AttackSizeType::kHuge, min, min));
		prow.put("at8", game_data.cell(ArmourType::kAT8, AttackSizeType::kHuge, min, min));
		prow.put("at9", game_data.cell(ArmourType::kAT9, AttackSizeType::kHuge, min, min));
		prow.put("at10", game_data.cell(ArmourType::kAT10, AttackSizeType::kHuge, min, min));
		prow.put("at11", game_data.cell(ArmourType::kAT11, AttackSizeType::kHuge, min, min));
		prow.put("at12", game_data.cell(ArmourType::kAT12, AttackSizeType::kHuge, min, min));
		prow.put("at13", game_data.cell(ArmourType::kAT13, AttackSizeType::kHuge, min, min));
		prow.put("at14", game_data.cell(ArmourType::kAT14, AttackSizeType::kHuge, min, min));
		prow.put("at15", game_data.cell(ArmourType::kAT15, AttackSizeType::kHuge, min, min));
		prow.put("at16", game_data.cell(ArmourType::kAT16, AttackSizeType::kHuge, min, min));
		prow.put("at17", game_data.cell(ArmourType::kAT17, AttackSizeType::kHuge, min, min));
		prow.put("at18", game_data.cell(ArmourType::kAT18, AttackSizeType::kHuge, min, min));
		prow.put("at19", game_data.cell(ArmourType::kAT19, AttackSizeType::kHuge, min, min));
		prow.put("at20", game_data.cell(ArmourType::kAT20, AttackSizeType::kHuge, min, min));
		umrows.push_back(std::make_pair("", prow));
	}

	datum.put("max_row", max_row);
	datum.push_back(std::make_pair("modified-rows", mrows));
	if (!umrows.empty())
		datum.push_back(std::make_pair("unmodified-rows", umrows));
}

std::string SpecialAttackTableSerializer::rootNode() const {
	return "attack-tables";
}

} // namespace rm::rule::parser