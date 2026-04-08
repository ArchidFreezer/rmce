#include <CharacterBuilderSerializer.h>
#include <JsonConverter.h>

namespace rm::serial {

json::value CharacterBuilderSerializer::serializeObject(const CharacterBuilder& ref) const {
	json::object obj;

	// Identity + basic state
	JsonConverter::setString(obj, "id", ref.id());
	JsonConverter::setString(obj, "name", ref.name_);
	JsonConverter::setBool(obj, "built", ref.built_);

	// Core rule data references
	if (ref.race_)
		JsonConverter::setString(obj, "race", ref.race_->id());
	if (ref.culture_)
		JsonConverter::setString(obj, "culture", ref.culture_->id());
	if (ref.culture_type_)
		JsonConverter::setString(obj, "cultureType", ref.culture_type_->id());
	if (ref.profession_)
		JsonConverter::setString(obj, "profession", ref.profession_->id());

	JsonConverter::setEnumSet(obj, "magicalRealms", ref.magical_realms_);
	JsonConverter::setInt(obj, "numHobbySkillRanks", ref.num_hobby_skill_ranks_);
	JsonConverter::setInt(obj, "numAdolescentLanguageRanks", ref.num_adolescent_language_ranks_);
	JsonConverter::setInt(obj, "numAdolescentSpellListRanks", ref.num_adolescent_spell_list_ranks_);
	JsonConverter::setInt(obj, "developmentPoints", ref.development_points_);
	/* Initial choices */
	// Race
	JsonConverter::setSkillSet(obj, "raceCategoryEverymanChoices", ref.race_category_everyman_choices_);
	// Culture type
	JsonConverter::setSkillPrimitiveMap<int>(obj, "cultureTypeCategorySkillRanks", ref.culture_type_category_skill_ranks_);
	// Profession
	JsonConverter::setSkillEnumMap<SkillDevelopmentType::Type>(obj, "profSkillDevelopmentTypeChoices", ref.prof_skill_development_type_choices_);
	JsonConverter::setSkillEnumMap<SkillDevelopmentType::Type>(obj, "profCategoryDevelopmentTypeChoices", ref.prof_category_development_type_choices_);
	JsonConverter::setSkillEnumMap<SkillDevelopmentType::Type>(obj, "profGroupDevelopmentTypeChoices", ref.prof_group_development_type_choices_);
	{
		std::set<const SpellListData*> base_spell_lists;
		for (const SpellListData* spell_list : ref.prof_base_spell_list_choices_) {
			if (spell_list)
				base_spell_lists.emplace(spell_list);
		}
		JsonConverter::setDataSet<SpellListData>(obj, "baseSpellListChoices", base_spell_lists);
	}

	/* Initial Stats */
	{
		json::array stats_array;
		for (const auto& [stat_type, stat] : ref.initial_stats_) {
			json::object stat_obj;
			JsonConverter::setString(stat_obj, "stat", toString(stat_type));
			JsonConverter::setInt(stat_obj, "temporary", stat.temporary());
			JsonConverter::setInt(stat_obj, "potential", stat.potential());
			JsonConverter::setInt(stat_obj, "bonus", stat.bonus());
			stats_array.emplace_back(std::move(stat_obj));
		}
		if (stats_array.size())
			obj["initialStats"] = std::move(stats_array);
	}

	/* Hobby/Adolescent choices */
	JsonConverter::setSkillPrimitiveMap<int>(obj, "hobbySkillRanks", ref.hobby_skill_ranks_);
	JsonConverter::setDataPrimitiveMap<SkillCategoryData, int>(obj, "hobbyCategoryRanks", ref.hobby_category_ranks_);
	if (ref.adolescent_spell_list_choice_) {
		JsonConverter::setString(obj, "adolescentSpellListChoice", ref.adolescent_spell_list_choice_->id());
	}
	{
		std::map<std::string, const LanguageAbility> language_abilities;
		for (const auto& ability : ref.adolescent_language_choices_) {
			language_abilities.emplace(ability.languageId(), ability);
		}
		JsonConverter::setLanguageAbilities(obj, "adolescentLanguageChoices", language_abilities);
	}

	/* Background choices */
	{
		json::array stats_array;
		for (const auto& [stat_type, stat] : ref.background_stats_) {
			json::object stat_obj;
			JsonConverter::setString(stat_obj, "stat", toString(stat_type));
			JsonConverter::setInt(stat_obj, "temporary", stat.temporary());
			JsonConverter::setInt(stat_obj, "potential", stat.potential());
			JsonConverter::setInt(stat_obj, "bonus", stat.bonus());
			stats_array.emplace_back(std::move(stat_obj));
		}
		if (stats_array.size())
			obj["backgroundStats"] = std::move(stats_array);
	}
	JsonConverter::setInt(obj, "backgroundExtraGold", ref.background_extra_gold_);
	{
		std::map<std::string, const LanguageAbility> language_abilities;
		for (const auto& ability : ref.background_language_choices_) {
			language_abilities.emplace(ability.languageId(), ability);
		}
		JsonConverter::setLanguageAbilities(obj, "backgroundLanguageChoices", language_abilities);
	}
	JsonConverter::setSkillPrimitiveMap<int>(obj, "backgroundSkillSpecialBonuses", ref.background_skill_special_bonuses_);
	JsonConverter::setDataPrimitiveMap<SkillCategoryData, int>(obj, "backgroundCategorySpecialBonuses", ref.background_category_special_bonuses_);
	{
		json::array items_array;
		for (const auto& item : ref.background_items_) {
			json::object item_obj;
			JsonConverter::setString(item_obj, "item", item);
			items_array.emplace_back(std::move(item_obj));
		}
		if (items_array.size())
			obj["backgroundItems"] = std::move(items_array);
	}

	/* Aggregated state */
	JsonConverter::setInt(obj, "totalGold", ref.total_gold_);
	{
		std::map<std::string, const LanguageAbility> language_abilities;
		for (const auto& ability : ref.language_abilities_) {
			language_abilities.emplace(ability.languageId(), ability);
		}
		JsonConverter::setLanguageAbilities(obj, "languageAbilities", language_abilities);
	}
	{
		std::map<RealmType::Type, std::string> realm_progressions;
		for (const auto& [realm, progression] : ref.realm_progressions_) {
			if (progression)
				realm_progressions.emplace(realm, progression->id());
		}
		JsonConverter::setEnumPrimitiveMap<RealmType::Type, std::string>(obj, "realmProgressions", realm_progressions);
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
	JsonConverter::setSkillSet(obj, "everymanSkills", ref.everyman_skills_);
	JsonConverter::setSkillSet(obj, "restrictedSkills", ref.restricted_skills_);
	JsonConverter::setDataSet<SkillCategoryData>(obj, "everymanSkillCategories", ref.everyman_skill_categories_);
	JsonConverter::setDataSet<SkillCategoryData>(obj, "restrictedSkillCategories", ref.restricted_skill_categories_);
	JsonConverter::setSkillPrimitiveMap<int>(obj, "skillRanks", ref.skill_ranks_);
	JsonConverter::setSkillPrimitiveMap<int>(obj, "skillProfessionalBonuses", ref.skill_professional_bonuses_);
	JsonConverter::setSkillPrimitiveMap<int>(obj, "skillSpecialBonuses", ref.skill_special_bonuses_);
	JsonConverter::setSkillEnumMap<SkillDevelopmentType::Type>(obj, "skillDevelopmentTypes", ref.skill_development_types_);
	JsonConverter::setDataPrimitiveMap<SkillCategoryData, int>(obj, "categoryRanks", ref.category_ranks_);
	JsonConverter::setDataPrimitiveMap<SkillCategoryData, int>(obj, "categoryProfessionalBonuses", ref.category_professional_bonuses_);
	JsonConverter::setDataPrimitiveMap<SkillCategoryData, int>(obj, "categorySpecialBonuses", ref.category_special_bonuses_);
	JsonConverter::setDataEnumMap<SkillCategoryData, SkillDevelopmentType::Type>(obj, "categoryDevelopmentTypes", ref.category_development_types_);
	{
		json::array arr;
		std::map<std::string, const SkillCategoryData*> sorted_categories{};
		for (const auto& [category, cost] : ref.category_development_costs_) {
			sorted_categories.emplace(category->id(), category);
		}
		for (const auto& pair : sorted_categories) {
			json::object category_cost_obj;
			category_cost_obj["category"] = pair.first;
			category_cost_obj["cost"] = ref.category_development_costs_.at(pair.second).toString();
			arr.emplace_back(std::move(category_cost_obj));
		}
		obj["categoryCosts"] = std::move(arr);
	}
	JsonConverter::setDataPrimitiveMap<SkillGroupData, int>(obj, "groupProfessionalBonuses", ref.group_professional_bonuses_);
	JsonConverter::setDataPrimitiveMap<SkillGroupData, int>(obj, "groupSpecialBonuses", ref.group_special_bonuses_);
	JsonConverter::setDataEnumMap<SkillGroupData, SkillDevelopmentType::Type>(obj, "groupDevelopmentTypes", ref.group_development_types_);
	JsonConverter::setDataPrimitiveMap<SpellListData, int>(obj, "spellListRanks", ref.spell_list_ranks_);
	{
		json::array items_array;
		for (const auto& item : ref.total_items_) {
			json::object item_obj;
			JsonConverter::setString(item_obj, "item", item);
			items_array.emplace_back(std::move(item_obj));
		}
		if (items_array.size())
			obj["totalItems"] = std::move(items_array);
	}

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
		const std::string culture_type_id = JsonConverter::getString(jsonObj, "cultureType");
		if (!culture_type_id.empty())
			ref.culture_type_ = &manager_.get<CultureTypeData>(culture_type_id);
	}

	{
		const std::string profession_id = JsonConverter::getString(jsonObj, "profession");
		if (!profession_id.empty())
			ref.profession_ = &manager_.get<ProfessionData>(profession_id);
	}

	ref.magical_realms_ = JsonConverter::getEnumSet<RealmType::Type>(jsonObj, "magicalRealms");
	ref.num_hobby_skill_ranks_ = JsonConverter::getInt(jsonObj, "numHobbySkillRanks", 0);
	ref.num_adolescent_language_ranks_ = JsonConverter::getInt(jsonObj, "numAdolescentLanguageRanks", 0);
	ref.num_adolescent_spell_list_ranks_ = JsonConverter::getInt(jsonObj, "numAdolescentSpellListRanks", 0);
	ref.development_points_ = JsonConverter::getInt(jsonObj, "developmentPoints", 0);
	// Initial choices
	// Race
	ref.race_category_everyman_choices_ = JsonConverter::getSkillSet(jsonObj, "raceCategoryEverymanChoices", manager_);
	// Culture Type
	ref.culture_type_category_skill_ranks_ = JsonConverter::getSkillPrimitiveMap<int>(jsonObj, "cultureTypeCategorySkillRanks", manager_);
	// Profession
	ref.prof_skill_development_type_choices_ = JsonConverter::getSkillEnumMap<SkillDevelopmentType::Type>(jsonObj, "profSkillDevelopmentTypeChoices", manager_);
	ref.prof_category_development_type_choices_ = JsonConverter::getSkillEnumMap<SkillDevelopmentType::Type>(jsonObj, "profCategoryDevelopmentTypeChoices", manager_);
	ref.prof_group_development_type_choices_ = JsonConverter::getSkillEnumMap<SkillDevelopmentType::Type>(jsonObj, "profGroupDevelopmentTypeChoices", manager_);
	{
		const std::set<const SpellListData*> base_spell_lists = JsonConverter::getDataSet<SpellListData>(jsonObj, "baseSpellListChoices", manager_);
		for (const SpellListData* spell_list : base_spell_lists) {
			if (spell_list)
				ref.prof_base_spell_list_choices_.insert(const_cast<SpellListData*>(spell_list));
		}
	}

	/* Initial Stats */
	{
		const json::array stats_array = JsonConverter::getJsonArray(jsonObj, "initialStats");
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

			ref.initial_stats_.insert_or_assign(stat_type, stat);
		}
	}

	/* Hobby/Adolescent choices */
	ref.hobby_skill_ranks_ = JsonConverter::getSkillPrimitiveMap<int>(jsonObj, "hobbySkillRanks", manager_);
	ref.hobby_category_ranks_ = JsonConverter::getDataPrimitiveMap<SkillCategoryData, int>(jsonObj, "hobbyCategoryRanks", manager_);
	{
		const std::string adolescent_spell_list_choice_id = JsonConverter::getString(jsonObj, "adolescentSpellListChoice");
		if (!adolescent_spell_list_choice_id.empty())
			ref.adolescent_spell_list_choice_ = &manager_.get<SpellListData>(adolescent_spell_list_choice_id);
	}
	{
		const std::map<std::string, const LanguageAbility> language_abilities = JsonConverter::getLanguageAbilityMap(jsonObj, "adolescentLanguageChoices", manager_);
		for (const auto& [language_name, ability] : language_abilities) {
			ref.adolescent_language_choices_.emplace(ability);
		}
	}

	/* Background choices */
	{
		const json::array stats_array = JsonConverter::getJsonArray(jsonObj, "backgroundStats");
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

			ref.background_stats_.insert_or_assign(stat_type, stat);
		}
	}
	ref.background_extra_gold_ = JsonConverter::getInt(jsonObj, "backgroundExtraGold", 0);
	{
		const std::map<std::string, const LanguageAbility> language_abilities = JsonConverter::getLanguageAbilityMap(jsonObj, "backgroundLanguageChoices", manager_);
		for (const auto& [language_name, ability] : language_abilities) {
			ref.background_language_choices_.emplace(ability);
		}
	}
	ref.background_skill_special_bonuses_ = JsonConverter::getSkillPrimitiveMap<int>(jsonObj, "backgroundSkillSpecialBonuses", manager_);
	ref.background_category_special_bonuses_ = JsonConverter::getDataPrimitiveMap<SkillCategoryData, int>(jsonObj, "backgroundCategorySpecialBonuses", manager_);
	{
		const json::array items_array = JsonConverter::getJsonArray(jsonObj, "backgroundItems");
		for (const json::value& item_value : items_array) {
			if (!item_value.is_object())
				continue;
			const json::object item_obj = item_value.as_object();
			const std::string item = JsonConverter::getString(item_obj, "item");
			if (!item.empty())
				ref.background_items_.emplace_back(item);
		}
	}

	/* Aggregated state */
	ref.total_gold_ = JsonConverter::getInt(jsonObj, "totalGold", 0);
	{
		const std::map<std::string, const LanguageAbility> language_abilities = JsonConverter::getLanguageAbilityMap(jsonObj, "languageAbilities", manager_);
		for (const auto& [language_name, ability] : language_abilities) {
			ref.language_abilities_.emplace(ability);
		}
	}
	{
		const auto realm_progressions = JsonConverter::getEnumPrimitiveMap<RealmType::Type, std::string>(jsonObj, "realmProgressions");
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
	ref.everyman_skills_ = JsonConverter::getSkillSet(jsonObj, "everymanSkills", manager_);
	ref.restricted_skills_ = JsonConverter::getSkillSet(jsonObj, "restrictedSkills", manager_);
	ref.everyman_skill_categories_ = JsonConverter::getDataSet<SkillCategoryData>(jsonObj, "everymanSkillCategories", manager_);
	ref.restricted_skill_categories_ = JsonConverter::getDataSet<SkillCategoryData>(jsonObj, "restrictedSkillCategories", manager_);
	ref.skill_ranks_ = JsonConverter::getSkillPrimitiveMap<int>(jsonObj, "skillRanks", manager_);
	ref.skill_professional_bonuses_ = JsonConverter::getSkillPrimitiveMap<int>(jsonObj, "skillProfessionalBonuses", manager_);
	ref.skill_special_bonuses_ = JsonConverter::getSkillPrimitiveMap<int>(jsonObj, "skillSpecialBonuses", manager_);
	ref.skill_development_types_ = JsonConverter::getSkillEnumMap<SkillDevelopmentType::Type>(jsonObj, "skillDevelopmentTypes", manager_);
	ref.category_ranks_ = JsonConverter::getDataPrimitiveMap<SkillCategoryData, int>(jsonObj, "categoryRanks", manager_);
	ref.category_professional_bonuses_ = JsonConverter::getDataPrimitiveMap<SkillCategoryData, int>(jsonObj, "categoryProfessionalBonuses", manager_);
	ref.category_special_bonuses_ = JsonConverter::getDataPrimitiveMap<SkillCategoryData, int>(jsonObj, "categorySpecialBonuses", manager_);
	ref.category_development_types_ = JsonConverter::getDataEnumMap<SkillCategoryData, SkillDevelopmentType::Type>(jsonObj, "categoryDevelopmentTypes", manager_);
	{
		json::array skillCategoryDevelopmentCostArr = JsonConverter::getJsonArray(jsonObj, "categoryCosts");
		for (const auto& item : skillCategoryDevelopmentCostArr) {
			if (item.is_object()) {
				json::object obj = item.as_object();
				std::string category_id = JsonConverter::getString(obj, "category");
				std::string cost = JsonConverter::getString(obj, "cost");
				ref.category_development_costs_.insert_or_assign(&manager().get<SkillCategoryData>(category_id), std::move(rm::game::character::SkillDevelopmentCost(cost)));
			}
		}
	}
	ref.group_professional_bonuses_ = JsonConverter::getDataPrimitiveMap<SkillGroupData, int>(jsonObj, "groupProfessionalBonuses", manager_);
	ref.group_special_bonuses_ = JsonConverter::getDataPrimitiveMap<SkillGroupData, int>(jsonObj, "groupSpecialBonuses", manager_);
	ref.group_development_types_ = JsonConverter::getDataEnumMap<SkillGroupData, SkillDevelopmentType::Type>(jsonObj, "groupDevelopmentTypes", manager_);
	ref.spell_list_ranks_ = JsonConverter::getDataPrimitiveMap<SpellListData, int>(jsonObj, "spellListRanks", manager_);
	{
		const json::array items_array = JsonConverter::getJsonArray(jsonObj, "totalItems");
		for (const json::value& item_value : items_array) {
			if (!item_value.is_object())
				continue;
			const json::object item_obj = item_value.as_object();
			const std::string item = JsonConverter::getString(item_obj, "item");
			if (!item.empty())
				ref.total_items_.emplace_back(item);
		}
	}

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
		json::array adolescent_languages_array;
		for (const auto& [language_name, ability] : race->adolescentLanguageAbilities()) {
			json::object language_obj;
			JsonConverter::setString(language_obj, "language", ability.languageId());
			if (ability.isSomatic())
				JsonConverter::setInt(language_obj, "somantic", ability.somatic());
			if (ability.isSpoken())
				JsonConverter::setInt(language_obj, "spoken", ability.spoken());
			if (ability.isWritten())
				JsonConverter::setInt(language_obj, "written", ability.written());
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