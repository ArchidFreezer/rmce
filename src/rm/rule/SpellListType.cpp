#include <SpellListType.h>

namespace rm {

	const std::string SpellListType::toString(Type type) {
		using enum Type;

		switch (type) {
		case kBase: return "Base";
		case kClosed: return "Closed";
		case kOpen: return "Open";
		case kTrainingPackage: return "Training Package";
		case kRacial: return "Racial";
		default: return "";
		}
	}

	const std::optional<SpellListType::Type> SpellListType::fromString(std::string_view sv) {
		using enum Type;

		const std::string& val = archid::lcaseconcat(sv);
		if (val == "base") return kBase;
		if (val == "closed") return kClosed;
		if (val == "open") return kOpen;
		if (val == "trainingpackage") return kTrainingPackage;
		if (val == "racial") return kRacial;

		return {};
	}

} // namespace rm