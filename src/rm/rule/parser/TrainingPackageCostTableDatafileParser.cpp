#include <fstream>
#include <sstream>

#include <StringUtils.h>
#include <TrainingPackageCostTableDatafileParser.h>
#include <GameRuleDataMatcherFactory.h>
#include <table/TableRowGameRuleDataMatcher.h>
#include <table/TrainingPackageCostTable.h>

using namespace rm::rule::table;

namespace rm::rule::parser {

	void TrainingPackageCostTableDatafileParser::read(const std::string& filename) {
		// Read the file and store the data in the cache
		std::ifstream file(filename);

		if (!file.is_open()) {
			throw std::runtime_error("Could not open file: " + filename);
		}

		std::string id = "TRAINING_PACKAGE_COST_TABLE";
		TrainingPackageCostTable& table = factory().get<TrainingPackageCostTable>(id);

		std::string line{};
		while (std::getline(file, line)) {
			if (line.empty()) { continue; }

			// Split the row by tabs to get the individual cells
			std::vector<std::string> tokens{ archid::tokenise(line, "\t") };

			// The first cell is the training package name, the rest are the costs for each profession
			// Grab the name and remove it from the vector so we can iterate through the costs in order
			std::string package_name{ tokens[0] };
			// Get the training package data object for the row
			const TrainingPackageData* package_data{ &factory().get<TrainingPackageData>(package_name) };

			GameRuleDataMatcherFactory matcher_factory{};
			const TableRowGameRuleDataMatcher* matcher = matcher_factory.matcher(package_data);

			tokens.erase(tokens.begin());

			TableRow<int> row{};

			for (const std::string& cost : tokens) {
				// The cost may be empty if there is no cost for the profession, so we need to check for that before trying to convert it to an int
				if (cost.empty()) { continue; }
				// Convert the cost to an int and add it to the table
				int cost_int{ std::stoi(cost) };
				row.addCell(cost_int);
			}

			table.addRow(matcher, row);

			std::cout << "Adding training package cost table row for package: " << package_name << "\n";
		}
	}

	void TrainingPackageCostTableDatafileParser::save(const std::string& filename) {
		// Read the file and store the data in the cache
		std::ofstream file(filename);

		if (!file.is_open()) {
			throw std::runtime_error("Could not open file: " + filename);
		}

		std::string id = "TRAINING_PACKAGE_COST_TABLE";
		TrainingPackageCostTable& table = factory().get<TrainingPackageCostTable>(id);

		// Sort the rows by the training package name and write them to the file, with the first column being the training package name and the rest being the costs for each profession
		int num_cols{ 0 };
		std::map<std::string, TableRow<int>> sorted_rows{};
		std::map<const TableRowGameRuleDataMatcher*, TableRow<int>> rows = table.modified();
		for (const auto& [matcher, row] : rows) {
			sorted_rows[matcher->gameData()->id()] = row;
			if (row.colCount() > num_cols) { num_cols = row.colCount(); }
		}

		for (const auto& [package_name, row] : sorted_rows) {
			file << package_name;
			for (int i{ 0 }; i < num_cols; i++) {
				file << "\t" << row.cell(i);
			}
			file << "\n";
		}
	}

} // namespace rm::rule::parser