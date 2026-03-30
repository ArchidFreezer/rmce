#pragma once
#include <sstream>
#include <PersistentSerializer.h>

namespace rm::serial {

/**
 * @class PersistentTsvSerializer
 * @brief Abstract class for serializing and deserializing persistent objects to and from a tab separated file.
 *
 * The copy and move constructors and assignment operators are deleted to prevent slicing and ensure that the class is only used as a base class for specific serializers.
 * 
 * The serializers that inherit from this class should implement the serializeObject and deserializeObject methods to handle the specific serialization and deserialization logic for the type of persistent object they are designed to handle.
 * The PersistentTsvSerializer class provides a common interface and a reference to the PersistentObjectManager that can be used by derived classes to manage persistent objects during serialization and deserialization.
 * 
 * The serializeObject method should take a PersistentObject and return a string representation of that object in TSV format, while the deserializeObject method should take an input stream containing TSV data and return a reference to the
 * deserialized PersistentObject.
 */
template<persistent_object PersistentObject>
class PersistentTsvSerializer : public PersistentSerializer<PersistentObject, std::string, std::istream> {
public:
	/**
	 * @brief Constructor for the PersistentTsvSerializer class.
	 *
	 * @param manager Reference to the PersistentObjectManager used for managing persistent objects during serialization and deserialization.
	 */
	PersistentTsvSerializer(rm::PersistentObjectManager& manager) : PersistentSerializer<PersistentObject, std::string, std::istream>(manager) {
	}

	/**
	 * @brief Virtual destructor for the PersistentTsvSerializer class.
	 */
	virtual ~PersistentTsvSerializer() = default;
};

} // namespace rm::serial
