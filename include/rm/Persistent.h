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
}