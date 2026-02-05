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

const std::string EnvironmentType::toString(EnvironmentType::Terrain terrain) {
	using enum EnvironmentType::Terrain;

	switch (terrain) {
	case kAlpine: return "Alpine";
	case kRough: return "Rough";
	case kUnderground: return "Underground";
	case kWaste: return "Waste";
	default: return "";
	}
}

const std::string EnvironmentType::description(EnvironmentType::Terrain terrain) {
	using enum EnvironmentType::Terrain;

	switch (terrain) {
	case kAlpine: return "Alpine, high altitude or mountainous";
	case kRough: return "Rough, rugged or rocky hills";
	case kUnderground: return "Underground";
	case kWaste: return "Waste, barren";
	default: return "";
	}
}

const std::optional<EnvironmentType::Terrain> EnvironmentType::terrain(std::string_view sv) {
	using enum EnvironmentType::Terrain;

	const std::string val = lcaseconcat(sv);
	if (val == "alpine") return kAlpine;
	if (val == "rough") return kRough;
	if (val == "underground") return kUnderground;
	if (val == "waste") return kWaste;
	
	return {};
}

const std::string EnvironmentType::toString(EnvironmentType::Vegetation vegetation) {
	using enum EnvironmentType::Vegetation;

	switch (vegetation) {
	case kBarren: return "Barren";
	case kConiferous: return "Coniferous";
	case kDeciduous: return "Deciduous";
	case kGrasslands: return "Grasslands";
	case kHeath: return "Heath";
	case kJungle: return "Jungle";
	case kPlains: return "Plains";
	case kTundra: return "Tundra";
	default: return "";
	}
}

const std::string EnvironmentType::description(EnvironmentType::Vegetation vegetation) {
	using enum EnvironmentType::Vegetation;

	switch (vegetation) {
	case kBarren: return "Barren, without vegetation";
	case kConiferous: return "Coniferous forest and/or taiga";
	case kDeciduous: return "Deciduous and mixed forest";
	case kGrasslands: return "Grasslands with long grass";
	case kHeath: return "Heath, scrub or moor";
	case kJungle: return "Jungle or rainforest";
	case kPlains: return "Grasslands with short grass";
	case kTundra: return "Tundra";
	default: return "";
	}
}

const std::optional<EnvironmentType::Vegetation> EnvironmentType::vegetation(std::string_view sv) {
	using enum EnvironmentType::Vegetation;

	const std::string val = lcaseconcat(sv);
	if (val == "barren") return kBarren;
	if (val == "coniferous") return kConiferous;
	if (val == "deciduous") return kDeciduous;
	if (val == "grasslands") return kGrasslands;
	if (val == "heath") return kHeath;
	if (val == "jungle") return kJungle;
	if (val == "plains") return kPlains;
	if (val == "tundra") return kTundra;

	return {};
}

