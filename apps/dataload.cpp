#include <filesystem>
#include <iostream>
#include "PersistentCache.h"
#include "PersistentObjectManager.h"
#include <CharacterBuilder.h>
#include <Dice.h>

using namespace rm;
using namespace rm::game::character;

void setStat(Character& character, StatType::Type stat_type) {
	archid::Dice d100{100, 0, 0};
	int temp_value = 0;
	while (temp_value <= 25) {
		temp_value = d100.roll().result();
	}
	character.setStat(stat_type, temp_value);
	const Stat& stat = character.getStat(stat_type);
	std::cout << std::format("|{:^17}|{:^10}|{:^10}|{:^10}|\n", toString(stat_type), stat.temporary(), stat.potential(), stat.bonus());
}

int main() {
	std::cout << "Current path is " << std::filesystem::current_path() << '\n';

	// Create the cache to store the game data
	PersistentCache cache{};
	PersistentObjectManager object_manager{cache};

	try {
		
		Character& character = object_manager.get<Character>();
		//CharacterBuilder builder{};
		character.setName("Test Character");
		//std::cout << "|" << std::setw(17) << "Stat" << "|" << std::setw(10) << "Temp" << "|" << std::setw(10) << "Pot" << "|" << std::setw(10) << "Bonus" << "|" << std::endl;
		std::cout << std::format("|{:^17}|{:^10}|{:^10}|{:^10}|\n", "Stat", "Temp", "Pot", "Bonus");
		std::cout << std::format("|{:^17}|{:^10}|{:^10}|{:^10}|\n", "-----------------", "----------", "----------", "----------");
		setStat(character, StatType::Type::kAgility);
		setStat(character, StatType::Type::kConstitution);
		setStat(character, StatType::Type::kEmpathy);
		setStat(character, StatType::Type::kIntuition);
		setStat(character, StatType::Type::kMemory);
		setStat(character, StatType::Type::kPresence);
		setStat(character, StatType::Type::kQuickness);
		setStat(character, StatType::Type::kReasoning);
		setStat(character, StatType::Type::kSelfDiscipline);
		setStat(character, StatType::Type::kStrength);
		
	} catch (std::runtime_error e) {
		std::cout << e.what() << std::endl;
	}

	return 0;
}
