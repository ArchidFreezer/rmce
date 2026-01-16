#pragma once
#include <string>

/**
 * @class ArmourTypeData
 * @brief Class representing data about armour types
 */
class ArmourTypeData
{
	std::string name_ = "";
	std::string description_ = "";
	std::string skill_ = "";
	int min_manoeuvre_mod_ = 0;
	int max_manoeuvre_mod_ = 0;
	int missile_attack_penalty_ = 0;
	int quickness_penalty_ = 0;
	bool animal_only_ = false;
	bool includes_greaves_ = false;
};