#include <gtest/gtest.h>

#include <boost/json.hpp>

#include <CharacterBuilderSerializer.h>
#include <JsonConverter.h>
#include <PersistentCache.h>
#include <PersistentObjectManager.h>

#include <CultureData.h>
#include <CultureTypeData.h>
#include <LanguageData.h>
#include <ProfessionData.h>
#include <RaceData.h>
#include <SkillCategoryData.h>
#include <SkillData.h>
#include <SkillDevelopmentType.h>
#include <SkillGroupData.h>
#include <SkillProgressionTypeData.h>
#include <SpellListData.h>
#include <StatType.h>
#include <RealmType.h>

using namespace rm;
using namespace rm::game::character;
using namespace rm::rule;
using namespace rm::rule::enums;

namespace json = boost::json;

namespace {

const std::string kBuilderId{"CHARACTERBUILDER_RT_001"};
const std::string kRaceId{"RACE_RT"};
const std::string kCultureId{"CULTURE_RT"};
const std::string kCultureTypeId{"CULTURETYPE_RT"};
const std::string kProfessionId{"PROFESSION_RT"};
const std::string kCategoryId{"SKILLCATEGORY_RT"};
const std::string kSkillId{"SKILL_RT"};
const std::string kGroupId{"SKILLGROUP_RT"};
const std::string kSpellListId{"SPELLLIST_RT"};
const std::string kProgressionId{"SKILLPROGRESSIONTYPE_RT"};
const std::string kLanguageId{"LANGUAGE_RT"};
const std::string kSubcategory{"RT_SUB"};

json::object makeSkillRef(const std::string& id, const std::string& subcategory) {
	json::object obj;
	obj["id"] = id;
	obj["subcategory"] = subcategory;
	return obj;
}

json::object makeDataPrimitive(const std::string& id, int value) {
	json::object obj;
	obj["id"] = id;
	obj["value"] = value;
	return obj;
}

json::object makeDataEnum(const std::string& id, SkillDevelopmentType::Type type) {
	json::object obj;
	obj["id"] = id;
	obj["value"] = toString(type);
	return obj;
}

json::object makeEnumPrimitive(RealmType::Type realm, const std::string& value) {
	json::object obj;
	obj["id"] = toString(realm);
	obj["value"] = value;
	return obj;
}

json::object makeLanguage(const std::string& language_id, int spoken, int written) {
	json::object obj;
	obj["language"] = language_id;
	obj["spoken"] = spoken;
	obj["written"] = written;
	return obj;
}

void validateAllFields(const json::object& out, PersistentObjectManager& manager) {
	EXPECT_EQ(serial::JsonConverter::getString(out, "id"), kBuilderId);
	EXPECT_EQ(serial::JsonConverter::getString(out, "name"), "RoundTrip Character");
	EXPECT_EQ(serial::JsonConverter::getBool(out, "built"), true);
	EXPECT_EQ(serial::JsonConverter::getInt(out, "num_hobby_skill_ranks"), 4);
	EXPECT_EQ(serial::JsonConverter::getInt(out, "num_spell_list_ranks"), 2);

	EXPECT_EQ(serial::JsonConverter::getString(out, "race"), kRaceId);
	EXPECT_EQ(serial::JsonConverter::getString(out, "culture"), kCultureId);
	EXPECT_EQ(serial::JsonConverter::getString(out, "culture_type"), kCultureTypeId);
	EXPECT_EQ(serial::JsonConverter::getString(out, "profession"), kProfessionId);

	const std::set<RealmType::Type> magical_realms = serial::JsonConverter::getEnumSet<RealmType::Type>(out, "magical_realms");
	EXPECT_EQ(magical_realms.size(), 2);
	EXPECT_TRUE(magical_realms.contains(RealmType::kArcane));
	EXPECT_TRUE(magical_realms.contains(RealmType::kArms));

	const std::set<const SkillCategoryData*> race_category_choices = serial::JsonConverter::getDataSet<SkillCategoryData>(out, "race_category_everyman_choices", manager);
	EXPECT_EQ(race_category_choices.size(), 1);
	EXPECT_EQ((*race_category_choices.begin())->id(), kCategoryId);

	const std::map<std::string, const LanguageRanks> race_langs = serial::JsonConverter::getLanguageAbilityMap(out, "race_adolescent_language_choices", manager);
	EXPECT_EQ(race_langs.size(), 1);
	EXPECT_EQ(race_langs.at(kLanguageId).spoken(), 3);
	EXPECT_EQ(race_langs.at(kLanguageId).written(), 2);

	const std::map<const SubcategoriedSkillData*, int> culture_type_skill_ranks = serial::JsonConverter::getSkillPrimitiveMap<int>(out, "culture_type_category_skill_ranks", manager);
	EXPECT_EQ(culture_type_skill_ranks.size(), 1);
	EXPECT_EQ(culture_type_skill_ranks.begin()->first->skillData().id(), kSkillId);
	EXPECT_EQ(culture_type_skill_ranks.begin()->first->subcategory().value(), kSubcategory);
	EXPECT_EQ(culture_type_skill_ranks.begin()->second, 7);

	const std::set<const SpellListData*> base_spell_lists = serial::JsonConverter::getDataSet<SpellListData>(out, "base_spell_list_choices", manager);
	EXPECT_EQ(base_spell_lists.size(), 1);
	EXPECT_EQ((*base_spell_lists.begin())->id(), kSpellListId);

	const std::map<const SubcategoriedSkillData*, SkillDevelopmentType::Type> prof_skill_subcat_types =
	    serial::JsonConverter::getSkillEnumMap<SkillDevelopmentType::Type>(out, "prof_skill_subcategory_development_type_choices", manager);
	EXPECT_EQ(prof_skill_subcat_types.size(), 1);
	EXPECT_EQ(prof_skill_subcat_types.begin()->second, SkillDevelopmentType::kEveryman);

	const std::map<const SubcategoriedSkillData*, SkillDevelopmentType::Type> prof_skill_types =
	    serial::JsonConverter::getSkillEnumMap<SkillDevelopmentType::Type>(out, "prof_skill_development_type_choices", manager);
	EXPECT_EQ(prof_skill_types.size(), 1);
	EXPECT_EQ(prof_skill_types.begin()->second, SkillDevelopmentType::kOccupational);

	const std::map<const SkillData*, SkillDevelopmentType::Type> prof_cat_types =
	    serial::JsonConverter::getDataEnumMap<SkillData, SkillDevelopmentType::Type>(out, "prof_category_development_type_choices", manager);
	EXPECT_EQ(prof_cat_types.size(), 1);
	EXPECT_EQ(prof_cat_types.begin()->first->id(), kSkillId);
	EXPECT_EQ(prof_cat_types.begin()->second, SkillDevelopmentType::kRestricted);

	const std::map<const SkillData*, SkillDevelopmentType::Type> prof_group_types =
	    serial::JsonConverter::getDataEnumMap<SkillData, SkillDevelopmentType::Type>(out, "prof_group_development_type_choices", manager);
	EXPECT_EQ(prof_group_types.size(), 1);
	EXPECT_EQ(prof_group_types.begin()->first->id(), kSkillId);
	EXPECT_EQ(prof_group_types.begin()->second, SkillDevelopmentType::kEveryman);

	const std::map<const SubcategoriedSkillData*, int> hobby_skill_ranks = serial::JsonConverter::getSkillPrimitiveMap<int>(out, "hobby_skill_ranks", manager);
	EXPECT_EQ(hobby_skill_ranks.size(), 1);
	EXPECT_EQ(hobby_skill_ranks.begin()->second, 2);

	const std::map<const SkillCategoryData*, int> hobby_category_ranks = serial::JsonConverter::getDataPrimitiveMap<SkillCategoryData, int>(out, "hobby_category_ranks", manager);
	EXPECT_EQ(hobby_category_ranks.size(), 1);
	EXPECT_EQ(hobby_category_ranks.begin()->first->id(), kCategoryId);
	EXPECT_EQ(hobby_category_ranks.begin()->second, 1);

	const std::map<std::string, const LanguageRanks> background_langs = serial::JsonConverter::getLanguageAbilityMap(out, "background_language_choices", manager);
	EXPECT_EQ(background_langs.size(), 1);
	EXPECT_EQ(background_langs.at(kLanguageId).spoken(), 6);
	EXPECT_EQ(background_langs.at(kLanguageId).written(), 5);

	const std::map<std::string, const LanguageRanks> language_abilities = serial::JsonConverter::getLanguageAbilityMap(out, "language_abilities", manager);
	EXPECT_EQ(language_abilities.size(), 1);
	EXPECT_EQ(language_abilities.at(kLanguageId).spoken(), 8);
	EXPECT_EQ(language_abilities.at(kLanguageId).written(), 7);

	const std::map<RealmType::Type, std::string> realm_progressions = serial::JsonConverter::getEnumPrimitiveMap<RealmType::Type, std::string>(out, "realm_progressions");
	EXPECT_EQ(realm_progressions.size(), 1);
	EXPECT_EQ(realm_progressions.at(RealmType::kArcane), kProgressionId);

	const json::array stats = serial::JsonConverter::getJsonArray(out, "stats");
	EXPECT_EQ(stats.size(), 1);
	{
		const json::object stat = stats[0].as_object();
		EXPECT_EQ(serial::JsonConverter::getString(stat, "stat"), toString(StatType::kAgility));
		EXPECT_EQ(serial::JsonConverter::getInt(stat, "temporary"), 75);
		EXPECT_EQ(serial::JsonConverter::getInt(stat, "potential"), 90);
		EXPECT_EQ(serial::JsonConverter::getInt(stat, "bonus"), 12);
	}

	const std::set<const SubcategoriedSkillData*> everyman_skills = serial::JsonConverter::getSkillSet(out, "everyman_skills", manager);
	EXPECT_EQ(everyman_skills.size(), 1);

	const std::set<const SubcategoriedSkillData*> restricted_skills = serial::JsonConverter::getSkillSet(out, "restricted_skills", manager);
	EXPECT_EQ(restricted_skills.size(), 1);

	const std::set<const SkillCategoryData*> everyman_categories = serial::JsonConverter::getDataSet<SkillCategoryData>(out, "everyman_skill_categories", manager);
	EXPECT_EQ(everyman_categories.size(), 1);

	const std::set<const SkillCategoryData*> restricted_categories = serial::JsonConverter::getDataSet<SkillCategoryData>(out, "restricted_skill_categories", manager);
	EXPECT_EQ(restricted_categories.size(), 1);

	EXPECT_EQ(serial::JsonConverter::getSkillPrimitiveMap<int>(out, "skill_ranks", manager).size(), 1);
	EXPECT_EQ(serial::JsonConverter::getSkillPrimitiveMap<int>(out, "skill_professional_bonuses", manager).size(), 1);
	EXPECT_EQ(serial::JsonConverter::getSkillEnumMap<SkillDevelopmentType::Type>(out, "skillsub_development_types", manager).size(), 1);

	const auto skill_development_types =
	    serial::JsonConverter::getDataEnumMap<SkillData, SkillDevelopmentType::Type>(out, "skill_development_types", manager);
	EXPECT_EQ(skill_development_types.size(), 1u);

	const auto category_ranks =
	    serial::JsonConverter::getDataPrimitiveMap<SkillCategoryData, int>(out, "category_ranks", manager);
	EXPECT_EQ(category_ranks.size(), 1u);

	const auto category_professional_bonuses =
	    serial::JsonConverter::getDataPrimitiveMap<SkillCategoryData, int>(out, "category_professional_bonuses", manager);
	EXPECT_EQ(category_professional_bonuses.size(), 1u);

	const auto category_special_bonuses =
	    serial::JsonConverter::getDataPrimitiveMap<SkillCategoryData, int>(out, "category_special_bonuses", manager);
	EXPECT_EQ(category_special_bonuses.size(), 1u);

	const auto category_development_types =
	    serial::JsonConverter::getDataEnumMap<SkillCategoryData, SkillDevelopmentType::Type>(out, "category_development_types", manager);
	EXPECT_EQ(category_development_types.size(), 1u);

	const auto group_professional_bonuses =
	    serial::JsonConverter::getDataPrimitiveMap<SkillGroupData, int>(out, "group_professional_bonuses", manager);
	EXPECT_EQ(group_professional_bonuses.size(), 1u);

	const auto group_special_bonuses =
	    serial::JsonConverter::getDataPrimitiveMap<SkillGroupData, int>(out, "group_special_bonuses", manager);
	EXPECT_EQ(group_special_bonuses.size(), 1u);

	const auto group_development_types =
	    serial::JsonConverter::getDataEnumMap<SkillGroupData, SkillDevelopmentType::Type>(out, "group_development_types", manager);
	EXPECT_EQ(group_development_types.size(), 1u);
}

TEST(CharacterBuilderSerializer, RoundTrip_AllFieldsValidated) {
	PersistentCache cache{};
	PersistentObjectManager manager(cache);
	serial::CharacterBuilderSerializer serializer(manager);

	// Seed referenced objects used by all map/set fields
	manager.get<RaceData>(kRaceId);
	manager.get<CultureData>(kCultureId);
	manager.get<CultureTypeData>(kCultureTypeId);
	manager.get<ProfessionData>(kProfessionId);
	manager.get<SkillCategoryData>(kCategoryId);
	manager.get<SkillData>(kSkillId);
	manager.get<SkillGroupData>(kGroupId);
	manager.get<SpellListData>(kSpellListId);
	manager.get<SkillProgressionTypeData>(kProgressionId);
	LanguageData& language = manager.get<LanguageData>(kLanguageId);
	language.setName("RT Language");

	json::object input{};
	input["id"] = kBuilderId;
	input["name"] = "RoundTrip Character";
	input["built"] = true;
	input["num_hobby_skill_ranks"] = 4;
	input["num_spell_list_ranks"] = 2;

	input["race"] = kRaceId;
	input["culture"] = kCultureId;
	input["culture_type"] = kCultureTypeId;
	input["profession"] = kProfessionId;

	input["magical_realms"] = json::array{toString(RealmType::kArcane), toString(RealmType::kArms)};

	input["race_category_everyman_choices"] = json::array{kCategoryId};
	input["race_adolescent_language_choices"] = json::array{makeLanguage(kLanguageId, 3, 2)};
	input["culture_type_category_skill_ranks"] = json::array{json::object{{"id", kSkillId}, {"subcategory", kSubcategory}, {"value", 7}}};
	input["base_spell_list_choices"] = json::array{kSpellListId};

	input["prof_skill_subcategory_development_type_choices"] = json::array{json::object{{"id", kSkillId}, {"subcategory", kSubcategory}, {"value", toString(SkillDevelopmentType::kEveryman)}}};
	input["prof_skill_development_type_choices"] = json::array{json::object{{"id", kSkillId}, {"subcategory", kSubcategory}, {"value", toString(SkillDevelopmentType::kOccupational)}}};
	input["prof_category_development_type_choices"] = json::array{makeDataEnum(kSkillId, SkillDevelopmentType::kRestricted)};
	input["prof_group_development_type_choices"] = json::array{makeDataEnum(kSkillId, SkillDevelopmentType::kEveryman)};
	input["hobby_skill_ranks"] = json::array{json::object{{"id", kSkillId}, {"subcategory", kSubcategory}, {"value", 2}}};
	input["hobby_category_ranks"] = json::array{makeDataPrimitive(kCategoryId, 1)};
	input["background_language_choices"] = json::array{makeLanguage(kLanguageId, 6, 5)};

	input["language_abilities"] = json::array{makeLanguage(kLanguageId, 8, 7)};
	input["realm_progressions"] = json::array{makeEnumPrimitive(RealmType::kArcane, kProgressionId)};
	input["stats"] = json::array{json::object{{"stat", toString(StatType::kAgility)}, {"temporary", 75}, {"potential", 90}, {"bonus", 12}}};

	input["everyman_skills"] = json::array{makeSkillRef(kSkillId, kSubcategory)};
	input["restricted_skills"] = json::array{makeSkillRef(kSkillId, kSubcategory)};
	input["everyman_skill_categories"] = json::array{kCategoryId};
	input["restricted_skill_categories"] = json::array{kCategoryId};

	input["skill_ranks"] = json::array{json::object{{"id", kSkillId}, {"subcategory", kSubcategory}, {"value", 11}}};
	input["skill_professional_bonuses"] = json::array{json::object{{"id", kSkillId}, {"subcategory", kSubcategory}, {"value", 4}}};
	input["skillsub_development_types"] = json::array{json::object{{"id", kSkillId}, {"subcategory", kSubcategory}, {"value", toString(SkillDevelopmentType::kRestricted)}}};
	input["skill_development_types"] = json::array{makeDataEnum(kSkillId, SkillDevelopmentType::kOccupational)};
	input["category_ranks"] = json::array{makeDataPrimitive(kCategoryId, 9)};
	input["category_professional_bonuses"] = json::array{makeDataPrimitive(kCategoryId, 3)};
	input["category_special_bonuses"] = json::array{makeDataPrimitive(kCategoryId, 1)};
	input["category_development_types"] = json::array{json::object{{"id", kCategoryId}, {"value", toString(SkillDevelopmentType::kEveryman)}}};

	input["group_professional_bonuses"] = json::array{makeDataPrimitive(kGroupId, 2)};
	input["group_special_bonuses"] = json::array{makeDataPrimitive(kGroupId, 1)};
	input["group_development_types"] = json::array{json::object{{"id", kGroupId}, {"value", toString(SkillDevelopmentType::kRestricted)}}};

	input["spell_list_ranks"] = json::array{makeDataPrimitive(kSpellListId, 3)};

	// round-trip 1
	serializer.deserializeObject(input);
	const CharacterBuilder& first = manager.get<CharacterBuilder>(kBuilderId);
	const json::object out1 = serializer.serializeObject(first).as_object();
	validateAllFields(out1, manager);

	// round-trip 2
	serializer.deserializeObject(const_cast<json::object&>(out1));
	const CharacterBuilder& second = manager.get<CharacterBuilder>(kBuilderId);
	const json::object out2 = serializer.serializeObject(second).as_object();
	validateAllFields(out2, manager);
}

} // namespace