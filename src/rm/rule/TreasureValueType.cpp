#include <TreasureValueType.h>

namespace rm::rule::enums {

	const std::string TreasureValueType::toString(Type type) {
		using enum Type;

		switch (type) {
		case kVeryPoor: return "Very Poor";
		case kPoor: return "Poor";
		case kNormal: return "Normal";
		case kRich: return "Rich";
		case kVeryRich: return "Very Rich";
		case kSpecial: return "Special";
		default: return "";
		}
	}

	const std::optional<TreasureValueType::Type> TreasureValueType::fromString(std::string_view sv) {
		using enum Type;

		const std::string val = archid::lcaseconcat(sv);
		if (val == "verypoor") return kVeryPoor;
		if (val == "poor") return kPoor;
		if (val == "normal") return kNormal;
		if (val == "rich") return kRich;
		if (val == "veryrich") return kVeryRich;
		if (val == "special") return kSpecial;

		return {};
	}

	void TreasureValueType::fromString(std::string_view sv, TreasureValueType::Type& type) {
		std::optional<Type> opt_type = fromString(sv);
		if (opt_type) type = opt_type.value();
		else throw std::invalid_argument("Invalid string value for TreasureValueType::Type: " + std::string(sv));
	}

} // namespace rm::rule::enums