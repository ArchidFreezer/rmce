#pragma once

#include <string>
#include <GameRuleData.h>
#include <RealmType.h>
#include <SpellListType.h>

/**
 * @class SpellListData
 * @brief Class representing a spell list available in the game
 */
class SpellListData : public GameRuleData {
public:
	/**
	 * @brief Constructor
	 * @param id Unique identifier for the spell list
	 * @param name Name of the spell llist as seen in game
	 * @param realm The realm of spells on the list
	 * @param type The type of spell list
	 * @param evil Whether the spell list is considered evil in nature
	 * @param summoning Whether the spell list contains summoning spells
	 */
	SpellListData(std::string_view id, std::string_view name, RealmType::Type realm, SpellListType::Type type, bool evil, bool summoning) :
		GameRuleData(id),
		name_{ name },
		realm_{ realm },
		type_{ type },
		evil_{ evil },
		summoning_{ summoning } {}

	/**
	 * @brief Constructor
	 * @param id Unique identifier for the spell list
	 */
	SpellListData(std::string_view id) : GameRuleData(id) {}

	/**
	 * @brief Set the name of the spell list
	 * @param name Spell list name
	 */
	void setName(std::string_view name) { name_ = name; }

	/**
	 * @brief Get the name of the spell list
	 * @return Spell list name as a string reference
	 */
	const std::string& name() const { return name_; }

	/**
	 * @brief Set the realm of spells on the list
	 * @param realm RealmType::Type to set
	 */
	void setRealm(RealmType::Type realm) { realm_ = realm; }

	/**
	 * @brief Get the realm of spells on the list
	 * @return RealmType::Type of spells
	 */
	const RealmType::Type realm() const { return realm_; }

	/**
	 * @brief Set the type of spell list
	 * @param type SpellListType::Type of list
	 */
	void setType(SpellListType::Type type) { type_ = type; }

	/**
	 * @brief Get the type of spell list
	 * @return SpellListType::Type of spell list
	 */
	const SpellListType::Type type() const { return type_; }

	/**
	 * @brief Set whether the spell list is considered to be evil
	 * @param evil Whether the spell list is evil
	 */
	void setIsEvil(bool evil) { evil_ = evil; }

	/**
	 * @brief get whether the spell list is considered evil
	 * @return `true` if the spell list is considered evil
	 * @return `false` if the spell list is not considered evil
	 */
	bool isEvil() const { return evil_; }

	/**
	 * @brief Set whether any spells on the list summon creatures or other entities
	 * @param summoning If any spells summon entities
	 */
	void setIsSummoning(bool summoning) { summoning_ = summoning; }

	/**
	 * @brief Get whether any spells on the list summon creatures or other entities
	 * @return `true` if spells summon entities
	 * @return `false` if spells do not summon entities
	 */
	bool isSummoning() const { return summoning_; }

private:
	std::string name_{}; /**< Name of teh spell list */
	RealmType::Type realm_{RealmType::Type::kArms}; /**< Realm of spells on the list */
	SpellListType::Type type_{ SpellListType::Type::kOpen }; /**< Type of spell list */
	bool evil_{}; /** Whether the list is considered evil */
	bool summoning_{}; /** Whether spells on the list summon entities */
};