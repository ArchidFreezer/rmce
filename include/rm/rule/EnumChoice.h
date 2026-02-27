#pragma once
#include <ranges>
#include <set>
#include <string>
#include <typeinfo>

namespace rm {

	/**
	 * @class EnumChoice
	 * @brief Class representing a choice that a player has available from a set of Enum objects
	 *
	 * There are several times, especially during character development, where a player must select one or more options from
	 * a set of options. This class defines the options available and the number that may be selected.
	 *
	 * The class is not meant to be prescriptive on the relationship between the number of choices and the options available so
	 * for example it may be used for defining one or more skills from a skill category that a member of a race may select as
	 * everyman skills, or it may be used to define a number of skill ranks that may be distributed amonst a set if skills for
	 * a profession or traning package.
	 *
	 * @tparam T Enum object of the options
	 */
	template <typename EnumType>
	class EnumChoice {
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
		int numChoices() const { return num_choices_; }

		/**
		 * @brief Add an object to those that may be chosen from
		 * @param option Enum object that may be chosen
		 */
		template <typename EnumType>
		void addOption(EnumType option) { options_.insert(option); }

		/**
		 * @brief Get the number of options there are to be selected from
		 * @return int number of optopns
		 */
		int numOptions() const { return options_.size(); }

		/**
		 * @brief Gets a container with the options that may be selected from
		 * @return std::set of Enum objects
		 */
		template <typename EnumType>
		const std::set<EnumType> options() const {
			return options_;
		}

		/**
		 * @brief Check if a Enum object is one of the options
		 * @param object Enum to check
		 * @return `true` if the object is an option
		 * @return `false` if the object is not an option
		 */
		template <typename EnumType>
		bool isOption(EnumType object) const {
			for (auto& key : options_) {
				if (key == object) return true;
			}
			return false;
		}

		/**
		 * @brief Override the less than operator
		 *
		 * The creation of this overload allows all EnumChoice objects to be used as keys in sorted containers
		 *
		 * @param other EnumChoice object to compare against
		 * @return `true` if this object is consdiered to be < \a other
		 * @return `false` if this object is not consdiered to be < \a other
		 */
		bool operator<(const EnumChoice& other) const {
			int this_hash{};
			for (auto& option : options_) {
				this_hash += std::hash<std::string>{}(toString(option));
			}

			int that_hash{};
			for (auto& option : other.options_) {
				that_hash += std::hash<std::string>{}(toString(option));
			}

			return (this_hash < that_hash);
		}

	private:
		int num_choices_{ 0 }; /**< How many of the options may be chosen */
		std::set<EnumType> options_{}; /**< The container of options */
	};

} // namespace rm