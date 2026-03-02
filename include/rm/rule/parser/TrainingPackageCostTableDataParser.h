#pragma once

#include <string_view>
#include <DataParser.h>

namespace rm::rule::parser {

	/**
	 * @class TrainingPackageCostTableDataParser
	 * @brief Class to parse training package cost table data
	 *
	 * This abstract class implements the read(istream) and save(ostream) functions to perform the actual parsing of the game data.
	 * 
	 * A derived class is required to implement the read() and save() functions for whichever type of stream is being used such as a file stream.
	 *
	 * @see DatafileParser
	 */
	class TrainingPackageCostTableDataParser : public DataParser {
	public:
		/**
		 * @brief Deleted default constructor to ensure initialisation of the base class.
		 */
		TrainingPackageCostTableDataParser() = delete;

		/**
		 * @brief Constructor
		 * @param object_manager Reference to an object manager to handle the data objects
		 */
		TrainingPackageCostTableDataParser(rm::PersistentObjectManager& object_manager) : DataParser(object_manager, "TrainingPackageCostTable") {}

		~TrainingPackageCostTableDataParser() = default; /** Default destructor for polymorphism */
		TrainingPackageCostTableDataParser(const TrainingPackageCostTableDataParser&) = default; /** Default copy constructor */
		TrainingPackageCostTableDataParser& operator=(const TrainingPackageCostTableDataParser&) = default; /** Default copy assignment operator */
		TrainingPackageCostTableDataParser(TrainingPackageCostTableDataParser&&) noexcept = default; /** Default move constructor */
		TrainingPackageCostTableDataParser& operator=(TrainingPackageCostTableDataParser&&) noexcept = default; /** Default move assignment operator */

		/**
		 * @brief Read training package cost table data from file, convert to objects and store in the game rule data cache
		 * @param is Input stream to read from
		 */
		void read(std::istream& is);

		/**
		 * @brief Write training package cost table data from the cache to a file
		 * @param os Output stream to write to
		 */
		void save(std::ostream& os);

	};

} // namespace rm::rule::parser