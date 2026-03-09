#pragma once
#include <string>
#include <PersistentObjectManager.h>

namespace rm::rule::serial {

/**
 * @brief A base class template for serializing and deserializing persistent objects to and from a specific object representation.
 * @tparam Object The object representation type used for serialization. This is the format into which PersistentObject instances are serialized.
 * @tparam PersistentObject The persistent object type that satisfies the persistent_object concept. This is the type that will be serialized and deserialized.
 */
template<persistent_object PersistentObject, typename SerializeReturn, typename DeserializeObject>
class PersistentSerializer {
public:
	/**
	 * @brief Constructs a PersistentSerializer with a reference to a PersistentObjectManager.
	 * @param manager A reference to the PersistentObjectManager that will be used for serialization operations.
	 */
	PersistentSerializer(rm::PersistentObjectManager& manager) : manager_{manager} {
	}

	/**
	 * @brief Virtual destructor for the PersistentSerializer class.
	 *
	 * The destructor is declared as virtual to allow for proper cleanup of derived classes when deleting through a pointer to the base class.
	 */
	virtual ~PersistentSerializer() = default;

	/* The copy and move constructors and assignment operators are deleted to prevent slicing and ensure that the class is only used as a base class for specific serializers. */
	PersistentSerializer(const PersistentSerializer&) = delete;            /**< Delete the copy constructor */
	PersistentSerializer& operator=(const PersistentSerializer&) = delete; /**< Delete the copy assignment operator */
	PersistentSerializer(PersistentSerializer&&) = delete;                 /**< Delete the move constructor */
	PersistentSerializer& operator=(PersistentSerializer&&) = delete;      /**< Delete the move assignment operator */

	/**
	 * @brief Serialize a PersistentObject to an SerializeReturn
	 *
	 * @param ref The PersistentObject to serialize
	 * @return An SerializeReturn representing the serialized PersistentObject
	 */
	virtual SerializeReturn serializeObject(const PersistentObject& ref) const = 0;

	/**
	 * @brief Deserialize a DeserializeObject to a PersistentObject
	 * 
	 * The returned PersistentObject is not necessarily a new object, but may be a reference to an existing object in the PersistentObjectManager if the DeserializeObject contains an ID that matches an existing object. This allows for the
	 * deserialization of objects that reference other objects by ID, e.g. a BookData object that references a SkillData object by ID.
	 *
	 * @param obj The DeserializeObject to deserialize
	 * @return A reference to the deserialized PersistentObject
	 */
	virtual const PersistentObject& deserializeObject(DeserializeObject& obj) const = 0;

	/**
	 * @brief Get the PersistentObjectManager used by this serializer
	 *
	 * @return Reference to the PersistentObjectManager
	 */
	rm::PersistentObjectManager& manager() const {
		return manager_;
	}

protected:
	rm::PersistentObjectManager& manager_; /**< Object manager to retrive persistent objects from or add to */
};

} // namespace rm::rule::serial