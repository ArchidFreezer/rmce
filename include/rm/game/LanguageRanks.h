#pragma once

#include <stdexcept>
#include <string>
#include <LanguageData.h>

namespace rm::game::character {
	/**
	 * @brief Thrown when there is an attempt to modify the number of ranks in a language and the result would be invlaid
	 *
	 * Some languages do not have some components and any attempt to set any ranks in one of these is invalid. Attempting to
	 * set the number of ranks to a negative number is also an invalid operation.
	 */
	class InvalidLanguageRank : public std::runtime_error {
	public:
		/**
		 * @brief Exception constructor
		 *
		 * std::runtime_error will handle the string
		 *
		 * @param error String to display for the error
		 */
		InvalidLanguageRank(const std::string& error) : std::runtime_error{ error } {}
	};


	/**
	 * @class LanguageRanks
	 * @brief Class representing the ability a character has in a language
	 *
	 * A character may develop their knowledge of a language in the game, with the spoken, written and any somantic components
	 * being developed separately. This object contains the degree of knowledge that a character has in each comnponent of a
	 * specific language.
	 *
	 */
	class LanguageRanks {
	public:

		LanguageRanks(); /**< Default constructor */

		/**
		 * @brief Constructor setting the language the ability is in
		 * @param language LanguageData reference to the language
		 */
		LanguageRanks(const rule::LanguageData& language) : LanguageRanks(language, 0, 0, 0) {}

		/**
		 * @brief Constructor
		 * @param language LanguageData refence to the language
		 * @param spoken number of spoken ranks the character has
		 * @param written number of written ranks the character has
		 * @param somantic number of somantic ranks the character has
		 */
		LanguageRanks(const rule::LanguageData& language, int spoken, int written, int somantic) : language_{ &language }, spoken_{ std::max(0,spoken) }, written_{ std::max(0,written) }, somatic_{ std::max(0,somantic) } {}

		/**
		 * @brief Set the language the ability is in
		 * @param language LanguageData reference to the language
		 */
		void setLanguage(const rule::LanguageData& language) { language_ = &language; }

		/**
		 * @brief Get the name of the language
		 * @return string reference to the name
		 */
		const std::string& language() const { return language_->name(); }

		/**
		 * @brief Get the id of the language
		 * @return string referenmce to the language id
		 */
		const std::string& languageId() const { return language_->id(); }

		/**
		 * @brief Modify the number of ranks in the language the character has
		 *
		 * Unless the game world has extremely complicated languages or dialects adding ranks above 10 tends to be a waste
		 *
		 * @param ranks int number of ranks
		 * @throw InvalidLanguageRank if the language has no somantic component
		 * @throw InvalidLanguageRank if an attempt is made to set the number of ranks to a negative number
		 */
		void updateSomanticRanks(int ranks);

		/**
		 * @brief Get the number of somatic ranks
		 * @return somatic ranks
		 */
		int somatic() const { return isSomatic() ? std::max(0, somatic_) : 0; }

		/**
		 * @brief Check whether the language has a somatic component
		 * @return `true` if there is a somatic component
		 * @return `false` if there is no somatic component
		 */
		bool isSomatic() const { return language_->isSomatic(); }

		/**
		 * @brief Modify the number of ranks in the language the character has
		 *
		 * Unless the game world has extremely complicated languages or dialects adding ranks above 10 tends to be a waste
		 *
		 * @param ranks int number of ranks
		 * @throw InvalidLanguageRank if the language has no spoken component
		 * @throw InvalidLanguageRank if an attempt is made to set the number of ranks to a negative number
		 */
		void updateSpokenRanks(int ranks);

		/**
		 * @brief Get the number of spoken ranks
		 * @return spoken ranks
		 */
		int spoken() const { return isSpoken() ? std::max(0, spoken_) : 0; }

		/**
		 * @brief Check whether the language has a spoken comnponent
		 * @return `true` if there is a spoken component
		 * @return `false` if there is no spoken component
		 */
		bool isSpoken() const { return language_->isSpoken(); }

		/**
		 * @brief Modify the number of ranks in the language the character has
		 *
		 * Unless the game world has extremely complicated languages or dialects adding ranks above 10 tends to be a waste
		 *
		 * @param ranks int number of ranks
		 * @throw InvalidLanguageRank if the language has no written component
		 * @throw InvalidLanguageRank if an attempt is made to set the number of ranks to a negative number
		 */
		void updateWrittenRanks(int ranks);

		/**
		 * @brief Get the number of written ranks
		 * @return written ranks
		 */
		int written() const { return isWritten() ? std::max(0, written_) : 0; }

		/**
		 * @brief Check whether the language has a written comnponent
		 * @return `true` if there is a written component
		 * @return `false` if there is no written component
		 */
		bool isWritten() const { return language_->isWritten(); }

		/**
		 * @brief Overload the less than operator to allow for sorting of LanguageRanks objects
		 *
		 * The overload is based on the id of the language, as this is a unique identifier for the language and allows for consistent ordering of LanguageRanks objects based on the language they represent.
		 *
		 * @param other The other LanguageRanks object to compare against
		 * @return `true` if this LanguageRanks is considered less than the other; `false` otherwise
		 */
		bool operator<(const LanguageRanks& other) const {
			 return language_->id() < other.language_->id();
		}

	private:
		const rule::LanguageData* language_{}; /**< Language the character has any ability in */
		int somatic_{ 0 }; /** Ability in somantic component of the languiage */
		int spoken_{ 0 }; /** Ability in spoken component of the languiage */
		int written_{ 0 }; /** Ability in written component of the languiage */
	};

} // namespace rm::game::character