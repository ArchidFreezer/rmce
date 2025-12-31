#pragma once
#include <string>

class ArmourType
{
	std::string m_name = "";
	std::string m_description = "";
	std::string m_skill = "";
	int m_min_manoeuvre_mod = 0;
	int m_max_manoeuvre_mod = 0;
	int m_missile_attack_penalty = 0;
	int m_quickness_penalty = 0;
	bool m_animal_only = false;
	bool m_includes_greaves = false;
};