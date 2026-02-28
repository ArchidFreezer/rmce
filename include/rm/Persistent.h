#pragma once

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

}