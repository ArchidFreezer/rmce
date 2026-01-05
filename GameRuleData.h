#pragma once
#include <string>

// Base class for objects that contain the data that populates the game rules such as skills and professions, etc.
class GameRuleData {

public:
	GameRuleData() = default;
	virtual ~GameRuleData() = default;

	virtual const std::string& getID() = 0;

};
