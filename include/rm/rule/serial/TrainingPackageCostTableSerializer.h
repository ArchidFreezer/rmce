#pragma once

#include <PersistentTsvSerializer.h>
#include <table/TrainingPackageCostTable.h>

namespace rm::rule::serial {

/**
 * @class TrainingPackageCostTableSerializer
 * @brief Serializer for the TrainingPackageCostTable class to and from TSV format.
 *
 * This class provides methods to serialize a TrainingPackageCostTable object into a TSV string and to deserialize a TrainingPackageCostTable object from a TSV input stream. It inherits from PersistentTsvSerializer, which provides a common interface for serializers that handle persistent objects in TSV format.
 *
 * The serializeObject method should convert the contents of a TrainingPackageCostTable object into a string formatted as TSV, while the deserializeObject method should read TSV data from an input stream and populate a TrainingPackageCostTable object accordingly.
 */
class TrainingPackageCostTableSerializer : public PersistentTsvSerializer<rm::rule::table::TrainingPackageCostTable> {
public:
	/**
	 * @brief Constructor for the TrainingPackageCostTableSerializer class.
	 *
	 * @param manager Object manager to retrive persistent objects from or add to
	 */
	TrainingPackageCostTableSerializer(rm::PersistentObjectManager& manager) : PersistentTsvSerializer<rm::rule::table::TrainingPackageCostTable>(manager) {
	}

	/**
	 * @brief Serialize a TrainingPackageCostTableData object to a TSV string
	 *
	 * @param ref Reference to the TrainingPackageCostTableData object to serialize
	 * @return A string containing the serialized TrainingPackageCostTableData object in TSV format
	 */
	std::string serializeObject(const rm::rule::table::TrainingPackageCostTable& ref) const override;

	/**
	 * @brief Deserialize a TrainingPackageCostTableData object from a TSV string
	 *
	 * @param is Input stream containing the TSV data to deserialize
	 * @return Reference to the deserialized TrainingPackageCostTableData object
	 */
	const rm::rule::table::TrainingPackageCostTable& deserializeObject(std::istream& is) const override;
};

} // namespace rm::rule::serial
