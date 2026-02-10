#pragma once

#include <set>
#include <string>
#include <BookData.h>
#include <GameRuleData.h>
#include <RealmType.h>
#include <SpellListType.h>

/**
 * @class SpellListData
 * @brief Class representing a spell list available in the game
 */
class SpellListData : public GameRuleData {
public:

	SpellListData() = delete; /**< Default constructor is deleted to ensure the base class is initialised */

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
	 * @brief Set the book that the profession is defined in
	 * @param book BookData pointer to the book
	 */
	void setBook(const BookData& book) { book_.emplace(&book); }

	/**
	 * @brief Get the book that the profession is defined in
	 *
	 * The book is stored as a pointer and may not have been initialised so it is considered optional. A check should be made
	 * before using the value to determine if the book has been set yet:
	 * @code
	 * if (profession.book()) {                              // Check if the book has been set
	 *   const BookData* book = profession.book().value();   // Get the book pointer
	 *   // Use the book
	 * }
	 * @endcode
	 * or
	 * @code
	 * std::cout << (profession.book() ? profession.book().value()->name() : "Book not set") << std::endl;
	 * @endcode
	 * @return Pointer to the BookData containing the profession definition
	 */
	const std::optional<const BookData*> book() const { return book_; }

	/**
	 * @brief Add a realm to those the spell draws power from
	 * @param realm RealmType::Type spell power realm
	 */
	void addRealm(RealmType::Type realm) {
		realms_.emplace(realm);
	}

	/**
	 * @brief Get whether the spell list draws power from a spell realm
	 * @param realm RealmType::Type spell power realm
	 * @return `true` if the list draws power from the realm
	 * @return `false` if the list does not draw power from the realm
	 */
	bool isRealm(RealmType::Type realm) const { return (realms_.find(realm) != realms_.end()); }

	/**
	 * @brief Get the realms that the spell list draws power from
	 * @return td::set<RealmType::Type> spell power realms
	 */
	const std::set<RealmType::Type> realms() { return realms_; }

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
	std::set< RealmType::Type> realms_{};/**< Realm(s) that the spells on the list draw power from */
	std::optional<const BookData*> book_{ std::nullopt }; /**< Book that the spell list is described in */
	SpellListType::Type type_{ SpellListType::Type::kOpen }; /**< Type of spell list */
	bool evil_{}; /** Whether the list is considered evil */
	bool summoning_{}; /** Whether spells on the list summon entities */
};