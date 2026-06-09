#include <ForagablePreparationType.h>

namespace rm::rule::enums {

const std::string ForagablePreparationType::toString(Type type) {
	using enum Type;

	switch (type) {
	case kApply:
		return "Apply";
	case kBrew:
		return "Brew";
	case kIngest:
		return "Ingest";
	case kLiquid:
		return "Liquid";
	case kPaste:
		return "Paste";
	case kPowder:
		return "Powder";
	default:
		return "";
	}
}

const std::optional<ForagablePreparationType::Type> ForagablePreparationType::fromString(std::string_view sv) {
	using enum Type;

	const std::string& val = archid::lcaseconcat(sv);
	if (val == "apply")
		return kApply;
	if (val == "brew")
		return kBrew;
	if (val == "ingest")
		return kIngest;
	if (val == "liquid")
		return kLiquid;
	if (val == "paste")
		return kPaste;
	if (val == "powder")
		return kPowder;

	return {};
}

void ForagablePreparationType::fromString(std::string_view sv, ForagablePreparationType::Type& type) {
	std::optional<Type> opt_type = fromString(sv);
	if (opt_type)
		type = opt_type.value();
	else
		throw std::invalid_argument("Invalid string value for ForagablePreparationType::Type: " + std::string(sv));
}

} // namespace rm::rule::enums