#pragma once
#include <ranges>
#include <set>
#include <GameRuleData.h>

/**
 * @class GameRuleDataChoice 
 * @brief Class representing a choice that a player has available from a set of GameRuleData objects
 * 
 * There are several times, especially during character development, where a player must select one or more options from
 * a set of options. This class defines the options available and the number that may be selected.
 * 
 * The class is not meant to be prescriptive on the relationship between the number of choices and the options available so
 * for example it may be used for defining one or more skills from a skill category that a member of a race may select as
 * everyman skills, or it may be used to define a number of skill ranks that may be distributed amonst a set if skills for
 * a profession or traning package.
 * 
 * @tparam T GameRuleData object of the options
 */
template <GameRuleDataObject T>
class GameRuleDataChoice {
public:

	/**
	 * @brief Set the number of the options that may be selected
	 * @param num_choices number of options
	 */
	void setNumChoices(int num_choices) { num_choices_ = num_choices; }

	/**
	 * @brief Get the number of the options that may be selected
	 * @return int number of choices
	 */
	int numChoices() const{ return num_choices_; }

	/**
	 * @brief Add an object to those that may be chosen from
	 * @param option GameRuleData object that may be chosen
	 */
	void addOption(const T& option) { options_.insert(&option); }

	/**
	 * @brief Get the number of options there are to be selected from
	 * @return int number of optopns
	 */
	int numOptions() const{ return options_.size(); }

	/**
	 * @brief Gets a container with the options that may be selected from
	 * @return std::set of GameRuleData objects
	 */
	const std::set<const T*> options() const {
		return options_;
	}

	/**
	 * @brief Check if a GameRuleData object is one of the options
	 * @param object GameRuleData to check
	 * @return `true` if the object is an option
	 * @return `false` if the object is not an option
	 */
	bool isOption(const T& object) const {
		for (auto& key :options_) {
			if (key->id() == object.id()) return true;
		}
		return false;
	}

private:
	int num_choices_{ 0 }; /**< How many of the options may be chosen */
	std::set<const T*> options_{}; /**< The container of options */
};
