#pragma once

#include <map>
#include <string_view>
#include <Dice.h>
#include <GameRuleData.h>

/**
 * @class TrainingPackageData
 * @brief A training package is a group of benefits (and sometimes disadvantages) that can be developed using development
 * points
 * 
 * The skills and abilities gained through a package represent long periods of exposure or training (sometimes years). If
 * these packages are developed after apprenticeship development, the character should be required to spend time in
 * training (the time will vary from package to package).
 * 
 * There are two different types of training packages: vocational and lifestyle. Most of the time, a character may have
 * only one lifestyle training package (GM discretion). However, he can have as many vocational training packages as he can
 * afford to purchase.
 * 
 * <em>Lifestyle</em> - These training packages represent groups of skills and abilities that would be gained by living a
 * particular lifestyle. Lifestyle packages should be hard to acquire, and take long periods of time when they are allowed.
 * 
 * <em>Vocational</em> - These training packages represent specific occupations in which a character may become proficient. He
 * will gain groups of skills and abilities that would necessarily be developed if the character were in the occupation.
 */
class TrainingPackageData : public GameRuleData {
public:
	/**
	 * @brief Delete the default constructor to ensure we initialise the base class
	 */
	TrainingPackageData() = delete;

	/**
	 * @brief Basic constructor to initialise the base class
	 * @param id Unique identifier of the training package
	 */
	TrainingPackageData(std::string_view id) : GameRuleData(id) {}

	/**
	 * @brief Set the name of the training package
	 * @param name Training package name
	 */
	void setName(std::string_view name) { name_ = name; }

	/**
	 * @brief Get the name of the training package
	 * @return Training package name as a string reference
	 */
	const std::string& name() const { return name_; }

	/**
	 * @brief Set the description of what defines the training package
	 * @param description std::string_view training package description
	 */
	void setDescription(std::string_view description) { description_ = description; }

	/**
	 * @brief Get the description of what the defines training package
	 * @return std::string reference of the description
	 */
	const std::string& description() const { return description_; }

	/**
	 * @brief Set the book that the training package is defined in
	 * @param book BookData pointer to the book
	 */
	void setBook(const BookData& book) { book_ = &book; }

	/**
	 * @brief Get the book that the training package is defined in
	 *
	 * @return BookData containing the training package definition
	 */
	const BookData& book() const { return *book_; }

	/**
	 * @brief Add a qualifier that, if met, reduces the cost of the training package
	 * 
	 * All conditions of a qualifier must be met in order to receive the reduced cost
	 * 
	 * @param qualifier Description of the qualifier
	 * @param reduction Reduction in package cost
	 */
	void addQualifier(std::string_view qualifier, int reduction) { qualifiers_.emplace(qualifier, reduction); }

	/**
	 * @brief Get a collection of qualifiers that, if met, reduce the cost of the training package
	 * 
	 * Only a single discout can apply so if a character meets the requireemnts of more than one qualifier the one with the
	 * biggest discount should be applied.
	 * 
	 * @return Map of qualifier text and associated cost reduction
	 */
	const std::map<std::string, int> qualifiers() const { return qualifiers_; }

	/**
	 * @brief Set whether non spell users may take the package
	 * 
	 * Some training packages are only for semi, hybrid and pure spell users
	 * @param caster_only `true` if the package is only for spell users; `false` otherwise
	 */
	void setSpellUserOnly(bool caster_only) { caster_only_ = caster_only; }

	/**
	 * @brief Get whether non spell users may take the package
	 * 
	 * @return `true` if only semi, hybrid and pure spell users may take the package
	 * @return `false` if the package is available to any profession
	 */
	bool spellUserOnly() const { return caster_only_; }

	/**
	 * @brief Set whether the package is lifestyle based
	 * 
	 * @param lifestyle `true` if the package is a lfestyle package; `false` if the package is vocational
	 */
	void setLifestyle(bool lifestyle) { lifestyle_ = lifestyle; }

	/**
	 * @brief Get whether the package is lifestyle based
	 * @return `true` if the package is lifestyle based
	 * @return `false` if the package is not lifestyle based
	 */
	bool lifestyle() const { return lifestyle_; }

	/**
	 * @brief Set the number of months to acquire the package
	 * 
	 * This is the amount of time the character needs to train before gaining the benefits of the package. This time should
	 * be modified by a percentage equal to triple the character’s SD stat bonus (expressed as a percentage).
	 * 
	 * Example: If a character has a +5 SD stat bonus, he would gain the benefits of a training package 15% earlier than
	 * someone with a +0 SD bonus (i.e., 85% of the normal time). However, if the character has a -5 SD stat bonus, he
	 * would take 15% longer to gain the benefit (i.e., 115% of the normal time). 
	 * 
	 * @param months Number of months to acquire the package
	 */
	void setTimeToAcquire(int months) { time_to_acquire_ = months; }

	/**
	 * @brief Set the number of months to acquire the package
	 *
	 * This is the amount of time the character needs to train before gaining the benefits of the package. This time should
	 * be modified by a percentage equal to triple the character’s SD stat bonus (expressed as a percentage).
	 *
	 * Example: If a character has a +5 SD stat bonus, he would gain the benefits of a training package 15% earlier than
	 * someone with a +0 SD bonus (i.e., 85% of the normal time). However, if the character has a -5 SD stat bonus, he
	 * would take 15% longer to gain the benefit (i.e., 115% of the normal time).
	 *
	 * @return Number of months to acquire the package
	 */
	int timeToAcquire() const { time_to_acquire_; }

	void setStaringMoneyChange(Dice change) { starting_money_change_ = change; }

	Dice startingMoneyChange() const { return starting_money_change_; }

private:
	std::string name_{}; /**< Name of the profession */
	std::string description_{}; /**< General description of the profession */
	const BookData* book_{}; /**< Book that the profession is described in */
	std::map<std::string, int> qualifiers_{}; /**<  Requirements, if met, that reduce the cost of the package */
	bool caster_only_{}; /**< Whether non spell user professions may take the package */
	bool lifestyle_{}; /**< Whether the package is a lifestyle package */
	int time_to_acquire_{}; /**< Time, in months, to acquire the package */
	Dice starting_money_change_{Dice(0)}; /**< Change in starting money a character gains during apprenticeship */

};
