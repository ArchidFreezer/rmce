#pragma once

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
		virtual ~Persistent() = default; /**< Default destructor for polymorphism */
		virtual const std::string& id() const = 0; /**< Pure virtual method to get the unique identifier of the object */
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
	 * This concept is used for persistent objects that require an ID to be provided at construction time,
	 * such as GameRuleData objects. It ensures the type cannot be default constructed and must be initialized with an ID.
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
	concept id_persistent_object = persistent_object<T>	&& (!std::default_initializable<T>)	&& std::constructible_from<T, std::string>;

}