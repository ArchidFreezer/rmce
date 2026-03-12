#pragma once

#include <boost/uuid.hpp>
#include <concepts>
#include <string>

namespace rm {

/**
 * @class Persistent
 *
 * @brief Polymorphic class that is the base for objects that are stored in a persistent manner, such as in a database or file system.
 *
 * This class is used as the base for all classes that contain objects to be stored, e.g. Characters, Items, etc.
 * These objects are typically cached, though that is not a requirement.
 *
 */
class Persistent {
public:
	virtual ~Persistent() = default;           /**< Default destructor for polymorphism */
	virtual const std::string& id() const = 0; /**< Pure virtual method to get the unique identifier of the object */
};

/**
 * @class PersistentId
 *
 * @brief Concrete implementation of the Persistent class that generates a unique identifier using Boost UUIDs
 *
 * This class provides a default implementation of the Persistent interface, where the unique identifier is generated as a random UUID string upon construction.
 * It can be used as a base class for any persistent object that requires a unique identifier without needing to implement the id generation logic in each derived class.
 */
class PersistentId : public Persistent {
public:
	/**< Default constructor that initializes the id to an empty string */
	PersistentId() : id_{to_string(boost::uuids::random_generator()())} {
	}

	virtual ~PersistentId() = default; /**< Default destructor for polymorphism */

	/**
	 * @brief Gets the identifier of this object.
	 * @return A constant reference to the identifier string.
	 */
	const std::string& id() const override {
		return id_;
	}

protected:
	std::string id_; /**< Unique identifier for the object */

	PersistentId(const PersistentId& other) = default;                /**< Copy constructor, defaulted to allow copying of the id string */
	PersistentId(PersistentId&& other) noexcept = default;            /**< Move constructor, defaulted to allow moving of the id string */
	PersistentId& operator=(const PersistentId& other) = default;     /**< Copy assignment operator, defaulted to allow copying of the id string */
	PersistentId& operator=(PersistentId&& other) noexcept = default; /**< Move assignment operator, defaulted to allow moving of the id string */

private:
};

/**
 * @brief Concept that ensures that a templated type is derived from the Persistent class
 *
 * It is used when templating a class or method where it may be used as follows
 *
 * @code
 * template <persistent_object T>
 * void someFunction(T obj) {
 *   // do something with obj
 * }
 * @endcode
 */
template<class T>
concept persistent_object = std::is_base_of<Persistent, T>::value;

/**
 * @brief Concept that ensures a templated type is both derived from Persistent and default constructible
 *
 * This concept combines the persistent_object requirement with default initialization capability,
 * ensuring the type can be created with `new T()` and is suitable for cache management.
 *
 * @code
 * template <default_persistent_object T>
 * void someFunction() {
 *   std::unique_ptr<T> obj(new T()); // Valid because T is both persistent and default constructible
 * }
 * @endcode
 */
template<class T>
concept default_persistent_object = persistent_object<T> && std::default_initializable<T>;

/**
 * @brief Concept that ensures a templated type is derived from Persistent, is NOT default constructible, and has a constructor taking a std::string parameter
 *
 * This concept is used for persistent objects that require an ID to be provided at construction time, such as GameRuleData objects. It ensures the type cannot be default constructed and must be initialized with a unique identifier, plus
 * have a generateId method that returns void. This method is expected to set the id of the object based on the provided identifier and a prefix derived from the type of the object, ensuring that all objects of this type have unique
 * identifiers in a standard format.
 *
 * @code
 * template <id_persistent_object T>
 * void someFunction(const std::string& id) {
 *   std::unique_ptr<T> obj(new T(id)); // Valid because T requires string construction
 *   // T obj; // Would fail - no default constructor
 * }
 * @endcode
 */
template<class T>
concept id_persistent_object = persistent_object<T> && (!std::default_initializable<T>) && std::constructible_from<T, std::string> && requires(T v) {
	{ v.generateId() } -> std::same_as<void>;
};

} // namespace rm