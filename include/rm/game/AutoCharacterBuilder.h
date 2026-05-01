#pragma once

#include <PersistentObjectManager.h>
#include <CharacterBuilder.h>

namespace rm::serial {
class AutoCharacterBuilderSerializer;
} // namespace rm::serial

namespace rm::game::character {
class CharacterBuilder; // Forward declaration to break the circular include with CharacterBuilder.h

using namespace rm::rule;

/**
 * @class AutoCharacterBuilder
 * @brief A helper class that provides a convenient interface for automatically generating a character with random choices for all attributes and stats.
 *
 * The AutoCharacterBuilder class is designed to simplify the process of creating a character by automatically generating random choices for all attributes and stats based on the rules and data defined in the game. It can be used for quickly creating NPCs or for players who want a more randomized character creation experience.
 */
class AutoCharacterBuilder : public GameObject {
	/* We need to make the serializer a friend to allow it access to the private members of this class for serialisation and deserialisation */
	friend class rm::serial::AutoCharacterBuilderSerializer;

public:
	/**
	 * @brief Automatically roll stats and assign them to the character being built.
	 *
	 * This method is used to automate the stat generation step. It will roll temporary and potential values and assign them to character stats based on the chosen profession.
	 * The default parameter values are those used for PCs and may be adjusted for NPCs as desired based on whether we are creating MOBs or unique characters.
	 *
	 * @param builder The CharacterBuilder object for which to automatically generate stats. This builder will be modified by this function to set the generated stats for the character being built.
	 * @param min The minimum temporary value to roll for each stat.
	 * @param primeFloorMin The minimum value for the prime stats.
	 * @param numPrimeFloorMin The number of prime floor stats that must meet or exceed the primeFloorMin value.
	 */
	void autoStats(CharacterBuilder& builder, int min = 25, int primeFloorMin = 90, int numPrimeFloorMin = 2) const;

	/**
	 * @brief Get the name of this AutoCharacterBuilder object.
	 * 
	 * The name of the object will be either the name of the associated CharacterBuilder object if it has been generated automatically, or may be 
	 * @return A string representing the name of this AutoCharacterBuilder object, which is used for display purposes and may not be unique.
	 */
	std::string name() const {
		return name_;
	}

	/**
	 * @brief Set the name of this AutoCharacterBuilder object.
	 * 
	 * The name of the object will be either the name of the associated CharacterBuilder object if it has been generated automatically, or may be set to a custom value for display purposes. This function allows for setting a custom name for the AutoCharacterBuilder object, which can be useful for distinguishing between different automatically generated characters or for providing more descriptive names in certain contexts.
	 * 
	 * @param name A string representing the name to set for this AutoCharacterBuilder object, which is used for display purposes and may not be unique.
	 */
	void setName(const std::string& name) {
		name_ = name;
	}

private:
	std::string name_{""}; /**< The name of this AutoCharacterBuilder object, which is used for display purposes and may not be unique. */
};

} // namespace rm::game::character
