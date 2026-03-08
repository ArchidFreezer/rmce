#include <TrainingPackageSerializer.h>
#include <JsonConverter.h>

namespace rm::rule::serial {

json::value TrainingPackageSerializer::serializeObject(const TrainingPackageData& ref) const {
	json::object obj;

	JsonConverter::setString(obj, "id", ref.id());
	JsonConverter::setString(obj, "name", ref.name());
	JsonConverter::setString(obj, "description", ref.description());
	if (!ref.flavourText().empty())
		JsonConverter::setString(obj, "flavour-text", ref.flavourText());
	JsonConverter::setString(obj, "book", ref.book().id());
	setQualifiers(obj, "qualifiers", ref.qualifiers());
	JsonConverter::setStringSet(obj, "notes", ref.notes());
	JsonConverter::setDataSet(obj, "races", ref.races());
	JsonConverter::setBool(obj, "lifestyle", ref.lifestyle());
	JsonConverter::setInt(obj, "time-to-acquire", ref.timeToAcquire());
	if (!ref.startingMoneyModifierDice().empty())
		JsonConverter::setString(obj, "starting-money-modifier-dice", ref.startingMoneyModifierDice());
	setSpecials(obj, "specials", ref.specials());
	if (!ref.statGains().empty())
		JsonConverter::setEnumSet(obj, "stat-gains", ref.statGains());
	if (ref.realmStatGain())
		JsonConverter::setBool(obj, "realm-stat-gain", ref.realmStatGain());
	if (ref.statGainChoices().numChoices())
		setStatGainChoices(obj, "stat-gain-choices", ref.statGainChoices());
	if (!ref.skillRanks().empty())
		JsonConverter::setSkillPrimitiveMap(obj, "skill-ranks", ref.skillRanks());
	if (!ref.skillRankChoices().empty())
		JsonConverter::setSkillChoicePrimitiveMap(obj, "skill-rank-choices", ref.skillRankChoices());
	if (!ref.skillCategoryRanks().empty())
		JsonConverter::setDataPrimitiveMap(obj, "category-ranks", ref.skillCategoryRanks());
	if (!ref.skillCategoryMultiSkillRankChoices().empty())
		setCategoryMultiSkillRankChoices(obj, "category-multi-skill-rank-choices", ref.skillCategoryMultiSkillRankChoices());
	if (!ref.skillGroupMultiSkillRankChoices().empty())
		setGroupMultiSkillRankChoices(obj, "group-multi-skill-rank-choices", ref.skillGroupMultiSkillRankChoices());
	if (!ref.skillGroupCategoryAndSkillRanks().empty())
		setSkillGroupCategoryAndSkillRankChoices(obj, "group-category-and-skill-rank-choices", ref.skillGroupCategoryAndSkillRanks());
	if (!ref.spellListChoices().empty())
		setSpellListChoices(obj, "spell-list-ranks", ref.spellListChoices());
	if (!ref.spellListCategoryChoices().empty())
		setSpellListCategoryChoices(obj, "spell-list-category-rank-choices", ref.spellListCategoryChoices());
	if (!ref.lifestyleSkills().empty())
		JsonConverter::setSkillSet(obj, "lifestyle-skills", ref.lifestyleSkills());
	if (!ref.lifestyleSkillCategories().empty())
		JsonConverter::setDataSet(obj, "lifestyle-categories", ref.lifestyleSkillCategories());
	if (!ref.lifestyleSkillGroups().empty())
		JsonConverter::setDataSet(obj, "lifestyle-groups", ref.lifestyleSkillGroups());
	if (!ref.lifestyleCategorySkillChoices().empty())
		JsonConverter::setDataChoiceSet<SkillCategoryData>(obj, "lifestyle-category-skill-choices", ref.lifestyleCategorySkillChoices());
	if (!ref.languageChoices().empty())
		JsonConverter::setDataChoicePrimitiveMap<LanguageData, int>(obj, "language-choices", ref.languageChoices());

	return obj;
}

const TrainingPackageData& TrainingPackageSerializer::deserializeObject(json::object& jsonObj) const {
	std::string id = JsonConverter::getString(jsonObj, "id");
	TrainingPackageData& ref = manager_.get<TrainingPackageData>(id);
	ref.setName(JsonConverter::getString(jsonObj, "name"));
	ref.setDescription(JsonConverter::getString(jsonObj, "description"));

	std::optional<std::string> flavour_text = JsonConverter::getOptionalString(jsonObj, "flavour-text");
	if (flavour_text)
		ref.setFlavourText(*flavour_text);

	// get the book from the cache
	ref.setBook(manager_.get<BookData>(JsonConverter::getString(jsonObj, "book")));
	ref.setQualifiers(getQualifiers(jsonObj, "qualifiers"));

	ref.setNotes(JsonConverter::getStringSet(jsonObj, "notes"));
	ref.setRaces(JsonConverter::getDataSet<RaceData>(jsonObj, "races", manager_));
	ref.setLifestyle(JsonConverter::getBool(jsonObj, "lifestyle"));
	ref.setTimeToAcquire(JsonConverter::getInt(jsonObj, "time-to-acquire", 0));
	std::optional<std::string> starting_money_dice = JsonConverter::getOptionalString(jsonObj, "starting-money-modifier-dice");
	if (starting_money_dice)
		ref.setStartingMoneyModifierDice(starting_money_dice.value());
	ref.setSpecials(getSpecials(jsonObj, "specials"));
	if (jsonObj.find("stat-gains") != jsonObj.end())
		ref.setStatGains(JsonConverter::getEnumSet<StatType::Type>(jsonObj, "stat-gains"));
	if (jsonObj.find("realm-stat-gain") != jsonObj.end())
		ref.setRealmStatGain(JsonConverter::getBool(jsonObj, "realm-stat-gain"));
	if (jsonObj.find("stat-gain-choices") != jsonObj.end())
		ref.setStatGainChoices(getStatGainChoices(jsonObj, "stat-gain-choices"));
	if (jsonObj.find("skill-ranks") != jsonObj.end())
		ref.setSkillRanks(JsonConverter::getSkillPrimitiveMap<int>(jsonObj, "skill-ranks", manager_));
	if (jsonObj.find("skill-rank-choices") != jsonObj.end())
		ref.setSkillRankChoices(JsonConverter::getSkillChoicePrimitiveMap<int>(jsonObj, "skill-rank-choices", manager_));
	if (jsonObj.find("category-ranks") != jsonObj.end())
		ref.setSkillCategoryRanks(JsonConverter::getDataPrimitiveMap<SkillCategoryData, int>(jsonObj, "category-ranks", manager_));
	if (jsonObj.find("category-multi-skill-rank-choices") != jsonObj.end())
		ref.setSkillCategoryMultiSkillRankChoices(getCategoryMultiSkillRankChoices(jsonObj, "category-multi-skill-rank-choices"));
	if (jsonObj.find("group-multi-skill-rank-choices") != jsonObj.end())
		ref.setSkillGroupMultiSkillRankChoices(getGroupMultiSkillRankChoices(jsonObj, "group-multi-skill-rank-choices"));
	if (jsonObj.find("group-category-and-skill-rank-choices") != jsonObj.end())
		ref.setSkillGroupCategoryAndSkillRanks(getSkillGroupCategoryAndSkillRankChoices(jsonObj, "group-category-and-skill-rank-choices"));
	if (jsonObj.find("spell-list-ranks") != jsonObj.end())
		ref.setSpellListChoices(getSpellListChoices(jsonObj, "spell-list-ranks"));
	if (jsonObj.find("spell-list-category-rank-choices") != jsonObj.end())
		ref.setSpellListCategoryChoices(getSpellListCategoryChoices(jsonObj, "spell-list-category-rank-choices"));
	if (jsonObj.find("lifestyle-skills") != jsonObj.end())
		ref.setLifestyleSkills(JsonConverter::getSkillSet(jsonObj, "lifestyle-skills", manager_));
	if (jsonObj.find("lifestyle-categories") != jsonObj.end())
		ref.setLifestyleSkillCategories(JsonConverter::getDataSet<SkillCategoryData>(jsonObj, "lifestyle-categories", manager_));
	if (jsonObj.find("lifestyle-groups") != jsonObj.end())
		ref.setLifestyleSkillGroups(JsonConverter::getDataSet<SkillGroupData>(jsonObj, "lifestyle-groups", manager_));
	if (jsonObj.find("lifestyle-category-skill-choices") != jsonObj.end())
		ref.setLifestyleCategorySkillChoices(JsonConverter::getDataChoiceSet<SkillCategoryData>(jsonObj, "lifestyle-category-skill-choices", manager_));
	if (jsonObj.find("language-choices") != jsonObj.end())
		ref.setLanguageChoices(JsonConverter::getDataChoicePrimitiveMap<LanguageData, int>(jsonObj, "language-choices", manager_));

	return ref;
}

std::map<std::string, int> TrainingPackageSerializer::getQualifiers(const json::object& obj, const std::string& key) const {
	std::map<std::string, int> result;
	auto it = obj.find(key);
	if (it != obj.end() && it->value().is_array()) {
		for (const auto& item : it->value().as_array()) {
			if (item.is_object()) {
				json::object entry_obj = item.as_object();
				std::string qualifier = JsonConverter::getString(entry_obj, "qualifier");
				int reduction = JsonConverter::getInt(entry_obj, "reduction", 0);
				result.emplace(qualifier, reduction);
			}
		}
	}
	return result;
}

void TrainingPackageSerializer::setQualifiers(json::object& obj, const std::string& key, const std::map<std::string, int>& map) const {
	json::array arr;
	std::map<std::string, int> sorted_map(map); // Sort the map by key
	for (const auto& [qualifier, reduction] : sorted_map) {
		json::object entry_obj;
		entry_obj["qualifier"] = qualifier;
		entry_obj["reduction"] = reduction;
		arr.push_back(entry_obj);
	}
	if (arr.size())
		obj[key] = arr;
}

std::vector<std::pair<std::string, int>> TrainingPackageSerializer::getSpecials(const json::object& obj, const std::string& key) const {
	std::vector<std::pair<std::string, int>> result;
	auto it = obj.find(key);
	if (it != obj.end() && it->value().is_array()) {
		for (const auto& item : it->value().as_array()) {
			if (item.is_object()) {
				json::object entry_obj = item.as_object();
				std::string value = JsonConverter::getString(entry_obj, "value");
				int chance = JsonConverter::getInt(entry_obj, "chance", 0);
				result.emplace_back(value, chance);
			}
		}
	}
	return result;
}

void TrainingPackageSerializer::setSpecials(json::object& obj, const std::string& key, const std::vector<std::pair<std::string, int>>& vector) const {
	json::array arr;
	for (const auto& [value, chance] : vector) {
		json::object entry_obj;
		entry_obj["value"] = value;
		entry_obj["chance"] = chance;
		arr.push_back(entry_obj);
	}
	if (arr.size())
		obj[key] = arr;
}

EnumChoice<StatType::Type> TrainingPackageSerializer::getStatGainChoices(const json::object& obj, const std::string& key) const {
	EnumChoice<StatType::Type> result;
	auto it = obj.find(key);
	if (it != obj.end() && it->value().is_object()) {
		json::object choices_obj = it->value().as_object();
		result.setNumChoices(JsonConverter::getInt(choices_obj, "num-choices", 0));
		if (choices_obj.find("options") != choices_obj.end() && choices_obj["options"].is_array()) {
			for (const auto& item : choices_obj["options"].as_array()) {
				if (item.is_string()) {
					std::string stat_str = item.as_string().c_str();
					std::optional<StatType::Type> stat_opt = StatType::fromString(stat_str);
					if (stat_opt) {
						result.addOption(*stat_opt);
					}
				}
			}
		}
	}
	return result;
}

void TrainingPackageSerializer::setStatGainChoices(json::object& obj, const std::string& key, const EnumChoice<StatType::Type>& stat_gain_choices) const {
	json::object choices_obj;
	JsonConverter::setInt(choices_obj, "num-choices", stat_gain_choices.numChoices());

	std::map<std::string, StatType::Type> sorted_options{};
	for (const StatType::Type option : stat_gain_choices.options<StatType::Type>()) {
		sorted_options.emplace(toString(option), option);
	}
	json::array options_arr;
	for (const auto& [option_str, option] : sorted_options) {
		options_arr.emplace_back(json::value(option_str));
	}

	if (options_arr.size())
		choices_obj["options"] = options_arr;
	obj[key] = choices_obj;
}

std::set<CategoryMultiSkillRankChoice> TrainingPackageSerializer::getCategoryMultiSkillRankChoices(const json::object& obj, const std::string& key) const {
	std::set<CategoryMultiSkillRankChoice> result;
	auto it = obj.find(key);
	if (it != obj.end() && it->value().is_array()) {
		for (const auto& item : it->value().as_array()) {
			if (item.is_object()) {
				json::object entry_obj = item.as_object();
				std::string category_id = JsonConverter::getString(entry_obj, "id");
				const SkillCategoryData* category = &manager_.get<SkillCategoryData>(category_id);
				int ranks = JsonConverter::getInt(entry_obj, "value", 0);
				int num_choices = JsonConverter::getInt(entry_obj, "num-choices", 0);
				result.emplace(CategoryMultiSkillRankChoice{category, ranks, num_choices});
			}
		}
	}
	return result;
}

void TrainingPackageSerializer::setCategoryMultiSkillRankChoices(json::object& obj, const std::string& key, const std::set<CategoryMultiSkillRankChoice>& choices) const {
	json::array arr;
	for (const auto& choice : choices) {
		json::object entry_obj;
		entry_obj["id"] = choice.category->id();
		entry_obj["value"] = choice.ranks;
		entry_obj["num-choices"] = choice.num_choices;
		arr.push_back(entry_obj);
	}
	if (arr.size())
		obj[key] = arr;
}

std::vector<GroupMultiSkillRankChoice> TrainingPackageSerializer::getGroupMultiSkillRankChoices(const json::object& obj, const std::string& key) const {
	std::vector<GroupMultiSkillRankChoice> result;
	auto it = obj.find(key);
	if (it != obj.end() && it->value().is_array()) {
		for (const auto& item : it->value().as_array()) {
			if (item.is_object()) {
				json::object entry_obj = item.as_object();
				std::string category_id = JsonConverter::getString(entry_obj, "id");
				const SkillGroupData* group = &manager_.get<SkillGroupData>(category_id);
				int ranks = JsonConverter::getInt(entry_obj, "value", 0);
				int num_choices = JsonConverter::getInt(entry_obj, "num-choices", 0);
				result.emplace_back(GroupMultiSkillRankChoice{group, ranks, num_choices});
			}
		}
	}
	return result;
}

void TrainingPackageSerializer::setGroupMultiSkillRankChoices(json::object& obj, const std::string& key, const std::vector<GroupMultiSkillRankChoice>& choices) const {
	json::array arr;
	for (const auto& choice : choices) {
		json::object entry_obj;
		entry_obj["id"] = choice.group->id();
		entry_obj["value"] = choice.ranks;
		entry_obj["num-choices"] = choice.num_choices;
		arr.push_back(entry_obj);
	}
	if (arr.size())
		obj[key] = arr;
}

std::vector<SkillGroupCategoryAndSkillRankChoice> TrainingPackageSerializer::getSkillGroupCategoryAndSkillRankChoices(const json::object& obj, const std::string& key) const {
	std::vector<SkillGroupCategoryAndSkillRankChoice> result;
	auto it = obj.find(key);
	if (it != obj.end() && it->value().is_array()) {
		for (const auto& item : it->value().as_array()) {
			if (item.is_object()) {
				json::object entry_obj = item.as_object();

				SkillGroupCategoryAndSkillRankChoice choice{};
				choice.group = &manager().get<SkillGroupData>(JsonConverter::getString(entry_obj, "id"));
				choice.ranks = JsonConverter::getInt(entry_obj, "value", 0);
				result.emplace_back(choice);
			}
		}
	}
	return result;
}

void TrainingPackageSerializer::setSkillGroupCategoryAndSkillRankChoices(json::object& obj, const std::string& key, const std::vector<SkillGroupCategoryAndSkillRankChoice>& choices) const {
	json::array arr;
	for (const auto& choice : choices) {
		json::object entry_obj;
		entry_obj["id"] = choice.group->id();
		entry_obj["value"] = choice.ranks;
		arr.push_back(entry_obj);
	}
	if (arr.size())
		obj[key] = arr;
}

std::set<SpellListChoices> TrainingPackageSerializer::getSpellListChoices(const json::object& obj, const std::string& key) const {
	std::set<SpellListChoices> result;
	auto it = obj.find(key);
	if (it != obj.end() && it->value().is_array()) {
		for (const auto& item : it->value().as_array()) {
			if (item.is_object()) {
				json::object entry_obj = item.as_object();
				SpellListChoices choice_struct{};

				std::optional<std::string> category_id = JsonConverter::getOptionalString(entry_obj, "optional-category");
				if (category_id)
					choice_struct.spell_list_category = &manager_.get<SkillCategoryData>(*category_id);

				choice_struct.num_choices = JsonConverter::getInt(entry_obj, "num-choices", 0);
				choice_struct.ranks = JsonConverter::getInt(entry_obj, "value", 0);

				if (entry_obj.find("options") != entry_obj.end() && entry_obj["options"].is_array()) {
					for (const auto& spell_list_item : entry_obj["options"].as_array()) {
						if (spell_list_item.is_string()) {
							std::string spell_list_id = spell_list_item.as_string().c_str();
							choice_struct.spell_lists.emplace(&manager_.get<SpellListData>(spell_list_id));
						}
					}
				}

				result.emplace(choice_struct);
			}
		}
	}
	return result;
}

void TrainingPackageSerializer::setSpellListChoices(json::object& obj, const std::string& key, const std::set<SpellListChoices>& choices) const {
	json::array arr;

	for (const auto& choice : choices) {
		json::object entry_obj;
		if (choice.spell_list_category)
			entry_obj["optional-category"] = choice.spell_list_category.value()->id();
		entry_obj["value"] = choice.ranks;
		entry_obj["num-choices"] = choice.num_choices;

		// Sort the spell lists by ID for consistent ordering in the JSON
		std::map<std::string, const SpellListData*> sorted_spell_lists;
		for (const auto& spell_list : choice.spell_lists) {
			sorted_spell_lists.emplace(spell_list->id(), spell_list);
		}

		json::array spell_lists_arr;
		for (const auto& [spell_list_id, spell_list] : sorted_spell_lists) {
			spell_lists_arr.emplace_back(json::value(spell_list_id));
		}
		if (spell_lists_arr.size())
			entry_obj["options"] = spell_lists_arr;

		arr.push_back(entry_obj);
	}

	if (arr.size())
		obj[key] = arr;
}

std::set<SpellListCategoryChoices> TrainingPackageSerializer::getSpellListCategoryChoices(const json::object& obj, const std::string& key) const {
	std::set<SpellListCategoryChoices> result;
	auto it = obj.find(key);
	if (it != obj.end() && it->value().is_array()) {
		for (const auto& item : it->value().as_array()) {
			if (item.is_object()) {
				json::object entry_obj = item.as_object();
				SpellListCategoryChoices choice_struct{};
				choice_struct.num_choices = JsonConverter::getInt(entry_obj, "num-choices", 0);
				choice_struct.ranks = JsonConverter::getInt(entry_obj, "value", 0);

				if (entry_obj.find("options") != entry_obj.end() && entry_obj["options"].is_array()) {
					for (const auto& spell_list_item : entry_obj["options"].as_array()) {
						if (spell_list_item.is_string()) {
							std::string spell_list_id = spell_list_item.as_string().c_str();
							choice_struct.spell_list_categories.emplace(&manager_.get<SkillCategoryData>(spell_list_id));
						}
					}
				}

				result.emplace(choice_struct);
			}
		}
	}
	return result;
}

void TrainingPackageSerializer::setSpellListCategoryChoices(json::object& obj, const std::string& key, const std::set<SpellListCategoryChoices>& choices) const {
	json::array arr;
	for (const auto& choice : choices) {
		json::object entry_obj;
		entry_obj["value"] = choice.ranks;
		entry_obj["num-choices"] = choice.num_choices;

		// Sort the categories by ID for consistent ordering in the JSON
		std::map<std::string, const SkillCategoryData*> sorted_categories;
		for (const auto& category : choice.spell_list_categories) {
			sorted_categories.emplace(category->id(), category);
		}
		
		json::array categories_arr;
		for (const auto& [category_id, category] : sorted_categories) {
			categories_arr.emplace_back(json::value(category_id));
		}
		
		if (categories_arr.size())
			entry_obj["options"] = categories_arr;
		arr.push_back(entry_obj);
	}
	if (arr.size())
		obj[key] = arr;
}

} // namespace rm::rule::serial