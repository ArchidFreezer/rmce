#include <LanguageAbility.h>

void LanguageAbility::updateSomanticRanks(int ranks) {
	if (!language_->isSomantic()) throw InvalidLanguageRank("Cannot update ranks as language " + language_->name() + "has no somantic component.");
	if (somantic_ + ranks < 0) throw InvalidLanguageRank("Cannot update ranks as " + language_->name() + " somantic ability only has " + std::to_string(somantic_) + " ranks.");
	somantic_ += ranks;
}

void LanguageAbility::updateSpokenRanks(int ranks) {
	if (!language_->isSpoken()) throw InvalidLanguageRank("Cannot update ranks as language " + language_->name() + "has no spoken component.");
	if (spoken_ + ranks < 0) throw InvalidLanguageRank("Cannot update ranks as " + language_->name() + " spoken ability only has " + std::to_string(spoken_) + " ranks.");
	spoken_ += ranks;
}

void LanguageAbility::updateWrittenRanks(int ranks) {
	if (!language_->isWritten()) throw InvalidLanguageRank("Cannot update ranks as language " + language_->name() + "has no written component.");
	if (written_ + ranks < 0) throw InvalidLanguageRank("Cannot update ranks as " + language_->name() + " written ability only has " + std::to_string(written_) + " ranks.");
	written_ += ranks;
}
