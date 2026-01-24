#include <BookData.h>
#include <SkillActionType.h>
#include <SkillCategoryData.h>
#include <SkillData.h>
#include <SkillDatafileParserXml.h>

void SkillDatafileParserXml::parse(bool id_only) {
	std::cout << "Loading Skill data ... ";
	std::cout << (id_only ? "[Pass 1]" : "[Pass 2]") << std::endl;

	// Get the lists to parse and loop through them
	const pt::ptree& tree = ptree().get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", GameRuleData::generateId(ruleDatatype(), name));

		if (id_only) {
			// We create a Data object and reference it with as a unique_ptr to allow us to use move semantics to transfer ownership
			// to the cache when we add it
			std::unique_ptr<SkillData> datum = std::make_unique<SkillData>(id);
			cache().add<SkillData>(std::move(datum), id);
		} else {
			SkillData& ref = cache().get<SkillData>(id);
			ref.setName(name);
			ref.setDescription(v.second.get<std::string>("description"));
			ref.setDifficultySummary(v.second.get<std::string>("difficulties-summary"));
			ref.setNotes(v.second.get<std::string>("notes"));
			ref.setIsRestricted(v.second.get<bool>("is-restricted"));
			ref.setCanSpecialise(v.second.get<bool>("can-specialise"));
			ref.setMandatorySubcategory(v.second.get<bool>("mandatory_subcategory"));
			ref.setExhaustionCost(v.second.get<float>("exhaustion"));
			ref.setDistanceMultiplier(v.second.get<float>("distance-multiplier"));

			// Get the book from the cache
			std::string book_id = GameRuleData::generateId("Book", v.second.get<std::string>("book"));
			ref.setBook(cache().get<BookData>(book_id));

			// Get the skill action type
			std::string action_type = v.second.get<std::string>("action");
			if (SkillActionType::fromString(action_type)) {
				ref.setActionType(SkillActionType::fromString(action_type).value());
			} else {
				throw SkillActionType::SkillActionNotFoundException("Could not find SkillActionType for " + action_type);
			}

			// Get the skill category from the cache
			std::string category = v.second.get<std::string>("category");
			std::string category_id = GameRuleData::generateId("SkillCategory", category);
			ref.setCategory(cache().get<SkillCategoryData>(category_id));

			// Get the stats if any are defined
			if (boost::optional<const pt::ptree&> stat_tree = v.second.get_child_optional("stats")) {
				for (const auto& stat : stat_tree.get()) {
					if (StatType::fromString(stat.second.data())) {
						ref.addStat(StatType::fromString(stat.second.data()).value());
					}
				}
			}

			// Get the Subcategories if any are defined
			if (boost::optional<const pt::ptree&> subcategory_tree = v.second.get_child_optional("subcategories")) {
				for (const auto& subcategory : subcategory_tree.get()) {
					ref.addSubcategory(subcategory.second.data());
				}
			}

			std::cout << "\tSkill name: " << ref.name() << std::endl;
		}
	}
	std::cout << " done" << std::endl;
}

std::string SkillDatafileParserXml::parseCategory(const std::string& category) {
	auto pos = category.find(" | ");
	if (pos != std::string::npos)
		return category.substr(pos + 3, category.length());
	else
		return category;
}