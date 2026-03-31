#include <CharacterBuilderSerializer.h>
#include <JsonConverter.h>

namespace rm::serial {

json::value CharacterBuilderSerializer::serializeObject(const CharacterBuilder& ref) const {
	json::object obj;

	// Identity
	JsonConverter::setString(obj, "id", ref.id());
	JsonConverter::setString(obj, "name", ref.name_);

	// Core choices — store IDs of the pointed-to rule objects (nullptr-safe)
	if (ref.race_)
		JsonConverter::setString(obj, "race", ref.race_->id());
	if (ref.culture_)
		JsonConverter::setString(obj, "culture", ref.culture_->id());
	if (ref.culture_type_)
		JsonConverter::setString(obj, "culture_type", ref.culture_type_->id());
	if (ref.profession_)
		JsonConverter::setString(obj, "profession", ref.profession_->id());

	// Magical realms — set of enum values
	JsonConverter::setEnumSet(obj, "magical_realms", ref.magical_realms_);

	// Everyman skill category choices — set of rule data pointers
	JsonConverter::setDataSet(obj, "everyman_skill_category_choices", ref.race_category_everyman_choices_);

	// Race adolescent language choices — map of language id -> LanguageAbility
	JsonConverter::setLanguageAbilities(obj, "race_adolescent_language_choices", ref.race_adolescent_language_choices_);

	// Race realm progressions — map of RealmType::Type enum -> SkillProgressionTypeData pointer
	JsonConverter::setEnumPrimitiveMap(obj, "race_realm_progressions", [&]() {
		std::map<RealmType::Type, std::string> realm_progression_ids;
		for (const auto& [realm, progression] : ref.realm_progressions_) {
			if (progression)
				realm_progression_ids.emplace(realm, progression->id());
		}
		return realm_progression_ids;
	}());

	return obj;
}

const CharacterBuilder& CharacterBuilderSerializer::deserializeObject(json::object& jsonObj) const {
	std::string id = JsonConverter::getString(jsonObj, "id");

	// If this object does not have an ID, create a new CharacterBuilder to get a cached object with a generated ID.
	if (id.empty()) {
		CharacterBuilder& tmp = manager_.get<CharacterBuilder>();
		id = tmp.id(); // Generate a new ID for this character builder
	}

	// Now we know we have an id, we can get the reference to the CharacterBuilder that we will populate with data. This will be either the newly created one (if there was no ID in the JSON) or an existing one (if there was an ID in the
	// JSON).
	CharacterBuilder& ref = manager_.get<CharacterBuilder>(id);

	ref.name_ = JsonConverter::getString(jsonObj, "name");

	// Core rule data references
	std::string race_id = JsonConverter::getString(jsonObj, "race");
	if (!race_id.empty())
		ref.race_ = &manager_.get<RaceData>(race_id);

	std::string culture_id = JsonConverter::getString(jsonObj, "culture");
	if (!culture_id.empty())
		ref.culture_ = &manager_.get<CultureData>(culture_id);

	std::string culture_type_id = JsonConverter::getString(jsonObj, "culture_type");
	if (!culture_type_id.empty())
		ref.culture_type_ = &manager_.get<CultureTypeData>(culture_type_id);

	std::string profession_id = JsonConverter::getString(jsonObj, "profession");
	if (!profession_id.empty())
		ref.profession_ = &manager_.get<ProfessionData>(profession_id);

	// Magical realms
	ref.magical_realms_ = JsonConverter::getEnumSet<RealmType::Type>(jsonObj, "magical_realms");

	// Everyman skill category choices
	ref.race_category_everyman_choices_ = JsonConverter::getDataSet<SkillCategoryData>(jsonObj, "everyman_skill_category_choices", manager_);

	// Race adolescent language choices
	ref.race_adolescent_language_choices_ = JsonConverter::getLanguageAbilityMap(jsonObj, "race_adolescent_language_choices", manager_);

	// Race realm progressions: map of enum -> SkillProgressionTypeData*
	auto realm_progression_ids = JsonConverter::getEnumPrimitiveMap<RealmType::Type, std::string>(jsonObj, "race_realm_progressions");
	for (const auto& [realm, progression_id] : realm_progression_ids) {
		if (!progression_id.empty())
			ref.realm_progressions_.emplace(realm, &manager_.get<SkillProgressionTypeData>(progression_id));
	}

	return ref;
}

} // namespace rm::serial