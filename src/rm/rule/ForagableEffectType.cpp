#include <ForagableEffectType.h>

namespace rm::rule::enums {

const std::string ForagableEffectType::toString(Type type) {
	using enum Type;

	switch (type) {
	case kAntidote:
		return "Antidote";
	case kBoneRepair:
		return "Bone Repair";
	case kBurnExposureRelief:
		return "Burn & Exposure Relief";
	case kCirculatoryRepair:
		return "Circulatory Repair";
	case kCirculatoryPoison:
		return "Circulatory Poison";
	case kConcussionRelief:
		return "Concussion Relief";
	case kConversionPoison:
		return "Conversion Poison";
	case kGeneralPurpose:
		return "General Purpose";
	case kIntoxicant:
		return "Intoxicant";
	case kLifePreservation:
		return "Life Preservation";
	case kMuscleCartilageTendonRepair:
		return "Muscle Cartilage & Tendon Repair";
	case kMusclePoison:
		return "Muscle Poison";
	case kNervePoison:
		return "Nerve Poison";
	case kNerveRepair:
		return "Nerve Repair";
	case kOrganRepairPreservation:
		return "Organ Repair & Preservation";
	case kPhysicalAlterationEnhancement:
		return "Physical Alteration & Enhancement";
	case kReductionPoison:
		return "Reduction Poison";
	case kRespitoryPoison:
		return "Respitory Poison";
	case kStatModifier:
		return "Stat Modifier";
	case kStunRelief:
		return "Stun Relief";
	default:
		return "";
	}
}

const std::optional<ForagableEffectType::Type> ForagableEffectType::fromString(std::string_view sv) {
	using enum Type;

	const std::string& val = archid::lcaseconcat(sv);
	if (val == "antidote")
		return kAntidote;
	if (val == "bonerepair")
		return kBoneRepair;
	if (val == "burnexposurerelief")
		return kBurnExposureRelief;
	if (val == "circulatoryrepair")
		return kCirculatoryRepair;
	if (val == "circulatorypoison")
		return kCirculatoryPoison;
	if (val == "concussionrelief")
		return kConcussionRelief;
	if (val == "conversionpoison")
		return kConversionPoison;
	if (val == "generalpurpose")
		return kGeneralPurpose;
	if (val == "intoxicant")
		return kIntoxicant;
	if (val == "lifepreservation")
		return kLifePreservation;
	if (val == "musclecartilagetendonrepair")
		return kMuscleCartilageTendonRepair;
	if (val == "musclepoison")
		return kMusclePoison;
	if (val == "nervepoison")
		return kNervePoison;
	if (val == "nerverepair")
		return kNerveRepair;
	if (val == "organrepairpreservation")
		return kOrganRepairPreservation;
	if (val == "physicalalterationenhancement")
		return kPhysicalAlterationEnhancement;
	if (val == "reductionpoison")
		return kReductionPoison;
	if (val == "respitorypoison")
		return kRespitoryPoison;
	if (val == "statmodifier")
		return kStatModifier;
	if (val == "stunrelief")
		return kStunRelief;

	return {};
}

void ForagableEffectType::fromString(std::string_view sv, ForagableEffectType::Type& type) {
	std::optional<Type> opt_type = fromString(sv);
	if (opt_type)
		type = opt_type.value();
	else
		throw std::invalid_argument("Invalid string value for ForagableEffectType::Type: " + std::string(sv));
}

} // namespace rm::rule::enums