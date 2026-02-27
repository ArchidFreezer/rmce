#include "AttackSizeType.h"

const std::string AttackSizeType::toString(Type size) {
	using enum Type;

	switch (size) {
	case kSmall: return "Small";
	case kMedium: return "Medium";
	case kLarge: return "Large";
	case kHuge: return "Huge";
	default: return "";
	}
}

const std::optional<AttackSizeType::Type> AttackSizeType::fromString(std::string_view sv) {
	using enum Type;

	const std::string val = archid::lcaseconcat(sv);
	if (val == "small") return kSmall;
	if (val == "medium") return kMedium;
	if (val == "large") return kLarge;
	if (val == "huge") return kHuge;

	return {};
}

void AttackSizeType::fromString(std::string_view sv, AttackSizeType::Type& type) {
	std::optional<Type> opt_type = fromString(sv);
	if (opt_type) type = opt_type.value();
	else throw std::invalid_argument("Invalid string value for AttackSizeType::Type: " + std::string(sv));
}