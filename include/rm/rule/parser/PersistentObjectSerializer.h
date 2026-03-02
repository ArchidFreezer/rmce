#pragma once
#include <iostream>
#include <string>

#include <PersistentObjectManager.h>

/**
 * @namespace rm::rule::parser
 * @brief Datafile parsers to (de)serialise game data to and from iostreams
 * 
 * The Assignment copy and move constructores have been deleted to prevent issues with any streams that may be used in the (de)serialization process, as copying or moving a stream can lead to
 * issues with the stream state and can cause unexpected behaviour.
 * By deleting these operations, we ensure that the class cannot be copied or moved, which helps to prevent potential bugs and ensures that the (de)serialization process is handled correctly.
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

		PersistentObjectSerializer(const PersistentObjectSerializer&) = delete; /**< Delete copy constructor */
		PersistentObjectSerializer& operator=(const PersistentObjectSerializer&) = delete; /**< Delete copy assignment operator */
		PersistentObjectSerializer(PersistentObjectSerializer&&) noexcept = delete; /**< Delete move constructor */
		PersistentObjectSerializer& operator=(PersistentObjectSerializer&&) noexcept = delete; /**< Delete move assignment operator */

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
		rm::PersistentObjectManager& manager() { return object_manager_; };

	private:
		rm::PersistentObjectManager object_manager_; /**< Factory to act as wrapper to cache, creating objects if required */
		std::string rule_datatype_{}; /**< Name of the type of data being processed */

	};

} // namespace rm::rule::parser