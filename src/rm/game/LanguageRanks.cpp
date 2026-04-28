#include <LanguageRanks.h>

namespace rm::game::character {

	void LanguageRanks::updateSomanticRanks(int ranks) {
		if (!language_->isSomatic()) throw InvalidLanguageRank("Cannot update ranks as language " + language_->name() + " has no somatic form.");
		if (somatic_ + ranks < 0) throw InvalidLanguageRank("Cannot update ranks by " + std::to_string(ranks) + " as " + language_->name() + " somatic ability only has " + std::to_string(somatic_) + " ranks.");
		somatic_ += ranks;
	}

	void LanguageRanks::updateSpokenRanks(int ranks) {
		if (!language_->isSpoken()) throw InvalidLanguageRank("Cannot update ranks as language " + language_->name() + " has no spoken form.");
		if (spoken_ + ranks < 0) throw InvalidLanguageRank("Cannot update ranks by " + std::to_string(ranks) + " as " + language_->name() + " spoken ability only has " + std::to_string(spoken_) + " ranks.");
		spoken_ += ranks;
	}

	void LanguageRanks::updateWrittenRanks(int ranks) {
		if (!language_->isWritten()) throw InvalidLanguageRank("Cannot update ranks as language " + language_->name() + " has no written form.");
		if (written_ + ranks < 0) throw InvalidLanguageRank("Cannot update ranks by " + std::to_string(ranks) + " as " + language_->name() + " written ability only has " + std::to_string(written_) + " ranks.");
		written_ += ranks;
	}

} // namespace rm::game