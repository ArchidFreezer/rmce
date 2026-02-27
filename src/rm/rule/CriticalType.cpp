#include <CriticalType.h>

namespace rm::rule::enums {

	const std::string CriticalType::toString(Type type) {
		using enum Type;

		switch (type) {
		case kBrawling: return "Brawling";
		case kGrapple: return "Grapple";
		case kImpact: return "Impact";
		case kKrush: return "Krush";
		case kMartialArtsStrikes: return "Martial Arts Strikes";
		case kMartialArtsSweeps: return "Martial Arts Sweeps";
		case kPuncture: return "Puncture";
		case kSlash: return "Slash";
		case kSubdual: return "Subdual";
		case kTiny: return "Tiny";
		case kUnbalance: return "Unbalance";
		case kAether: return "Aether";
		case kCold: return "Cold";
		case kElectrical: return "Electrical";
		case kHeat: return "Heat";
		case kNether: return "Nether";
		default: return "";
		}
	}

	const std::optional<CriticalType::Type> CriticalType::fromString(std::string_view sv) {
		using enum Type;

		const std::string& val = archid::lcaseconcat(sv);
		if (val == "brawling") return kBrawling;
		if (val == "grapple") return kGrapple;
		if (val == "impact") return kImpact;
		if (val == "krush") return kKrush;
		if (val == "martialartsstrikes") return kMartialArtsStrikes;
		if (val == "martialartssweeps") return kMartialArtsSweeps;
		if (val == "puncture") return kPuncture;
		if (val == "slash") return kSlash;
		if (val == "subdual") return kSubdual;
		if (val == "tiny") return kTiny;
		if (val == "unbalance") return kUnbalance;
		if (val == "aether") return kAether;
		if (val == "cold") return kCold;
		if (val == "electrical") return kElectrical;
		if (val == "heat") return kHeat;
		if (val == "nether") return kNether;

		return {};
	}


	void CriticalType::fromString(std::string_view sv, CriticalType::Type& type) {
		std::optional<Type> opt_type = fromString(sv);
		if (opt_type) type = opt_type.value();
		else throw std::invalid_argument("Invalid string value for CriticalType::Type: " + std::string(sv));
	}

} // namespace rm::rule::enums