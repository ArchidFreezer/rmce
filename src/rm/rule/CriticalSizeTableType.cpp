#include "CriticalSizeTableType.h"

const std::string CriticalSizeTableType::toString(Type type) {
	using enum Type;

	switch (type) {
	case kNormal: return "Normal";
	case kLarge: return "Large";
	case kHuge: return "Super Large";
	default: return "";
	}
}

const std::optional<CriticalSizeTableType::Type> CriticalSizeTableType::fromString(std::string_view sv) {
	using enum Type;

	const std::string& val = lcaseconcat(sv);
	if (val == "normal") return kNormal;
	if (val == "large") return kLarge;
	if (val == "superlarge") return kHuge;

	return {};
}

void CriticalSizeTableType::fromString(std::string_view sv, CriticalSizeTableType::Type& type) {
	std::optional<Type> opt_type = fromString(sv);
	if (opt_type) type = opt_type.value();
	else throw std::invalid_argument("Invalid string value for CriticalSizeTableType::Type: " + std::string(sv));
}