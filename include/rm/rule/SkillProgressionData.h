#pragma once

namespace rm::rule {

	/**
	 * @class SkillProgressionData
	 *
	 * @brief Class representing the bonus a skill progression provides for a skill or skill category
	 *
	 * Each skill or skill category that is developed by a character has a progression associated with it which determines
	 * the bonus applied given the number of skill ranks developed. Body Development and Power Point Development are considered
	 * skills in ths regard and follow the same rules.
	 *
	 */
	class SkillProgressionData {
	public:
		/**
		 * @brief Get the skill bonus provided by the progression type for the number of ranks defined in the parameter
		 * @param ranks Number of ranks to get the bonus for
		 * @return int containing the skill bonus
		 */
		virtual int bonus(int ranks) const = 0;
	};

} // namespace rm::rule