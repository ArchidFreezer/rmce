#include <CharacterBuilderSerializer.h>
#include <JsonConverter.h>

namespace rm::serial {

json::value CharacterBuilderSerializer::serializeObject(const CharacterBuilder& ref) const {
	json::object obj;

	// Identity + basic state
	JsonConverter::setString(obj, "id", ref.id());
	JsonConverter::setString(obj, "name", ref.name_);
	JsonConverter::setBool(obj, "built", ref.built_);
	JsonConverter::setInt(obj, "num_adolescent_language_ranks", ref.num_adolescent_language_ranks_);
	JsonConverter::setInt(obj, "num_hobby_skill_ranks", ref.num_hobby_skill_ranks_);
	JsonConverter::setInt(obj, "num_spell_list_ranks", ref.num_adolescent_spell_list_ranks_);

	// Core rule data references
	if (ref.race_)
		JsonConverter::setString(obj, "race", ref.race_->id());
	if (ref.culture_)
		JsonConverter::setString(obj, "culture", ref.culture_->id());
	if (ref.culture_type_)
		JsonConverter::setString(obj, "culture_type", ref.culture_type_->id());
	if (ref.profession_)
		JsonConverter::setString(obj, "profession", ref.profession_->id());

	// Initial choices
	JsonConverter::setEnumSet(obj, "magical_realms", ref.magical_realms_);
	JsonConverter::setDataSet<SkillCategoryData>(obj, "race_category_everyman_choices", ref.race_category_everyman_choices_);
	JsonConverter::setLanguageAbilities(obj, "race_adolescent_language_choices", ref.race_adolescent_languages_);
	JsonConverter::setSkillPrimitiveMap<int>(obj, "culture_type_category_skill_ranks", ref.culture_type_category_skill_ranks_);
	if (ref.adolescent_spell_list_choice_)
		JsonConverter::setString(obj, "adolescent_spell_list_choice", ref.adolescent_spell_list_choice_->id());

	{
		std::set<const SpellListData*> base_spell_lists;
		for (const SpellListData* spell_list : ref.base_spell_list_choices_) {
			if (spell_list)
				base_spell_lists.emplace(spell_list);
		}
		JsonConverter::setDataSet<SpellListData>(obj, "base_spell_list_choices", base_spell_lists);
	}

	JsonConverter::setSkillEnumMap<SkillDevelopmentType::Type>(obj, "prof_skill_subcategory_development_type_choices", ref.prof_skill_subcategory_development_type_choices_);
	JsonConverter::setSkillEnumMap<SkillDevelopmentType::Type>(obj, "prof_skill_development_type_choices", ref.prof_skill_development_type_choices_);
	JsonConverter::setDataEnumMap<SkillData, SkillDevelopmentType::Type>(obj, "prof_category_development_type_choices", ref.prof_category_development_type_choices_);
	JsonConverter::setDataEnumMap<SkillData, SkillDevelopmentType::Type>(obj, "prof_group_development_type_choices", ref.prof_group_development_type_choices_);
	JsonConverter::setSkillPrimitiveMap<int>(obj, "hobby_skill_ranks", ref.hobby_skill_ranks_);
	JsonConverter::setDataPrimitiveMap<SkillCategoryData, int>(obj, "hobby_category_ranks", ref.hobby_category_ranks_);
	JsonConverter::setLanguageAbilities(obj, "background_language_choices", ref.background_language_choices_);

	// Aggregated state
	{
		std::map<std::string, const LanguageAbility> language_abilities;
		for (const auto& [language_name, ability] : ref.language_abilities_) {
			language_abilities.emplace(ability.languageId(), ability);
		}
		JsonConverter::setLanguageAbilities(obj, "language_abilities", language_abilities);
	}

	{
		std::map<RealmType::Type, std::string> realm_progressions;
		for (const auto& [realm, progression] : ref.realm_progressions_) {
			if (progression)
				realm_progressions.emplace(realm, progression->id());
		}
		JsonConverter::setEnumPrimitiveMap<RealmType::Type, std::string>(obj, "realm_progressions", realm_progressions);
	}

	{
		json::array stats_array;
		for (const auto& [stat_type, stat] : ref.stats_) {
			json::object stat_obj;
			JsonConverter::setString(stat_obj, "stat", toString(stat_type));
			JsonConverter::setInt(stat_obj, "temporary", stat.temporary());
			JsonConverter::setInt(stat_obj, "potential", stat.potential());
			JsonConverter::setInt(stat_obj, "bonus", stat.bonus());
			stats_array.emplace_back(std::move(stat_obj));
		}
		if (stats_array.size())
			obj["stats"] = std::move(stats_array);
	}

	JsonConverter::setSkillSet(obj, "everyman_skills", ref.everyman_skills_);
	JsonConverter::setSkillSet(obj, "restricted_skills", ref.restricted_skills_);
	JsonConverter::setDataSet<SkillCategoryData>(obj, "everyman_skill_categories", ref.everyman_skill_categories_);
	JsonConverter::setDataSet<SkillCategoryData>(obj, "restricted_skill_categories", ref.restricted_skill_categories_);
	JsonConverter::setSkillPrimitiveMap<int>(obj, "skill_ranks", ref.skill_ranks_);
	JsonConverter::setSkillPrimitiveMap<int>(obj, "skill_professional_bonuses", ref.skill_professional_bonuses_);
	JsonConverter::setSkillEnumMap<SkillDevelopmentType::Type>(obj, "skillsub_development_types", ref.skillsub_development_types_);
	JsonConverter::setDataEnumMap<SkillData, SkillDevelopmentType::Type>(obj, "skill_development_types", ref.skill_development_types_);
	JsonConverter::setDataPrimitiveMap<SkillCategoryData, int>(obj, "category_ranks", ref.category_ranks_);
	JsonConverter::setDataPrimitiveMap<SkillCategoryData, int>(obj, "category_professional_bonuses", ref.category_professional_bonuses_);
	JsonConverter::setDataPrimitiveMap<SkillCategoryData, int>(obj, "category_special_bonuses", ref.category_special_bonuses_);
	JsonConverter::setDataEnumMap<SkillCategoryData, SkillDevelopmentType::Type>(obj, "category_development_types", ref.category_development_types_);
	JsonConverter::setDataPrimitiveMap<SkillGroupData, int>(obj, "group_professional_bonuses", ref.group_professional_bonuses_);
	JsonConverter::setDataPrimitiveMap<SkillGroupData, int>(obj, "group_special_bonuses", ref.group_special_bonuses_);
	JsonConverter::setDataEnumMap<SkillGroupData, SkillDevelopmentType::Type>(obj, "group_development_types", ref.group_development_types_);
	JsonConverter::setDataPrimitiveMap<SpellListData, int>(obj, "spell_list_ranks", ref.spell_list_ranks_);

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

	// Reset existing state and bind object manager
	ref.reset(false);
	ref.object_factory_ = &manager_;

	// Identity + basic state
	ref.name_ = JsonConverter::getString(jsonObj, "name");
	ref.built_ = JsonConverter::getBool(jsonObj, "built", false);
	ref.num_adolescent_language_ranks_ = JsonConverter::getInt(jsonObj, "num_adolescent_language_ranks", 0);
	ref.num_hobby_skill_ranks_ = JsonConverter::getInt(jsonObj, "num_hobby_skill_ranks", 0);
	ref.num_adolescent_spell_list_ranks_ = JsonConverter::getInt(jsonObj, "num_spell_list_ranks", 0);

	// Core rule data references
	{
		const std::string race_id = JsonConverter::getString(jsonObj, "race");
		if (!race_id.empty())
			ref.race_ = &manager_.get<RaceData>(race_id);
	}

	{
		const std::string culture_id = JsonConverter::getString(jsonObj, "culture");
		if (!culture_id.empty())
			ref.culture_ = &manager_.get<CultureData>(culture_id);
	}

	{
		const std::string culture_type_id = JsonConverter::getString(jsonObj, "culture_type");
		if (!culture_type_id.empty())
			ref.culture_type_ = &manager_.get<CultureTypeData>(culture_type_id);
	}

	{
		const std::string profession_id = JsonConverter::getString(jsonObj, "profession");
		if (!profession_id.empty())
			ref.profession_ = &manager_.get<ProfessionData>(profession_id);
	}

	// Initial choices
	ref.magical_realms_ = JsonConverter::getEnumSet<RealmType::Type>(jsonObj, "magical_realms");
	ref.race_category_everyman_choices_ = JsonConverter::getDataSet<SkillCategoryData>(jsonObj, "race_category_everyman_choices", manager_);
	ref.race_adolescent_languages_ = JsonConverter::getLanguageAbilityMap(jsonObj, "race_adolescent_language_choices", manager_);
	ref.culture_type_category_skill_ranks_ = JsonConverter::getSkillPrimitiveMap<int>(jsonObj, "culture_type_category_skill_ranks", manager_);
	{
		const std::string adolescent_spell_list_choice_id = JsonConverter::getString(jsonObj, "adolescent_spell_list_choice");
		if (!adolescent_spell_list_choice_id.empty())
			ref.adolescent_spell_list_choice_ = &manager_.get<SpellListData>(adolescent_spell_list_choice_id);
	}

	{
		const std::set<const SpellListData*> base_spell_lists = JsonConverter::getDataSet<SpellListData>(jsonObj, "base_spell_list_choices", manager_);
		for (const SpellListData* spell_list : base_spell_lists) {
			if (spell_list)
				ref.base_spell_list_choices_.insert(const_cast<SpellListData*>(spell_list));
		}
	}

	ref.prof_skill_subcategory_development_type_choices_ = JsonConverter::getSkillEnumMap<SkillDevelopmentType::Type>(jsonObj, "prof_skill_subcategory_development_type_choices", manager_);
	ref.prof_skill_development_type_choices_ = JsonConverter::getSkillEnumMap<SkillDevelopmentType::Type>(jsonObj, "prof_skill_development_type_choices", manager_);
	ref.prof_category_development_type_choices_ = JsonConverter::getDataEnumMap<SkillData, SkillDevelopmentType::Type>(jsonObj, "prof_category_development_type_choices", manager_);
	ref.prof_group_development_type_choices_ = JsonConverter::getDataEnumMap<SkillData, SkillDevelopmentType::Type>(jsonObj, "prof_group_development_type_choices", manager_);
	ref.hobby_skill_ranks_ = JsonConverter::getSkillPrimitiveMap<int>(jsonObj, "hobby_skill_ranks", manager_);
	ref.hobby_category_ranks_ = JsonConverter::getDataPrimitiveMap<SkillCategoryData, int>(jsonObj, "hobby_category_ranks", manager_);
	ref.background_language_choices_ = JsonConverter::getLanguageAbilityMap(jsonObj, "background_language_choices", manager_);

	// Aggregated state
	{
		const std::map<std::string, const LanguageAbility> language_abilities = JsonConverter::getLanguageAbilityMap(jsonObj, "language_abilities", manager_);
		for (const auto& [language_name, ability] : language_abilities) {
			ref.language_abilities_.insert_or_assign(ability.language(), ability);
		}
	}

	{
		const auto realm_progressions = JsonConverter::getEnumPrimitiveMap<RealmType::Type, std::string>(jsonObj, "realm_progressions");
		for (const auto& [realm, progression_id] : realm_progressions) {
			if (!progression_id.empty())
				ref.realm_progressions_.insert_or_assign(realm, &manager_.get<SkillProgressionTypeData>(progression_id));
		}
	}

	{
		const json::array stats_array = JsonConverter::getJsonArray(jsonObj, "stats");
		for (const json::value& stat_value : stats_array) {
			if (!stat_value.is_object())
				continue;

			const json::object stat_obj = stat_value.as_object();

			StatType::Type stat_type{};
			fromString(JsonConverter::getString(stat_obj, "stat"), stat_type);

			const int temporary = JsonConverter::getInt(stat_obj, "temporary", 0);
			const int potential = JsonConverter::getInt(stat_obj, "potential", 0);
			const int bonus = JsonConverter::getInt(stat_obj, "bonus", 0);

			Stat stat{};
			stat.setTemporary(temporary);
			stat.setPotential(potential);
			stat.setRacialBonus(bonus - rm::game::character::stat::getBasicBonus(temporary));

			ref.stats_.insert_or_assign(stat_type, stat);
		}
	}

	ref.everyman_skills_ = JsonConverter::getSkillSet(jsonObj, "everyman_skills", manager_);
	ref.restricted_skills_ = JsonConverter::getSkillSet(jsonObj, "restricted_skills", manager_);
	ref.everyman_skill_categories_ = JsonConverter::getDataSet<SkillCategoryData>(jsonObj, "everyman_skill_categories", manager_);
	ref.restricted_skill_categories_ = JsonConverter::getDataSet<SkillCategoryData>(jsonObj, "restricted_skill_categories", manager_);
	ref.skill_ranks_ = JsonConverter::getSkillPrimitiveMap<int>(jsonObj, "skill_ranks", manager_);
	ref.skill_professional_bonuses_ = JsonConverter::getSkillPrimitiveMap<int>(jsonObj, "skill_professional_bonuses", manager_);
	ref.skillsub_development_types_ = JsonConverter::getSkillEnumMap<SkillDevelopmentType::Type>(jsonObj, "skillsub_development_types", manager_);
	ref.skill_development_types_ = JsonConverter::getDataEnumMap<SkillData, SkillDevelopmentType::Type>(jsonObj, "skill_development_types", manager_);
	ref.category_ranks_ = JsonConverter::getDataPrimitiveMap<SkillCategoryData, int>(jsonObj, "category_ranks", manager_);
	ref.category_professional_bonuses_ = JsonConverter::getDataPrimitiveMap<SkillCategoryData, int>(jsonObj, "category_professional_bonuses", manager_);
	ref.category_special_bonuses_ = JsonConverter::getDataPrimitiveMap<SkillCategoryData, int>(jsonObj, "category_special_bonuses", manager_);
	ref.category_development_types_ = JsonConverter::getDataEnumMap<SkillCategoryData, SkillDevelopmentType::Type>(jsonObj, "category_development_types", manager_);
	ref.group_professional_bonuses_ = JsonConverter::getDataPrimitiveMap<SkillGroupData, int>(jsonObj, "group_professional_bonuses", manager_);
	ref.group_special_bonuses_ = JsonConverter::getDataPrimitiveMap<SkillGroupData, int>(jsonObj, "group_special_bonuses", manager_);
	ref.group_development_types_ = JsonConverter::getDataEnumMap<SkillGroupData, SkillDevelopmentType::Type>(jsonObj, "group_development_types", manager_);
	ref.spell_list_ranks_ = JsonConverter::getDataPrimitiveMap<SpellListData, int>(jsonObj, "spell_list_ranks", manager_);

	return ref;
}

json::value CharacterBuilderSerializer::serializeHobbyChoices(const CharacterBuilder& ref) const {
	json::object obj;
	// Hobby skill choices
	JsonConverter::setInt(obj, "numHobbyRanks", ref.num_hobby_skill_ranks_);
	const CultureData* culture = ref.culture_;
	if (culture) {
		json::array hobby_skills_array;
		for (const auto& skill : culture->hobbySkills()) {
			json::object skill_obj;
			JsonConverter::setString(skill_obj, "id", skill->skillData().id());
			if (skill->subcategory()) {
				JsonConverter::setString(skill_obj, "subcategory", skill->subcategory().value());
			}
			JsonConverter::setInt(skill_obj, "value", ref.getMaxHobbyRanksForSkill(skill));
			hobby_skills_array.push_back(skill_obj);
		}
		if (hobby_skills_array.size())
			obj["hobbySkills"] = hobby_skills_array;

		json::array hobby_categories_array;
		for (const auto& category : culture->hobbySkillCategories()) {
			json::object category_obj;
			JsonConverter::setString(category_obj, "id", category->id());
			JsonConverter::setInt(category_obj, "value", ref.getMaxHobbyRanksForCategory(category));
			hobby_categories_array.push_back(category_obj);
		}
		if (hobby_categories_array.size())
			obj["hobbyCategories"] = hobby_categories_array;
	}

	// Background language choices
	const RaceData* race = ref.race_;
	if (race) {
		JsonConverter::setInt(obj, "numLanguageRanks", ref.num_adolescent_language_ranks_);
		json::array adolescent_languages_array	;
		for (const auto& [language_name, ability] : race->adolescentLanguageAbilities()) {
			json::object language_obj;
			JsonConverter::setString(language_obj, "language", ability.languageId());
			if (ability.isSomatic()) JsonConverter::setInt(language_obj, "somantic", ability.somatic());
			if (ability.isSpoken()) JsonConverter::setInt(language_obj, "spoken", ability.spoken());
			if (ability.isWritten()) JsonConverter::setInt(language_obj, "written", ability.written());
			adolescent_languages_array.push_back(language_obj);
		}
		if (adolescent_languages_array.size()) {
			obj["adolescentLanguages"] = adolescent_languages_array;
		}
	}

	// Adolescent spell list ranks
	{
		JsonConverter::setInt(obj, "numSpellListRanks", ref.num_adolescent_spell_list_ranks_);
		JsonConverter::setDataSet(obj, "adolescentSpellLists", ref.getAdolescentSpellListChoices());
	}

	return obj;
}

} // namespace rm::serial