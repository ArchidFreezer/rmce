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
	GameRuleData() = default;

	/**
	 * @brief Default destructor
	 * 
	 * This is made virtual to define the class as polymorphic as a standard practice, even though there is another virtial method.
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
	virtual const std::string& id() = 0;

};
