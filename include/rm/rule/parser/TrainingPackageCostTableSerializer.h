#pragma once

#include <string_view>
#include <PersistentObjectSerializer.h>

namespace rm::rule::parser {

	/**
	 * @class TrainingPackageCostTableSerializer
	 * @brief Class to parse training package cost table data
	 *
	 * This abstract class implements the read(istream) and save(ostream) functions to perform the actual parsing of the game data.
	 * 
	 * A derived class is required to implement the read() and save() functions for whichever type of stream is being used such as a file stream.
	 *
	 * @see DatafileParser
	 */
	class TrainingPackageCostTableSerializer : public PersistentObjectSerializer {
	public:
		/**
		 * @brief Deleted default constructor to ensure initialisation of the base class.
		 */
		TrainingPackageCostTableSerializer() = delete;

		/**
		 * @brief Constructor
		 * @param object_manager Reference to an object manager to handle the data objects
		 */
		TrainingPackageCostTableSerializer(rm::PersistentObjectManager& object_manager) : PersistentObjectSerializer(object_manager, "TrainingPackageCostTable") {}

		~TrainingPackageCostTableSerializer() = default; /** Default destructor for polymorphism */
		TrainingPackageCostTableSerializer(const TrainingPackageCostTableSerializer&) = default; /** Default copy constructor */
		TrainingPackageCostTableSerializer& operator=(const TrainingPackageCostTableSerializer&) = default; /** Default copy assignment operator */
		TrainingPackageCostTableSerializer(TrainingPackageCostTableSerializer&&) noexcept = default; /** Default move constructor */
		TrainingPackageCostTableSerializer& operator=(TrainingPackageCostTableSerializer&&) noexcept = default; /** Default move assignment operator */

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