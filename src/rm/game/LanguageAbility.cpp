#include <LanguageAbility.h>

namespace rm::game::character {

	void LanguageAbility::updateSomanticRanks(int ranks) {
		if (!language_->isSomatic()) throw InvalidLanguageRank("Cannot update ranks as language " + language_->name() + " has no somantic form.");
		if (somatic_ + ranks < 0) throw InvalidLanguageRank("Cannot update ranks by " + std::to_string(ranks) + " as " + language_->name() + " somantic ability only has " + std::to_string(somatic_) + " ranks.");
		somatic_ += ranks;
	}

	void LanguageAbility::updateSpokenRanks(int ranks) {
		if (!language_->isSpoken()) throw InvalidLanguageRank("Cannot update ranks as language " + language_->name() + " has no spoken form.");
		if (spoken_ + ranks < 0) throw InvalidLanguageRank("Cannot update ranks by " + std::to_string(ranks) + " as " + language_->name() + " spoken ability only has " + std::to_string(spoken_) + " ranks.");
		spoken_ += ranks;
	}

	void LanguageAbility::updateWrittenRanks(int ranks) {
		if (!language_->isWritten()) throw InvalidLanguageRank("Cannot update ranks as language " + language_->name() + " has no written form.");
		if (written_ + ranks < 0) throw InvalidLanguageRank("Cannot update ranks by " + std::to_string(ranks) + " as " + language_->name() + " written ability only has " + std::to_string(written_) + " ranks.");
		written_ += ranks;
	}

} // namespace rm::game