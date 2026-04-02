#pragma once
#include <optional>
#include <string>
#include <StringUtils.h>

namespace rm::game::character {

/**
 * @class SkillDevelopmentCost
 * @brief Represents the number of development points a character has to spend to gain a rank in a skill
 *
 * Depending on the character profession and other factors the character may be able to purchase one or more ranks in a
 * skill when levelling up. This class defines how manyranks may be purchased and the cost of each.
 *
 * Each of the getter functions returns a std::optional value which should be checked before use and thene the value
 * retrieved, for example:
 * @code
 * SkillDevelopmentCost cost(3, 5);
 * if (cost.one()) {
 *   std::cout << "First rank cost is " << cost.one().value() << std::endl;
 * } else {
 *   std::cout << "Cannot purchase any ranks" << std::endl;
 * }
 * if (cost.two()) {
 *   std::cout << "Second rank cost is " << cost.two().value() << std::endl;
 * } else {
 *   std::cout << "Cannot purchase a second rank" << std::endl;
 * }
 * if (cost.three()) {
 *   std::cout << "Third rank cost is " << cost.three().value() << std::endl;
 * } else {
 *   std::cout << "Cannot purchase a third rank" << std::endl;
 * }
 * @endcode
 * This would produce the following output:
 * @code
 * First rank cost is 3
 * Second rank cost is 5
 * Cannot purchase a third rank
 * @endcode
 */
class SkillDevelopmentCost {
public:
	/**
	 * @brief Constructor defining a development where no ranks may be purchased
	 */
	SkillDevelopmentCost() : SkillDevelopmentCost(0, 0, 0, 0) {}
	/**
	 * @brief Constructor defining a development where only a single rank may be purchased each level
	 * @param one int Number of deveopment points to purchase first rank
	 */
	SkillDevelopmentCost(int one) : SkillDevelopmentCost(one, 0, 0, 0) {}
	/**
	 * @brief Constructor defining a development where two ranks may be purchased each level
	 * @param one int Number of deveopment points to purchase first rank
	 * @param two int Number of deveopment points to purchase second rank
	 */
	SkillDevelopmentCost(int one, int two) : SkillDevelopmentCost(one, two, 0, 0) {}
	/**
	 * @brief Constructor defining a development where three ranks may be purchased each level
	 * @param one int Number of deveopment points to purchase first rank
	 * @param two int Number of deveopment points to purchase second rank
	 * @param three int Number of deveopment points to purchase third rank
	 */
	SkillDevelopmentCost(int one, int two, int three) : SkillDevelopmentCost(one, two, three, 0) {}
	/**
	 * @brief Constructor defining a development where four ranks may be purchased each level
	 * @param one int Number of deveopment points to purchase first rank
	 * @param two int Number of deveopment points to purchase second rank
	 * @param three int Number of deveopment points to purchase third rank
	 * @param four int Number of deveopment points to purchase fourth rank
	 */
	SkillDevelopmentCost(int one, int two, int three, int four) : one_{ one }, two_{ two }, three_{ three }, four_{ four } {}

	/**
	 * @brief Constructor that takes a sting in the form of costs delimited by colons.
	 *
	 * An example @a cost would be "3:5" which would represent the ability to develop up to 2 ranks each level with the first
	 * rank costing 3 development points and the second 5.
	 *
	 * @param cost string containing he costs
	 */
	SkillDevelopmentCost(const std::string& cost) {
		std::vector<std::string> tokens = archid::tokenise(cost, ":");
		if (tokens.size() > 0) one_ = stoi(tokens[0]);
		if (tokens.size() > 1) two_ = stoi(tokens[1]);
		if (tokens.size() > 2) three_ = stoi(tokens[2]);
		if (tokens.size() > 3) four_ = stoi(tokens[3]);
	}

	/**
	 * @brief The cost in development points to purchase the first rank
	 *
	 * The return value is optional which needs to be tested before use. If it is std::nullopt then it indicates that no
	 * ranks may be purchased in the skill:
	 * @code
	 * if (skill_develpment_cost.first()) {
	 *   int first_rank_cost = skill_develpment_cost.first().value();
	 * } else {
	 *   // No ranks may be purchased
	 * }
	 * @endcode
	 * @return std::optional<int> cost, if available
	 */
	std::optional<int> first() const { if (one_) return one_; else return std::nullopt; }

	/**
	 * @brief The cost in development points to purchase a first rank
	 *
	 * The return value is optional which needs to be tested before use. If it is std::nullopt then it indicates that it is
	 * not possible to purchase a first rank:
	 * @code
	 * if (skill_develpment_cost.one()) {
	 *   int one_rank_cost = skill_develpment_cost.one().value();
	 * } else {
	 *   // No ranks may be purchased
	 * }
	 * @endcode
	 * @return std::optional<int> cost, if available
	 */
	std::optional<int> one() const { return first(); }

	/**
	 * @brief The cost in development points to purchase a second rank
	 *
	 * The return value is optional which needs to be tested before use. If it is std::nullopt then it indicates that it is
	 * not possible to purchase a second rank:
	 * @code
	 * if (skill_develpment_cost.second()) {
	 *   int second_rank_cost = skill_develpment_cost.second().value();
	 * } else {
	 *   // Two ranks may not be purchased
	 * }
	 * @endcode
	 * @return std::optional<int> cost, if available
	 */
	std::optional<int> second() const { if (two_) return two_; else return std::nullopt; }

	/**
	 * @brief The cost in development points to purchase two ranks
	 *
	 * The return value is optional which needs to be tested before use. If it is std::nullopt then it indicates that it is not possible to purchase two ranks:
	 * @code
	 * if (skill_develpment_cost.two()) {
	 *   int two_rank_cost = skill_develpment_cost.two().value();
	 * } else {
	 *   // Two ranks may not be purchased
	 * }
	 * @endcode
	 * @return std::optional<int> cost, if available
	 */
	std::optional<int> two() const {
		if (one() && two_) return (one().value() + two_);	else return std::nullopt;
	}

	/**
	 * @brief The cost in development points to purchase a third rank
	 *
	 * The return value is optional which needs to be tested before use. If it is std::nullopt then it indicates that it is not possible to purchase a third rank:
	 * @code
	 * if (skill_develpment_cost.third()) {
	 *   int third_rank_cost = skill_develpment_cost.third().value();
	 * } else {
	 *   // Three ranks may not be purchased
	 * }
	 * @endcode
	 * @return std::optional<int> cost, if available
	 */
	std::optional<int> third() const { if (three_) return three_; else return std::nullopt; }

	/**
	 * @brief The cost in development points to purchase three ranks
	 *
	 * The return value is optional which needs to be tested before use. If it is std::nullopt then it indicates that it is not possible to purchase three ranks:
	 * @code
	 * if (skill_develpment_cost.three()) {
	 *   int third_rank_cost = skill_develpment_cost.three().value();
	 * } else {
	 *   // Three ranks may not be purchased
	 * }
	 * @endcode
	 * @return std::optional<int> cost, if available
	 */
	std::optional<int> three() const {
		if (two() && three_) return (two().value() + three_); else return std::nullopt;
	}

	/**
	 * @brief The cost in development points to purchase a fourth rank
	 *
	 * The return value is optional which needs to be tested before use. If it is std::nullopt then it indicates that it is
	 * not possible to purchase a fourth rank:
	 * @code
	 * if (skill_develpment_cost.fourth()) {
	 *   int fourth_rank_cost = skill_develpment_cost.fourth().value();
	 * } else {
	 *   // Four ranks may not be purchased
	 * }
	 * @endcode
	 * @return std::optional<int> cost, if available
	 */
	std::optional<int> fourth() const { if (four_) return four_; else return std::nullopt; }

	/**
	 * @brief The cost in development points to purchase four ranks
	 *
	 * The return value is optional which needs to be tested before use. If it is std::nullopt then it indicates that it is not possible to purchase four ranks:
	 * @code
	 * if (skill_develpment_cost.four()) {
	 *   int fourth_rank_cost = skill_develpment_cost.four().value();
	 * } else {
	 *   // Four ranks may not be purchased
	 * }
	 * @endcode
	 * @return std::optional<int> cost, if available
	 */
	std::optional<int> four() const {
		if (three() && four_) return (three().value() + four_); else return std::nullopt;
	}

	/**
	 * @brief Get the maximum number of ranks that may be purchased in the skill
	 * @return int maximum number of ranks that may be purchased
	 */
	int maxRanks() const {
		if (four_) return 4;
		else if (three_) return 3;
		else if (two_) return 2;
		else if (one_) return 1;
		return 0;
	}

	/**
	 * @brief Get the maximum number of ranks that may be purchased in the skill with a given amount of development points
	 * @param cost int amount of development points available
	 * @return int maximum number of ranks that may be purchased with the given amount of development points
	 */
	int maxRanks(int cost) const {
		if (four() && cost >= four().value()) return 4;
		else if (three() && cost >= three().value()) return 3;
		else if (two() && cost >= two().value()) return 2;
		else if (one() && cost >= one().value()) return 1;
		return 0;
	}

	/**
	 * @brief Get the string representation of the costs with each valid cost delimited by a colon, e.g. 3:3:3
	 * @return string_view of the representation
	 */
	const std::string toString() const {
		return ""
			+ (first() ? std::to_string(one_) : "")
			+ (second() ? ":" + std::to_string(two_) : "")
			+ (third() ? ":" + std::to_string(three_) : "")
			+ (fourth() ? ":" + std::to_string(four_) : "");
	}

private:
	int one_{}; /**< Development point cost for first rank; 0 if unavailable */
	int two_{}; /**< Development point cost for second rank; 0 if unavailable */
	int three_{}; /**< Development point cost for third rank; 0 if unavailable */
	int four_{}; /**< Development point cost for fourth rank; 0 if unavailable */
};

} // namespace rm::game::character