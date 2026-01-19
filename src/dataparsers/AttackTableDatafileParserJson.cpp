#include <iostream>
#include <stdexcept>
#include <string>
#include <AttackTableDatafileParserJson.h>
#include <table/AttackTable.h>

void AttackTableDatafileParserJson::save(const std::string& filename) {
	if (filename.empty()) return;

	// Main tree
	pt::ptree tree;

	// Tree of attack tables
	pt::ptree ptables;

	std::set<std::string> keys{};
	cache().keys<AttackTable>(keys);

	// Loop through the tables
	for (std::string t : keys) {
		// Individual table
		pt::ptree ptable;
		try {
			AttackTable& table_data = cache().get<AttackTable>(t);


			ptable.put("id", table_data.id());
			ptable.put("name", table_data.name());

			int max_row{ 0 };

			pt::ptree mrows; // Modified rows
			for (auto& m : table_data.modified()) {
				pt::ptree prow;

				TableRow tr = m.second;

				max_row = std::max(max_row, m.first->max());

				prow.put("min", m.first->min());
				prow.put("max", m.first->max());
				prow.put("at1", tr.cell(0));
				prow.put("at2", tr.cell(1));
				prow.put("at3", tr.cell(2));
				prow.put("at4", tr.cell(3));
				prow.put("at5", tr.cell(4));
				prow.put("at6", tr.cell(5));
				prow.put("at7", tr.cell(6));
				prow.put("at8", tr.cell(7));
				prow.put("at9", tr.cell(8));
				prow.put("at10", tr.cell(9));
				prow.put("at11", tr.cell(10));
				prow.put("at12", tr.cell(11));
				prow.put("at13", tr.cell(12));
				prow.put("at14", tr.cell(13));
				prow.put("at15", tr.cell(14));
				prow.put("at16", tr.cell(15));
				prow.put("at17", tr.cell(16));
				prow.put("at18", tr.cell(17));
				prow.put("at19", tr.cell(18));
				prow.put("at20", tr.cell(19));
				mrows.push_back(std::make_pair("", prow));
			}

			pt::ptree umrows; // Modified rows
			for (auto& m : table_data.unmodified()) {
				pt::ptree prow;

				TableRow tr = m.second;

				prow.put("min", m.first->min());
				prow.put("max", m.first->max());
				prow.put("at1", tr.cell(0));
				prow.put("at2", tr.cell(1));
				prow.put("at3", tr.cell(2));
				prow.put("at4", tr.cell(3));
				prow.put("at5", tr.cell(4));
				prow.put("at6", tr.cell(5));
				prow.put("at7", tr.cell(6));
				prow.put("at8", tr.cell(7));
				prow.put("at9", tr.cell(8));
				prow.put("at10", tr.cell(9));
				prow.put("at11", tr.cell(10));
				prow.put("at12", tr.cell(11));
				prow.put("at13", tr.cell(12));
				prow.put("at14", tr.cell(13));
				prow.put("at15", tr.cell(14));
				prow.put("at16", tr.cell(15));
				prow.put("at17", tr.cell(16));
				prow.put("at18", tr.cell(17));
				prow.put("at19", tr.cell(18));
				prow.put("at20", tr.cell(19));
				umrows.push_back(std::make_pair("", prow));
			}

			ptable.put("max_row", max_row);
			ptable.push_back(std::make_pair("modified-rows", mrows));
			if (!umrows.empty()) ptable.push_back(std::make_pair("unmodified-rows", umrows));

		} catch (const std::out_of_range& e) {
			std::cout << "Error: " << e.what() << std::endl;
		}
		ptables.push_back(std::make_pair("", ptable));
	}

	tree.add_child(rootNode(), ptables);

	pt::write_json(filename, tree);
}

void AttackTableDatafileParserJson::parse(bool id_only) {
	// We know there are no references in attack tables so we create the complete object in the cache on the first pass
	if (!id_only) return;

	std::cout << "Loading Attack Table data ..." << std::endl;

	// Get the tables to parse and loop through them
	const pt::ptree& tree = ptree().get_child(rootNode());
	for (const auto& ptable : tree) {
		std::string name = ptable.second.get<std::string>("name");
		std::string id = ptable.second.get("id", GameRuleData::generateId(ruleDatatype(), name));
		int max_rows = ptable.second.get<int>("max_row");

		std::unique_ptr<AttackTable>table = std::make_unique <AttackTable>(name, max_rows);
		table->setName(name);

		if (boost::optional<const pt::ptree&> pmods = ptable.second.get_child_optional("modified-rows")) {
			for (const auto& pmod : pmods.get()) {
				TableRow<std::string> row{};
				int min = pmod.second.get<int>("min");
				int max = pmod.second.get<int>("max");
				for (int i{ 1 }; i < 21; i++) {
					row.addCell(pmod.second.get<std::string>("at" + std::to_string(i)));
				}
				table->addRow(std::make_unique<NumberRange<int>>(min, max), row);
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
				table->addUnmodifiedRow(std::make_unique<NumberRange<int>>(min, max), row);
			}
		}

		cache().add<AttackTable>(std::move(table), id);
	}
}