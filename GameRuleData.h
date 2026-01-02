#pragma once
#include <string_view>

#include "GameRuleDataType.h"

// Base class for objects that contain the data that populates the game rules such as skills and professions, etc.
class GameRuleData {

public:
	GameRuleData() = delete;
	GameRuleData(GameRuleDataType type) : type_{ type } {}
	~GameRuleData() = default;

	const GameRuleDataType getGameRuleDataType() const { return type_; }

	virtual const std::string& getID() = 0;

private:
	// The type is a unique key to identify the data type
	const GameRuleDataType type_{};
};
