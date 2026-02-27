#include <AnimalOutlookType.h>

namespace rm {

	const std::string AnimalOutlookType::toString(Type size) {
		using enum Type;

		switch (size) {
		case kAggressive: return "Aggressive";
		case kAloof: return "Aloof";
		case kAltruistic: return "Altruistic";
		case kBelligerent: return "Belligerent";
		case kBerserk: return "Berserk";
		case kCarefree: return "Carefree";
		case kCruel: return "Cruel";
		case kDomineering: return "Domineering";
		case kGood: return "Good";
		case kGreedy: return "Greedy";
		case kHostile: return "Hostile";
		case kHungry: return "Hungry";
		case kInquisitive: return "Inquisitive";
		case kJumpy: return "Jumpy";
		case kNormal: return "Normal";
		case kPassive: return "Passive";
		case kPlayful: return "Playful";
		case kProtective: return "Protective";
		case kTimid: return "Timid";
		default: return "";
		}
	}

	const std::optional<AnimalOutlookType::Type> AnimalOutlookType::fromString(std::string_view sv) {
		using enum Type;

		const std::string val = archid::lcaseconcat(sv);
		if (val == "aggressive") return kAggressive;
		if (val == "aloof") return kAloof;
		if (val == "altruistic") return kAltruistic;
		if (val == "belligerent") return kBelligerent;
		if (val == "berserk") return kBerserk;
		if (val == "carefree") return kCarefree;
		if (val == "cruel") return kCruel;
		if (val == "domineering") return kDomineering;
		if (val == "good") return kGood;
		if (val == "greedy") return kGreedy;
		if (val == "hostile") return kHostile;
		if (val == "hungry") return kHungry;
		if (val == "inquisitive") return kInquisitive;
		if (val == "jumpy") return kJumpy;
		if (val == "normal") return kNormal;
		if (val == "passive") return kPassive;
		if (val == "playful") return kPlayful;
		if (val == "protective") return kProtective;
		if (val == "timid") return kTimid;

		return {};
	}

	const std::string AnimalOutlookType::description(Type size) {
		using enum Type;

		switch (size) {
		case kAggressive: return "Aggressive and will attack if provoked or hungry";
		case kAloof: return "Ignores other creatures unless interfered with, or attacked";
		case kAltruistic: return "Has an unselfish regard for the interests of others, often to the extent of risking his own safety";
		case kBelligerent: return "Often attacks without provocation";
		case kBerserk: return "Attacks closest living creature until it is destroyed";
		case kCarefree: return "Does not believe that danger or misfortune exists for it";
		case kCruel: return "Not only hostile, but delights in death, pain, and suffering";
		case kDomineering: return "Desires power, attempts to control or dominate other creatures";
		case kGood: return "Opposed to “evil” (e.g., those who are cruel, hostile, belligerent, etc.); supportive of those who are also good";
		case kGreedy: return "Will attack or attempt to steal from other creatures if the risk does not seem too high";
		case kHostile: return "Normally attacks other creatures on sight";
		case kHungry: return "If hungry, will attack anything edible; otherwise Normal";
		case kInquisitive: return "Inquisitive and curious; will approach and examine unusual situations";
		case kJumpy: return "Normally bolts at any sign of other creatures";
		case kNormal: return "Watches and is wary of other creatures; will sometimes attack if hungry";
		case kPassive: return "Ignores the presence of other creatures unless threatened";
		case kPlayful: return "Mischievous, but playful; will attempt to play with or play pranks on other creatures";
		case kProtective: return "Protective of a thing, place, other creature, etc.";
		case kTimid: return "Skittish around other creatures, runs at the slightest hint of danger";
		default: return "";
		}
	}

	void AnimalOutlookType::fromString(std::string_view sv, AnimalOutlookType::Type& type) {
		std::optional<Type> opt_type = fromString(sv);
		if (opt_type) type = opt_type.value();
		else throw std::invalid_argument("Invalid string value for AnimalOutlookType::Type: " + std::string(sv));
	}

} // namespace rm