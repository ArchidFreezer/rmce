#pragma once
#include <iostream>
#include <string>

#include <PersistentObjectManager.h>

/**
 * @namespace rm::rule::parser
 * @brief Datafile parsers to (de)serialise game data to and from iostreams
 */
namespace rm::rule::parser {

	/**
	 * @class PersistentObjectSerializer
	 * @brief Abstract class to facilitate serialising persistent objects to and from an iostream.
	 *
	 * This base class has the tools to assist in the processing of game rule data, such as skills, professions, etc., from persistent storage.
	 *
	 * The class also contains a reference to a data cache where the data objects read from the stream should be stored.
	 */
	class PersistentObjectSerializer {
	public:
		/**
		 * @brief Consructor
		 * @param object_manager Reference to an object manager to handle the data objects
		 * @param datatype String containing the name of the type of data being processed
		 */
		PersistentObjectSerializer(rm::PersistentObjectManager& object_manager, std::string_view datatype) :
			rule_datatype_{ datatype },
			object_manager_{ object_manager } {}

		/**
		 * @brief Default destructor
		 *
		 * This is made virtual to define the class as polymorphic as a standard practice, even though there is another virtial method.
		 */
		virtual ~PersistentObjectSerializer() = default;

		PersistentObjectSerializer(const PersistentObjectSerializer&) = default; /**< Default copy constructor */
		PersistentObjectSerializer& operator=(const PersistentObjectSerializer&) = default; /**< Default copy assignment operator */
		PersistentObjectSerializer(PersistentObjectSerializer&&) noexcept = default; /**< Default move constructor */
		PersistentObjectSerializer& operator=(PersistentObjectSerializer&&) noexcept = default; /**< Default move assignment operator */

		/**
		 * @brief Getter for the type of data being processed
		 * @return String containing the name of the type of data being processed
		 */
		const std::string& ruleDatatype() const { return rule_datatype_; }

		/**
		 * @brief Read game rule data, convert to objects and store in the game rule data cache
		 */
		virtual void read() = 0;

		/**
		 * @brief Write game rule data from the cache
		 */
		virtual void save() = 0;

		/**
		 * @brief Get the object manager cache
		 */
		rm::PersistentObjectManager& factory() { return object_manager_; };

	private:
		rm::PersistentObjectManager object_manager_; /**< Factory to act as wrapper to cache, creating objects if required */
		std::string rule_datatype_{}; /**< Name of the type of data being processed */

	};

} // namespace rm::rule::parser