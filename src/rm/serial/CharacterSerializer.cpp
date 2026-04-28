#include <CharacterSerializer.h>
#include <JsonConverter.h>
#include <EnumIterator.h>

namespace rm::serial {

json::value CharacterSerializer::serializeObject(const Character& ref) const {
	json::object obj;

	// Identity
	JsonConverter::setString(obj, "id", ref.id());
	JsonConverter::setString(obj, "name", ref.name_);
	JsonConverter::setInt(obj, "level", ref.level());
	JsonConverter::setInt(obj, "experiencePoints", ref.experiencePoints());
	JsonConverter::setBool(obj, "male", ref.male_);
	JsonConverter::setBool(obj, "playerCharacter", ref.player_character_);
	JsonConverter::setInt(obj, "gold", ref.gold_);
	JsonConverter::setStringSet(obj, "items", ref.items_);

	// Core rule data references
	if (ref.race_)
		JsonConverter::setString(obj, "race", ref.race_->id());
	if (ref.culture_)
		JsonConverter::setString(obj, "culture", ref.culture_->id());
	if (ref.profession_)
		JsonConverter::setString(obj, "profession", ref.profession_->id());

	// Stats
	{
		json::array stats_array;
		for (auto value : archid::enum_range(StatType::kAgility, StatType::kStrength)) {
			if (!ref.stats_.contains(value))
				continue;
			const auto& stat = ref.stats_.at(value);
			json::object stat_obj;
			JsonConverter::setString(stat_obj, "stat", toString(value));
			JsonConverter::setInt(stat_obj, "temporary", stat.temporary());
			JsonConverter::setInt(stat_obj, "potential", stat.potential());
			JsonConverter::setInt(stat_obj, "racialBonus", stat.racialBonus());
			JsonConverter::setInt(stat_obj, "totalBonus", stat.bonus());
			stats_array.emplace_back(std::move(stat_obj));
		}
		if (stats_array.size())
			obj["stats"] = std::move(stats_array);
	}

	/* Physical characteristics */
	JsonConverter::setInt(obj, "height", ref.height_);
	JsonConverter::setInt(obj, "weight", ref.weight_);
	JsonConverter::setString(obj, "buildDescription", ref.build_description_);
	JsonConverter::setInt(obj, "lifespan", ref.lifespan_);

	/* Derived data */
	JsonConverter::setInt(obj, "developmentPoints", ref.development_points_);
	JsonConverter::setEnumSet(obj, "magicalRealms", ref.power_realms_);
	JsonConverter::setEnumPrimitiveMap(obj, "resistances", ref.resistances_);
	// Spell List Categories
	{
		// First sort the categories alphabetically by their ID to ensure a consistent order in the JSON output, which is important for testing and readability.
		std::map<std::string_view, const SkillCategoryData*> categories{};
		for (const auto& [category, list_set] : ref.spell_list_categories_) {
			categories[category->id()] = category;
		}
		json::array arr;
		for (const auto& [category_id, category] : categories) {
			const auto& list_set = ref.spell_list_categories_.at(category);
			json::object category_obj;
			JsonConverter::setString(category_obj, "category", std::string(category_id));
			JsonConverter::setDataSet<SpellListData>(category_obj, "spellLists", list_set);
			arr.emplace_back(std::move(category_obj));
		}
		obj["spellListCategories"] = std::move(arr);
	}

	JsonConverter::setInt(obj, "maxHits", ref.max_hits_);
	JsonConverter::setInt(obj, "hits", ref.hits_);
	JsonConverter::setInt(obj, "maxPowerPoints", ref.max_power_points_);
	JsonConverter::setInt(obj, "powerPoints", ref.power_points_);

	/* Learned Abilities */
	// Languages
	{
		std::map<std::string, const LanguageRanks> language_abilities;
		for (const auto& [id, ability] : ref.language_abilities_) {
			language_abilities.emplace(ability.languageId(), ability);
		}
		JsonConverter::setLanguageAbilities(obj, "languageAbilities", language_abilities);
	}

	// Spell Lists
	{
		std::map<std::string_view, const SpellListData*> sorted_spell_lists{};
		for (const auto& [spell_list, ranks] : ref.spell_list_ranks_) {
			sorted_spell_lists[spell_list->id()] = spell_list;
		}
		json::array spell_list_ranks_array;
		for (const auto& [spell_list_id, spell_list] : sorted_spell_lists) {
			const auto& ranks = ref.spell_list_ranks_.at(spell_list);
			json::object spell_list_obj;
			JsonConverter::setString(spell_list_obj, "id", std::string(spell_list_id));
			JsonConverter::setInt(spell_list_obj, "value", ranks);
			spell_list_ranks_array.emplace_back(std::move(spell_list_obj));
		}
		if (spell_list_ranks_array.size())
			obj["spellListRanks"] = std::move(spell_list_ranks_array);
	}

	// Categories
	{
		// First sort the categories alphabetically by their ID to ensure a consistent order in the JSON output, which is important for testing and readability.
		std::map<std::string_view, const SkillCategoryData*> categories{};
		for (const auto& [category, char_category] : ref.categories_) {
			categories[category->id()] = category;
		}
		json::array categories_array;
		for (const auto& [category_id, category] : categories) {
			const auto& char_category = ref.categories_.at(category);
			json::object category_obj;
			JsonConverter::setString(category_obj, "id", std::string(category_id));
			JsonConverter::setString(category_obj, "progression", char_category.progression_type_->id());
			JsonConverter::setString(category_obj, "developmentCost", char_category.development_cost_.toString());
			JsonConverter::setEnumSet(category_obj, "stats", char_category.stats_);
			JsonConverter::setInt(category_obj, "professionBonus", char_category.profession_bonus_);
			JsonConverter::setInt(category_obj, "ranks", char_category.ranks_);
			JsonConverter::setInt(category_obj, "specialBonus", char_category.special_bonus_);
			JsonConverter::setInt(category_obj, "totalBonus", char_category.bonus());
			categories_array.emplace_back(std::move(category_obj));
		}
		if (categories_array.size())
			obj["categories"] = std::move(categories_array);
	}

	// Skills
	{
		// First sort the skills alphabetically by their ID to ensure a consistent order in the JSON output, which is important for testing and readability.
		std::map<std::string_view, const SubcategoriedSkillData*> skills{};
		for (const auto& [sub_skill, char_skill] : ref.skills_) {
			skills[sub_skill->id()] = sub_skill;
		}
		json::array skills_array;
		for (const auto& [sub_skill_id, sub_skill] : skills) {
			const auto& char_skill = ref.skills_.at(sub_skill);
			json::object skill_obj;
			json::object skill_data_obj = JsonConverter::setSkill(*sub_skill);
			skill_obj["skillData"] = std::move(skill_data_obj);
			JsonConverter::setString(skill_obj, "progression", char_skill.progression_type_->id());
			JsonConverter::setString(skill_obj, "developmentType", toString(char_skill.development_type_));
			JsonConverter::setInt(skill_obj, "professionBonus", char_skill.profession_bonus_);
			JsonConverter::setInt(skill_obj, "ranks", char_skill.ranks_);
			JsonConverter::setInt(skill_obj, "specialBonus", char_skill.special_bonus_);
			JsonConverter::setInt(skill_obj, "totalBonus", char_skill.bonus());
			skills_array.emplace_back(std::move(skill_obj));
		}
		if (skills_array.size())
			obj["skills"] = std::move(skills_array);
	}

	return obj;
}

const Character& CharacterSerializer::deserializeObject(json::object& jsonObj) const {
	std::string id = JsonConverter::getString(jsonObj, "id");

	// If this object does not have an ID, create a new Character to get a cached object with a generated ID.
	if (id.empty()) {
		Character& tmp = manager_.get<Character>();
		id = tmp.id(); // Generate a new ID for this character builder
	}

	// Now we know we have an id, we can get the reference to the Character that we will populate with data. This will be either the newly created one (if there was no ID in the JSON) or an existing one (if there was an ID in the
	// JSON).
	Character& ref = manager_.get<Character>(id);

	// Populate the character's data from the JSON object
	ref.setName(JsonConverter::getString(jsonObj, "name"));
	ref.male_ = JsonConverter::getBool(jsonObj, "male");
	ref.level_ = JsonConverter::getInt(jsonObj, "level", 1);
	ref.experience_points_ = JsonConverter::getInt(jsonObj, "experiencePoints", 10000);
	ref.player_character_ = JsonConverter::getBool(jsonObj, "playerCharacter", false);
	ref.gold_ = JsonConverter::getInt(jsonObj, "gold", 0);
	ref.items_ = JsonConverter::getStringVector(jsonObj, "items");

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
		const std::string profession_id = JsonConverter::getString(jsonObj, "profession");
		if (!profession_id.empty())
			ref.profession_ = &manager_.get<ProfessionData>(profession_id);
	}

	// Stats
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
			const int racial_bonus = JsonConverter::getInt(stat_obj, "racialBonus", 0);

			Stat stat{};
			stat.updateTemporary(temporary); // We use update rather than set as we don't need to generate the potential value.
			stat.setPotential(potential);
			stat.setRacialBonus(racial_bonus);
			ref.stats_.insert_or_assign(stat_type, stat);
		}
	}

	/* Physical characteristics */
	ref.height_ = JsonConverter::getInt(jsonObj, "height", 0);
	ref.weight_ = JsonConverter::getInt(jsonObj, "weight", 0);
	ref.build_description_ = JsonConverter::getString(jsonObj, "buildDescription");
	ref.lifespan_ = JsonConverter::getInt(jsonObj, "lifespan", 0);

	/* Derived data */
	ref.development_points_ = JsonConverter::getInt(jsonObj, "developmentPoints", 0);
	ref.power_realms_ = JsonConverter::getEnumSet<RealmType::Type>(jsonObj, "magicalRealms");
	ref.resistances_ = JsonConverter::getEnumPrimitiveMap<ResistanceType::Type, int>(jsonObj, "resistances");
	// Spell List Categories
	{
		const json::array spell_list_categories_array = JsonConverter::getJsonArray(jsonObj, "spellListCategories");
		for (const json::value& category_value : spell_list_categories_array) {
			if (!category_value.is_object())
				continue;
			const json::object category_obj = category_value.as_object();
			const std::string category_id = JsonConverter::getString(category_obj, "category");
			const SkillCategoryData* category_data = &manager_.get<SkillCategoryData>(category_id);
			std::set<const SpellListData*> spell_lists = JsonConverter::getDataSet<SpellListData>(category_obj, "spellLists", manager_);
			ref.spell_list_categories_.insert_or_assign(category_data, std::move(spell_lists));
		}
	}

	/* Learned Abilities */
	// Languages
	{
		const std::map<std::string, const LanguageRanks> language_abilities = JsonConverter::getLanguageAbilityMap(jsonObj, "languageAbilities", manager_);
		for (const auto& [id, ability] : language_abilities) {
			ref.language_abilities_.insert_or_assign(id, ability);
		}
	}
	
	// Spell Lists
	{
		const json::array spell_list_ranks_array = JsonConverter::getJsonArray(jsonObj, "spellListRanks");
		for (const json::value& spell_list_value : spell_list_ranks_array) {
			if (!spell_list_value.is_object())
				continue;
			const json::object spell_list_obj = spell_list_value.as_object();
			const std::string spell_list_id = JsonConverter::getString(spell_list_obj, "id");
			const SpellListData* spell_list_data = &manager_.get<SpellListData>(spell_list_id);
			const int ranks = JsonConverter::getInt(spell_list_obj, "value", 0);
			ref.spell_list_ranks_.insert_or_assign(spell_list_data, ranks);
		}
	}

	// Categories
	{
		const json::array categories_array = JsonConverter::getJsonArray(jsonObj, "categories");
		for (const json::value& category_value : categories_array) {
			if (!category_value.is_object())
				continue;
			const json::object category_obj = category_value.as_object();
			const std::string category_id = JsonConverter::getString(category_obj, "id");
			const SkillCategoryData* category_data = &manager_.get<SkillCategoryData>(category_id);
			Category char_category{};
			char_category.category_data_ = category_data;
			char_category.progression_type_ = &manager_.get<SkillProgressionTypeData>(JsonConverter::getString(category_obj, "progression"));
			char_category.development_cost_ = rm::game::character::SkillDevelopmentCost(JsonConverter::getString(category_obj, "developmentCost"));
			char_category.stats_ = JsonConverter::getEnumVector<StatType::Type>(category_obj, "stats");
			char_category.profession_bonus_ = JsonConverter::getInt(category_obj, "professionBonus", 0);
			char_category.ranks_ = JsonConverter::getInt(category_obj, "ranks", 0);
			char_category.special_bonus_ = JsonConverter::getInt(category_obj, "specialBonus", 0);
			ref.categories_.insert_or_assign(category_data, std::move(char_category));
		}
	}
	// Skills
	{
		const json::array skills_array = JsonConverter::getJsonArray(jsonObj, "skills");
		for (const json::value& skill_value : skills_array) {
			if (!skill_value.is_object())
				continue;
			const json::object skill_obj = skill_value.as_object();

			const json::object& skill_data_obj = skill_obj.at("skillData").as_object();
			const SubcategoriedSkillData* sub_skill_data = JsonConverter::getSkill(skill_data_obj, manager_);
			Skill char_skill{};
			try {
				char_skill.skill_data_ = sub_skill_data;
				char_skill.category_ = &ref.categories_.at(&sub_skill_data->skillData().category());
				char_skill.progression_type_ = &manager_.get<SkillProgressionTypeData>(JsonConverter::getString(skill_obj, "progression"));
				fromString(JsonConverter::getString(skill_obj, "developmentType"), char_skill.development_type_);
				char_skill.profession_bonus_ = JsonConverter::getInt(skill_obj, "professionBonus", 0);
				char_skill.ranks_ = JsonConverter::getInt(skill_obj, "ranks", 0);
				char_skill.special_bonus_ = JsonConverter::getInt(skill_obj, "specialBonus", 0);
				// We do not need to set the total bonus as this is derived data that is calculated from the other values, so it will be generated when the character's data is updated after deserialization.
				ref.skills_.insert_or_assign(sub_skill_data, std::move(char_skill));
			} catch (const std::exception& e) {
				// If there was an error deserializing the skills, log the error and continue deserializing the rest of the character's data. This will allow us to still load the character even if there was an issue with the skills,
				// which is better than not loading the character at all.
				std::cerr << "Error deserializing character skills: " << e.what() << std::endl;
			}
		}
	}

	/* Hits & PPs */
	ref.hits_ = JsonConverter::getInt(jsonObj, "hits", 0);
	ref.body_devlopment_skill_ = &manager_.subcategoriedSkillData(Character::BD_SKILL_ID);
	ref.power_points_ = JsonConverter::getInt(jsonObj, "powerPoints", 0);
	ref.power_point_skill_ = &manager_.subcategoriedSkillData(Character::PP_SKILL_ID);

	// Finally perform any derived data updates that are necessary after deserialization. This will ensure that all the character's data is consistent and ready to be used in the game.
	ref.updateAllDerivedData();

	return ref;
}

} // namespace rm::serial