#include <filesystem>
#include <iostream>
#include <PersistentCache.h>
#include <PersistentObjectManager.h>
#include <CharacterBuilder.h>


using namespace rm;

int main() {
	std::cout << "Current path is " << std::filesystem::current_path() << '\n';

	// Create the cache to store the game data
	PersistentCache cache{};
	PersistentObjectManager object_manager{cache};

	try {
		using namespace rm::game::character;
		
		CharacterBuilder builder{};

		
	} catch (std::runtime_error e) {
		std::cout << e.what() << std::endl;
	}

	return 0;
}
