#include <CharacterLevellerSerializer.h>
#include <JsonConverter.h>

namespace rm::serial {

json::value CharacterLevellerSerializer::serializeObject(const CharacterLeveller& ref) const {
	json::object obj;

	// Identity + basic state
	JsonConverter::setString(obj, "id", ref.id());
	JsonConverter::setString(obj, "character", ref.character_->id());
	JsonConverter::setDataPrimitiveMap<TrainingPackageData, int>(obj, "trainingPackageCosts", ref.training_package_costs_);
	JsonConverter::setDataSet<TrainingPackageData>(obj, "trainingPackages", ref.training_packages_);
	JsonConverter::setEnumSet(obj, "statGains", ref.stat_gains_);
	JsonConverter::setSkillPrimitiveMap<int>(obj, "skillRanks", ref.skill_ranks_);
	JsonConverter::setDataPrimitiveMap<SkillCategoryData, int>(obj, "categoryRanks", ref.category_ranks_);
	JsonConverter::setDataPrimitiveMap<SpellListData, int>(obj, "spellListRanks", ref.spell_list_ranks_);
	{
		std::map<std::string, const LanguageAbility> language_abilities;
		for (const auto& ability : ref.language_abilities_) {
			language_abilities.emplace(ability.languageId(), ability);
		}
		JsonConverter::setLanguageAbilities(obj, "languageAbilities", language_abilities);
	}

	return obj;
}

const CharacterLeveller& CharacterLevellerSerializer::deserializeObject(json::object& jsonObj) const {
	std::string id = JsonConverter::getString(jsonObj, "id");

	// If this object does not have an ID, create a new CharacterLeveller to get a cached object with a generated ID.
	if (id.empty()) {
		CharacterLeveller& tmp = manager_.get<CharacterLeveller>();
		id = tmp.id(); // Generate a new ID for this character builder
	}

	// Now we know we have an id, we can get the reference to the CharacterLeveller that we will populate with data. This will be either the newly created one (if there was no ID in the JSON) or an existing one (if there was an ID in the
	// JSON).
	CharacterLeveller& ref = manager_.get<CharacterLeveller>(id);

	// Deserialize the character reference
	std::string characterId = JsonConverter::getString(jsonObj, "character");
	if (!characterId.empty()) {
		ref.character_ = &manager_.get<Character>(characterId);
	}

	ref.training_package_costs_ = JsonConverter::getDataPrimitiveMap<TrainingPackageData, int>(jsonObj, "trainingPackageCosts", manager_);
	ref.training_packages_ = JsonConverter::getDataSet<TrainingPackageData>(jsonObj, "trainingPackages", manager_);
	ref.stat_gains_ = JsonConverter::getEnumSet<StatType::Type>(jsonObj, "statGains");
	ref.skill_ranks_ = JsonConverter::getSkillPrimitiveMap<int>(jsonObj, "skillRanks", manager_);
	ref.category_ranks_ = JsonConverter::getDataPrimitiveMap<SkillCategoryData, int>(jsonObj, "categoryRanks", manager_);
	ref.spell_list_ranks_ = JsonConverter::getDataPrimitiveMap<SpellListData, int>(jsonObj, "spellListRanks", manager_);
	{
		const std::map<std::string, const LanguageAbility> language_abilities = JsonConverter::getLanguageAbilityMap(jsonObj, "languageAbilities", manager_);
		for (const auto& [language_name, ability] : language_abilities) {
			ref.language_abilities_.emplace(ability);
		}
	}

	return ref;
}

} // namespace rm::serial