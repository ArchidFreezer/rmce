#pragma once
#include <string>
#include <string_view>
#include <GameRuleData.h>
#include <ArmourType.h>

/**
 * @class ArmourTypeData
 * 
 * Armour types are designed to reflect the protective capability of one’s covering. Heavier coverings (e.g., ATs 15, 16,
 * 19, 20, etc.) are usually much more cumbersome than light armour or mere clothing. It is easier to make contact with
 * combatants in heavy armour in combat, but it resists penetration better than the less cumbersome armour types. Therefore,
 * "critical strikes" achieved against the more cumbersome armour types will be less frequent and lower in severity.
 * However attacks resulting in just concussion hits will be more frequent (i.e., the hindrance due to these coverings make
 * the target easier to strike but harder to seriously damage).
 * 
 * Less cumbersome armour types have the advantage of providing the wearer with mobility. Fewer swings will make contact
 * because the agile defender will completely avoid the blows. However, when a swing does make contact, the result is
 * usually more severe than it would be against the more cumbersome armour types: frequent and severe criticals along with
 * more concussion hits are the rule.
 * 
 * @brief Class representing data about armour types
 */
class ArmourTypeData : public GameRuleData {
public:
	/**
	 * @brief Constructor to fully populate the object
	 * @param id Unique identifier for the armour type
	 * @param type Actual ArmourType that the data represents
	 * @param name Name of the armour type as seen in-game
	 * @param description Description of what the armour type consists of
	 * @param min_manoeuvre_mod The minimum modification applied to manoeuvres that the armour applies
	 * @param max_manoeuvre_mod The maximum modification applied to manoeuvres that the armour applies
	 * @param missile_attack_penalty The penalty applied to missile weapons attacks
	 * @param quickness_penalty The penalty applied to the weaerers Quickness bonus
	 * @param animal_only Whether the armour type is for animal skin/hide only
	 * @param includes_greaves Whether the armour type is considered to have greaves
	 */
	ArmourTypeData(std::string_view id, ArmourType::Type type, std::string_view name, std::string_view description, int min_manoeuvre_mod, int max_manoeuvre_mod, int missile_attack_penalty, int quickness_penalty, bool animal_only, bool includes_greaves) :
		GameRuleData(id),
		type_{ type },
		name_{ name },
		description_{ description },
		min_manoeuvre_mod_{ min_manoeuvre_mod },
		max_manoeuvre_mod_{ max_manoeuvre_mod },
		missile_attack_penalty_{ missile_attack_penalty_ },
		quickness_penalty_{ quickness_penalty },
		animal_only_{ animal_only },
		includes_greaves_{ includes_greaves } {}

	/**
	 * @brief Constructor
	 * @param id Unique identifier for the book
	 */
	ArmourTypeData(std::string_view id) : GameRuleData(id) {}

	/**
	 * @brief Set the ArmourType::Type enumeration value of armour
	 * @param type ArmourType::Type type of armour
	 */
	void setType(ArmourType::Type type) { type_ = type; }

	/**
	 * @brief Get the ArmourType::Type enumeration value of armour
	 * @return ArmourType::Type type of armour
	 */
	ArmourType::Type type() const { return type_; }

	/**
 * @brief Set the name of the armour type as seen in-game
 * @param name Armour type name
 */
	void setName(std::string_view name) { name_ = name; }

	/**
	 * @brief Get the name of the armour type as seen in-game
	 * @return Armour type name as a string reference
	 */
	const std::string& name() const { return name_; }

	/**
	 * @brief Set the description of what the armour type consists of
	 * @param description std::string_view armour type description
	 */
	void setDescription(std::string_view description) { description_ = description_; }

	/**
	 * @brief Get the description of what the armour type consists of
	 * @return std::string reference ot the decsription
	 */
	const std::string& description() const { return description_; }

	/**
	 * @brief Set the minimum modification to manoeuvres wearing the armour applies
	 * 
	 * Wearing some types of armour can restrict movement and this applies a penalty when trying to perform some manoeuvres.
	 * 
	 * It is possible to develop skill in armour types that reduces this penalty but it may not be possible to remove it,
	 * this value defines the minimum that the penalty may be reduced to when fully trained in the armour.
	 *
	 * @param mod Minimum modification possible when fully trained in the armours use
	 */
	void setMinManoeuvreMod(int mod) { min_manoeuvre_mod_ = mod; }

	/**
	 * @brief Get the minimum modification to manoeuvres wearing the armour applies
	 *
	 * Wearing some types of armour can restrict movement and this applies a penalty when trying to perform some manoeuvres.
	 *
	 * It is possible to develop skill in armour types that reduces this penalty but it may not be possible to remove it,
	 * this value defines the minimum that the penalty may be reduced to when fully trained in the armour.
	 *
	 * @return Minimum modification possible when fully trained in the armours use
	 */
	int minManoeuvreMod() const { return min_manoeuvre_mod_; }

	/**
	 * @brief Set the maximum modification to manoeuvres wearing the armour applies
	 *
	 * Wearing some types of armour can restrict movement and this applies a penalty when trying to perform some manoeuvres.
	 *
	 * It is possible to develop skill in armour types that reduces this penalty; this value defines the penalty that is
	 * applied if the wearer has not been trained at all in manoeuverring in the armour.
	 *
	 * @param mod Minimum modification possible when fully trained in the armours use
	 */
	void setMaxManoeuvreMod(int mod) { max_manoeuvre_mod_ = mod; }

	/**
	 * @brief Get the minimum modification to manoeuvres wearing the armour applies
	 *
	 * Wearing some types of armour can restrict movement and this applies a penalty when trying to perform some manoeuvres.
	 *
	 * It is possible to develop skill in armour types that reduces this penalty; this value defines the penalty that is
	 * applied if the wearer has not been trained at all in manoeuverring in the armour.
	 *
	 * @return Minimum modification possible when fully trained in the armours use
	 */
	int maxManoeuvreMod() const { return max_manoeuvre_mod_; }

	/**
	 * @brief Sets the penalty that is applied to missile attack OB when wearing this armour
	 * 
	 * The missile attack penalty acts as a modification to the missile OB for a combatant wearing the given armor. This is
	 * meant to reflect the disadvantageous effect of armor worn on the arms and hands of a combatant.
	 * 
	 * @param penalty Modification to missile OB
	 */
	void setMissileAttackPenalty(int penalty) { missile_attack_penalty_ = penalty; }

	/**
	 * @brief Gets the penalty that is applied to missile attack OB when wearing this armour
	 *
	 * The missile attack penalty acts as a modification to the missile OB for a combatant wearing the given armor. This is
	 * meant to reflect the disadvantageous effect of armor worn on the arms and hands of a combatant.
	 *
	 * @return int Modification to missile OB
	 */
	int missileAttackPenalty() const { return missile_attack_penalty_; }

	/**
	 * @brief Sets the reduction in combatant’s Quickness stat bonus for his DB
	 * 
	 * This penalty can only reduce the armor wearer’s Quickness stat bonus. It will not reduce a combatant’s overall DB below
	 * the level that it would be at with a zero Quickness stat bonus.
	 * 
	 * @param penalty Reduction in stat bonus
	 */
	void setQuicknessPenalty(int penalty) { quickness_penalty_ = penalty; }

	/**
	 * @brief Gets the reduction in combatant’s Quickness stat bonus for his DB
	 *
	 * This penalty can only reduce the armor wearer’s Quickness stat bonus. It will not reduce a combatant’s overall DB below
	 * the level that it would be at with a zero Quickness stat bonus.
	 *
	 * @return int Reduction in stat bonus
	 */
	int quicknessPenalty() const { return quickness_penalty_; }

	/**
	 * @brief Sets if the armour type is only appliable to animal skin
	 * 
	 * Some armour types are animal armors, natural body coverings with no normal armor equivalents. One cannot achieve such
	 * an AT without acquiring some enchanted and specially designed armor (e.g., magic fell beast skin).
	 * 
	 * @param animal_only Whether the armour type is for animal armour only
	 */
	void setIsAnimalOnly(bool animal_only) { animal_only_ = animal_only; }

	/**
	 * @brief Sets if the armour type is only appliable to animal skin
	 *
	 * Some armour types are animal armors, natural body coverings with no normal armor equivalents. One cannot achieve such
	 * an AT without acquiring some enchanted and specially designed armor (e.g., magic fell beast skin).
	 * 
	 * @return `true` if the armour type is animal covering only
	 * @return `false` if the armour type is standard armour
	 */
	bool animalOnly() const { return animal_only_; }

	/**
	 * @brief Set whether the armour has protection for the forearms and lower legs
	 * 
	 * Not all armour types have protection for the forearms or lower legs and this indicates whether this one does or not.
	 * The use of the word greaves does not mean that the greaves have to be separate items as they may be fully incorporated
	 * such as in full plate armour.
	 * 
	 * @param greaves Whether the armour is consdered to be protected as with greaves
	 */
	void setIncludesGreaves(bool greaves) { includes_greaves_ = greaves; }

	/**
	 * @brief Get whether the armour has protection for the forearms and lower legs
	 *
	 * Not all armour types have protection for the forearms or lower legs and this indicates whether this one does or not.
	 * The use of the word greaves does not mean that the greaves have to be separate items as they may be fully incorporated
	 * such as in full plate armour.
	 * 
	 * @return `true` if the armour covers the forearms and lower legs
	 * @return `false` if the armour does not cover the forearms and lower legs
	 */
	bool includesGreaves() const { return includes_greaves_; }

private:
	ArmourType::Type type_{}; /**< Actual ArmourType that the data represents */
	std::string name_{}; /** Name of the armour type as seen in-game */
	std::string description_{}; /** Description of what the armour type consists of */
	int min_manoeuvre_mod_{}; /** The minimum modification applied to manoeuvres that the armour applies */
	int max_manoeuvre_mod_{}; /** The maximum modification applied to manoeuvres that the armour applies */
	int missile_attack_penalty_{}; /** The penalty applied to missile weapons attacks */
	int quickness_penalty_{}; /** The penalty applied to the weaerers Quickness bonus */
	bool animal_only_{}; /** Whether the armour type is for animal skin/hide only */
	bool includes_greaves_{}; /** Whether the armour type is considered to have greaves */
};