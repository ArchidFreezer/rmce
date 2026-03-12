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
	friend class PersistentObjectManager; /**< PersistentObjectManager is a friend to allow it access to the private copy/maove constructores and assignment operators */

public:
	GameRuleData() = delete; /** < Delete the default constructor to ensure that an id is always provided when creating an object */

	virtual ~GameRuleData() = default; /**< Default destructor for polymorphism */

	/**
	 * @brief Get the unique id of the game data object
	 *
	 * Each object containing game rule data requires a unique identifier to allow it to be referenced. Having this virtual method
	 * in this base class ensures that all derived classes have to provide one.
	 *
	 * @return Identifier as a string reference.
	 */
	const std::string& id() const {
		return id_;
	}

	/**
	 * @brief Override the less than operator
	 *
	 * The creation of this overload allows all GameRuleData objects to be used as keys in sorted containers
	 *
	 * @param other GameRuleData object to compoare against
	 * @return `true` if this object is consdiered to be < \a other
	 * @return `false` if this object is not consdiered to be < \a other
	 */
	bool operator<(const GameRuleData& other) const {
		return (id_ < other.id_);
	}

	/**
	 * @brief Get the prefix of the unique id of the game data object
	 * @return Prefix of the unique id as a string view
	 */
	virtual std::string_view prefix() const = 0;

protected:
	/**
	 * @brief Constructor
	 * @param id Unique identifier for the object
	 */
	GameRuleData(std::string_view id) : id_{id} {
	}

private:
	std::string id_{}; /**< Unique identifier for the object */

	/*
	 * Make all the copy and move constructors and assignment operators private to ensure that the unique identifier is not accidentally copied or moved, which would lead to multiple
	 * objects having the same unique identifier, which would break the uniqueness requirement of the identifier
	 */
	GameRuleData(const GameRuleData& other) = default;            /**< Default copy constructor */
	GameRuleData& operator=(const GameRuleData& other) = default; /**< Default assignment operator */
	GameRuleData(GameRuleData&& other) = default;                 /**< Default move constructor */
	GameRuleData& operator=(GameRuleData&& other) = default;      /**< Default move assignment operator */
};

/**
 * @brief Concept that ensures that a templated type is derived from the GameRuleData class
 *
 * It is used when templating a class or method where it may be used as follows
 *
 * @code
 * template <game_rule_data_object T>
 * void someFunction(T obj) {
 *   // do something with obj
 * }
 * @endcode
 */
template<class T>
concept game_rule_data_object = std::is_base_of<GameRuleData, T>::value;

} // namespace rm::rule