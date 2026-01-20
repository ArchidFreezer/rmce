#include <iostream>
#include <AttackTableDatafileParserXml.h>
#include <table/AttackTable.h>
#include <table/SpecialAttackTable.h>
#include <table/TableRowNumberMatcherFactory.h>

void AttackTableDatafileParserXml::parse(bool id_only) {
	// We know there are no references in attack tables so we create the complete object in the cache on the first pass
	if (!id_only) return;

	std::cout << "Loading AttackTable data ..." << std::endl;

	const pt::ptree& tree = ptree().get_child(rootNode());
	for (const auto& v : tree) {

		std::string name = v.second.get<std::string>("name");

		// Ball spells only have rows that go up to 100
		int max_row_val{ name.ends_with("_Ball") ? 100 : 150 };

		// Check whether we have a standard or special attack table
		// The max attack roll sizes are in a specified order
		const pt::ptree& maxTree = v.second.get_child("max-attack-size-roll");
		std::map<AttackSizeType::Type, int> limits{}; /**< The maximum row index that each attack size may use */
		bool special = false;
		for (const auto& max : maxTree) {
			std::string attr = max.second.get<std::string>("<xmlattr>.size");
			int val = max.second.get_value<int>();
			if (attr.find("SMALL") != std::string::npos) {
				special = (val != max_row_val); // If the max small value is 150 then we know there are no bounds
				limits.emplace(AttackSizeType::kSmall, val);
			}
			else if (attr.find("MEDIUM") != std::string::npos) limits.emplace(AttackSizeType::kMedium, val);
			else if (attr.find("LARGE") != std::string::npos) limits.emplace(AttackSizeType::kLarge, val);
			else if (attr.find("HUGE") != std::string::npos) limits.emplace(AttackSizeType::kHuge, val);
		}

		// Now we can create the specific type of table
		std::unique_ptr<SpecialAttackTable>special_table = std::make_unique <SpecialAttackTable>(name, limits);
		special_table->setName(name);
		std::unique_ptr<AttackTable>attack_table = std::make_unique <AttackTable>(name, max_row_val);
		attack_table->setName(name);

		TableRowNumberMatcherFactory matchers;

		// Process the modified rows
		if (boost::optional<const pt::ptree&> mod_rows_tree = v.second.get_child_optional("modified-rows")) {
			for (const auto& prow : mod_rows_tree.get()) {
				TableRow<std::string> row{};
				int min = prow.second.get<int>("<xmlattr>.rangeMin");
				int max = prow.second.get<int>("<xmlattr>.rangeMax");
				int at = 20;
				// The data in the xml have AT20 first down to AT1, the attack table expects the cells in the other order
				std::map<int, std::string> cells{};
				for (const auto& value : prow.second) {
					std::string val_first = value.first;
					std::string val_data = value.second.data();

					if (value.first != "<xmlattr>") {
						cells.insert({ at--, value.second.data() });
					}
				}
				for (int i{ 1 }; i < 21; i++) {
					row.addCell(cells.at(i));
				}
				if (special)
					special_table->addRow(matchers.matcher(min, max), row);
				else
					attack_table->addRow(matchers.matcher(min, max), row);
			}
		}

		// Process the unmodified rows
		if (boost::optional<const pt::ptree&> mod_rows_tree = v.second.get_child_optional("unmodified-rows")) {
			for (const auto& prow : mod_rows_tree.get()) {
				TableRow<std::string> row{};
				int min = prow.second.get<int>("<xmlattr>.rangeMin");
				int max = prow.second.get<int>("<xmlattr>.rangeMax");
				int at = 20;
				// The data in the xml have AT20 first down to AT1, the attack table expects the cells in the other order
				std::map<int, std::string> cells{};
				for (const auto& value : prow.second) {
					std::string val_first = value.first;
					std::string val_data = value.second.data();

					if (value.first != "<xmlattr>") {
						cells.insert({ at--, value.second.data() });
					}
				}
				for (int i{ 1 }; i < 21; i++) {
					row.addCell(cells.at(i));
				}
				if (special)
					special_table->addUnmodifiedRow(matchers.matcher(min, max), row);
				else
					attack_table->addUnmodifiedRow(matchers.matcher(min, max), row);
			}
		}

		// We should now have the table parsed so add it to the cache
		std::cout << "\nTable name: " << name << std::endl;


		std::string id = GameRuleData::generateId(ruleDatatype(), name);;
		if (special)
			cache().add<SpecialAttackTable>(std::move(special_table), id);
		else
			cache().add<AttackTable>(std::move(attack_table), id);
	}

}
