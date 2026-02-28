#pragma once
#include <string>

#include <Persistent.h>

/**
 * @namespace rm::rule
 * @brief Rules of the game represented by classes and data structures
 * 
 * The rules are the objects that are used as lookups or references when creating game objects such as races or professions and lookup tables for things such as skills, languages, etc.
 */
namespace rm::rule {

	/**
	 * @class GameRuleData
	 *
	 * @brief Polymorphic class that is the base for objects that store data on the rules of the game
	 *
	 * This class is used as the base for all classes that contain objects with data for the game rules, e.g. Professions, Languages, Skills, etc.
	 * 
	 * All the copy and move constructors and assignment operators ar deleted to ensure that the unique identifier is not accidentally copied or moved, which would lead to multiple
	 * objects having the same unique identifier, which would break the uniqueness requirement of the identifier and cause issues with serialisation and deserialisation.
	 */
	class GameRuleData : public rm::Persistent {
		friend class GameRuleDataFactory; /**< Factory class is a friend to allow it to set the unique identifier of the object when it is created */

	public:
		/**
		 * @brief Default constructor
		 */
		GameRuleData() = delete;

		/**
		 * @brief Constructor
		 * @param id Unique identifier for the object
		 */
		GameRuleData(std::string_view id) : id_{ id } {}

		/**
		 * @brief Default destructor
		 *
		 * This is made virtual to define the class as polymorphic as a standard best practice.
		 */
		virtual ~GameRuleData() = default;

		/**
		 * @brief Get the unique id of the game data object
		 *
		 * Each object containing game rule data requires a unique identifier to allow it to be referenced. Having this virtual method
		 * in this base class ensures that all derived classes have to provide one.
		 *
		 * @return Identifier as a string reference.
		 */
		const std::string& id() const { return id_; }

		/**
		 * @brief Override the less than operator
		 *
		 * The creation of this overload allows all GameRuleData objects to be used as keys in sorted containers
		 *
		 * @param other GameRuleData object to compoare against
		 * @return `true` if this object is consdiered to be < \a other
		 * @return `false` if this object is not consdiered to be < \a other
		 */
		bool operator<(const GameRuleData& other) const { return (id_ < other.id_); }

		/**
		 * @brief Creates a string that can be used as a unique identifier for an object
		 *
		 * Each object requires a unique identifier and this is a helper function that can create one in a standard format. The
		 * output is a string that is a combination of the type and val parameters that only contain ucase letters, numbers and
		 * the underscore character. Any character that is not an alphanum will be converted to an underscore with consecutive
		 * underscores being discarded.
		 *
		 * @param type The type of data object
		 * @param val The unique name of the data object
		 * @return String containing an id in standard format
		 */
		static const std::string generateId(std::string_view type, std::string_view val);

	private:
		std::string id_{}; /**< Unique identifier for the object */

		/*
	 * Make all the copy and move constructors and assignment operators private to ensure that the unique identifier is not accidentally copied or moved, which would lead to multiple
	 * objects having the same unique identifier, which would break the uniqueness requirement of the identifier
	 */
		GameRuleData(const GameRuleData& other) = default; /**< Default copy constructor */
		GameRuleData& operator=(const GameRuleData& other) = default; /**< Default assignment operator */
		GameRuleData(GameRuleData&& other) = default; /**< Default move constructor */
		GameRuleData& operator=(GameRuleData&& other) = default; /**< Default move assignment operator */

	};

	/**
	 * @brief Concept that ensures that a templated type is derived from the GameRuleData class
	 *
	 * It is used when templating a class or method where it may be used as follows
	 *
	 * @code
	 * template <GameRuleDataObject T>
	 * void someFunction(T obj) {
	 *   // do something with obj
	 * }
	 * @endcode
	 */
	template<class T>
	concept GameRuleDataObject = std::is_base_of<GameRuleData, T>::value;


} // namespace rm::rule