#include <ArmourType.h>
#include <ArmourTypeData.h>
#include <ArmourTypeDatafileParserXml.h>

void ArmourTypeDatafileParserXml::parse(bool id_only) {
	// We know there are no references in armour types so we create the complete object in the cache on the first pass
	if (!id_only) return;

	std::cout << "Loading ArmourType data ..." << std::endl;

	// Get the armour types to parse and loop through them
	const pt::ptree& tree = ptree().get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", GameRuleData::generateId(ruleDatatype(), name));

		// We create a ArmourTypeData object and reference it with as a unique_ptr to allow us to use move semantics to transfer ownership
		// to the cache when we add it
		std::unique_ptr<ArmourTypeData> datum = std::make_unique<ArmourTypeData>(id);
		datum->setName(name);
		datum->setDescription(v.second.get<std::string>("description"));
		datum->setMinManoeuvreMod(v.second.get<int>("min-manoeuvre-mod"));
		datum->setMaxManoeuvreMod(v.second.get<int>("max-manoeuvre-mod"));
		datum->setMissileAttackPenalty(v.second.get<int>("missile-attack-penalty"));
		datum->setQuicknessPenalty(v.second.get<int>("quickness-penalty"));
		datum->setAnimalOnly(v.second.get<bool>("animal-only"));

		// Attempt to find the ArmourType
		std::string type{ v.second.get<std::string>("type") };
		if (ArmourType::fromString(type)) {
			datum->setType(ArmourType::fromString(type).value());
		} else {
			throw ArmourType::ArmourTypeNotFoundException("Could not find a armour type for: " + type);
		}

		std::cout << "\tArmourType name: " << datum->name() << std::endl;
		cache().add<ArmourTypeData>(std::move(datum), id);

	}
	std::cout << " done" << std::endl;
}

