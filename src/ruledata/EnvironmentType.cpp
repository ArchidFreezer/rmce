#include <EnvironmentType.h>

const std::string EnvironmentType::toString(EnvironmentType::Feature feature) {
	using enum EnvironmentType::Feature;

	switch (feature) {
	case kBattlefield: return "Battlefield";
	case kBurial: return "Burial";
	case kCave: return "Cave";
	case kCavern: return "Cavern";
	case kDimention: return "Dimention";
	case kEnchanted: return "Enchanted";
	case kHabitation: return "Habitation";
	case kRuins: return "Ruins";
	case kRural: return "Rural";
	case kVolcanic: return "Volcanic";
	default: return "";
	}
}

const std::string EnvironmentType::description(EnvironmentType::Feature feature) {
	using enum EnvironmentType::Feature;

	switch (feature) {
	case kBattlefield: return "Battlefields and/or shipwrecks";
	case kBurial: return "Burial sites";
	case kCave: return "Cave entrances, overhangs and/or lairs";
	case kCavern: return "Cavern complexes";
	case kDimention: return "Cross-over points for dimentions";
	case kEnchanted: return "Enchanted or magical places";
	case kHabitation: return "Villages, towns, cities and/or castles";
	case kRuins: return "Ruins of any habitation or building";
	case kRural: return "Rural inhabited and/or cultivated areas";
	case kVolcanic: return "Volcanic areas";
	default: return "";
	}
}

const std::optional<EnvironmentType::Feature> EnvironmentType::feature(std::string_view sv) {
	using enum EnvironmentType::Feature;

	const std::string val = lcaseconcat(sv);
	if (val == "battlefield") return kBattlefield;
	if (val == "burial") return kBurial;
	if (val == "cave") return kCave;
	if (val == "cavern") return kCavern;
	if (val == "dimention") return kDimention;
	if (val == "enchanted") return kEnchanted;
	if (val == "habitation") return kHabitation;
	if (val == "ruins") return kRuins;
	if (val == "rural") return kRural;
	if (val == "volcanic") return kVolcanic;

	return {};
}

