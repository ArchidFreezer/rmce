#pragma once
#include <string>

/**
 * @class GameRuleData
 * 
 * @brief Polymorphic class that is the base for objects that store data on the rules of the game
 * 
 * This class is used as the base for all classes that contain objects with data for the game rules, e.g. Professions,
 * Languages, Skills, etc. 
 */
class GameRuleData {

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

};