#include <string>
#include <AttackTableDatafileParserJson.h>
#include <table/AttackTable.h>
#include <NumberMatcherFactory.h>

using namespace rm::rule::enums;
using namespace rm::rule::table;

namespace rm::rule::parser {

	void AttackTableDatafileParserJson::populateDatum(std::string& id, pt::ptree& datum) {
		AttackTable& game_data = factory().get<AttackTable>(id);

		datum.put("id", game_data.id());
		datum.put("name", game_data.name());

		int max_row{ 0 };

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
			prow.put("at1", game_data.cell(ArmourType::kAT1, min));
			prow.put("at2", game_data.cell(ArmourType::kAT2, min));
			prow.put("at3", game_data.cell(ArmourType::kAT3, min));
			prow.put("at4", game_data.cell(ArmourType::kAT4, min));
			prow.put("at5", game_data.cell(ArmourType::kAT5, min));
			prow.put("at6", game_data.cell(ArmourType::kAT6, min));
			prow.put("at7", game_data.cell(ArmourType::kAT7, min));
			prow.put("at8", game_data.cell(ArmourType::kAT8, min));
			prow.put("at9", game_data.cell(ArmourType::kAT9, min));
			prow.put("at10", game_data.cell(ArmourType::kAT10, min));
			prow.put("at11", game_data.cell(ArmourType::kAT11, min));
			prow.put("at12", game_data.cell(ArmourType::kAT12, min));
			prow.put("at13", game_data.cell(ArmourType::kAT13, min));
			prow.put("at14", game_data.cell(ArmourType::kAT14, min));
			prow.put("at15", game_data.cell(ArmourType::kAT15, min));
			prow.put("at16", game_data.cell(ArmourType::kAT16, min));
			prow.put("at17", game_data.cell(ArmourType::kAT17, min));
			prow.put("at18", game_data.cell(ArmourType::kAT18, min));
			prow.put("at19", game_data.cell(ArmourType::kAT19, min));
			prow.put("at20", game_data.cell(ArmourType::kAT20, min));
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
			prow.put("at1", game_data.cell(ArmourType::kAT1, min, min));
			prow.put("at2", game_data.cell(ArmourType::kAT2, min, min));
			prow.put("at3", game_data.cell(ArmourType::kAT3, min, min));
			prow.put("at4", game_data.cell(ArmourType::kAT4, min, min));
			prow.put("at5", game_data.cell(ArmourType::kAT5, min, min));
			prow.put("at6", game_data.cell(ArmourType::kAT6, min, min));
			prow.put("at7", game_data.cell(ArmourType::kAT7, min, min));
			prow.put("at8", game_data.cell(ArmourType::kAT8, min, min));
			prow.put("at9", game_data.cell(ArmourType::kAT9, min, min));
			prow.put("at10", game_data.cell(ArmourType::kAT10, min, min));
			prow.put("at11", game_data.cell(ArmourType::kAT11, min, min));
			prow.put("at12", game_data.cell(ArmourType::kAT12, min, min));
			prow.put("at13", game_data.cell(ArmourType::kAT13, min, min));
			prow.put("at14", game_data.cell(ArmourType::kAT14, min, min));
			prow.put("at15", game_data.cell(ArmourType::kAT15, min, min));
			prow.put("at16", game_data.cell(ArmourType::kAT16, min, min));
			prow.put("at17", game_data.cell(ArmourType::kAT17, min, min));
			prow.put("at18", game_data.cell(ArmourType::kAT18, min, min));
			prow.put("at19", game_data.cell(ArmourType::kAT19, min, min));
			prow.put("at20", game_data.cell(ArmourType::kAT20, min, min));
			umrows.push_back(std::make_pair("", prow));
		}

		datum.put("max_row", max_row);
		datum.push_back(std::make_pair("modified-rows", mrows));
		if (!umrows.empty()) datum.push_back(std::make_pair("unmodified-rows", umrows));
	}

	void AttackTableDatafileParserJson::parse() {
		std::cout << "Loading Attack Table data ..." << std::endl;

		// Get the tables to parse and loop through them
		const pt::ptree& tree = ptree().get_child(rootNode());
		for (const auto& ptable : tree) {
			std::string id = ptable.second.get<std::string>("id");

			AttackTable& table = factory().get<AttackTable>(id);
			table.setName(ptable.second.get<std::string>("name"));
			table.setMaxRow(ptable.second.get<int>("max_row"));

			// Get a factory for the matchers
			NumberMatcherFactory matchers;

			if (boost::optional<const pt::ptree&> pmods = ptable.second.get_child_optional("modified-rows")) {
				for (const auto& pmod : pmods.get()) {
					TableRow<std::string> row{};
					int min = pmod.second.get<int>("min");
					int max = pmod.second.get<int>("max");
					for (int i{ 1 }; i < 21; i++) {
						row.addCell(pmod.second.get<std::string>("at" + std::to_string(i)));
					}
					table.addRow(matchers.matcher(min, max), row);
				}
			}

			if (boost::optional<const pt::ptree&> pumods = ptable.second.get_child_optional("unmodified-rows")) {
				for (const auto& pumod : pumods.get()) {
					TableRow<std::string> row{};
					int min = pumod.second.get<int>("min");
					int max = pumod.second.get<int>("max");
					for (int i{ 1 }; i < 21; i++) {
						row.addCell(pumod.second.get<std::string>("at" + std::to_string(i)));
					}
					table.addUnmodifiedRow(matchers.matcher(min, max), row);
				}
			}
		}
	}

} // namespace rm::rule::parser