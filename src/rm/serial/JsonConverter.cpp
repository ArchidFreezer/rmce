#include <JsonConverter.h>

namespace rm::serial {

std::string JsonConverter::getString(const json::object& obj, const std::string& key, const std::string& default_value) {
	auto it = obj.find(key);
	if (it != obj.end()) {
		int error_code = 0;
	}
	if (it != obj.end() && it->value().is_string()) {
		return std::string(it->value().as_string().c_str());
	}
	return default_value;
}

std::optional<std::string> JsonConverter::getOptionalString(const json::object& obj, const std::string& key) {
	auto it = obj.find(key);
	if (it != obj.end() && it->value().is_string()) {
		return std::string(it->value().as_string().c_str());
	}
	return std::nullopt;
}

int JsonConverter::getInt(const json::object& obj, const std::string& key, int default_value) {
	auto it = obj.find(key);
	if (it != obj.end()) {
		if (it->value().is_string()) {
			return atoi(it->value().as_string().c_str());
		} else if (it->value().is_int64()) {
			return static_cast<int>(it->value().as_int64());
		}
	}
	return default_value;
}

float JsonConverter::getFloat(const json::object& obj, const std::string& key, float default_value) {
	auto it = obj.find(key);
	if (it != obj.end()) {
		if (it->value().is_string()) {
			return static_cast<float>(atof(it->value().as_string().c_str()));
		} else if (it->value().is_double()) {
			return static_cast<float>(it->value().as_double());
		} else if (it->value().is_int64()) {
			return static_cast<float>(it->value().as_int64());
		}
	}
	return default_value;
}

double JsonConverter::getDouble(const json::object& obj, const std::string& key, double default_value) {
	auto it = obj.find(key);
	if (it != obj.end()) {
		if (it->value().is_string()) {
			return atof(it->value().as_string().c_str());
		} else if (it->value().is_double()) {
			return static_cast<double>(it->value().as_double());
		} else if (it->value().is_int64()) {
			return static_cast<double>(it->value().as_int64());
		}
	}
	return default_value;
}

bool JsonConverter::getBool(const json::object& obj, const std::string& key, bool default_value) {
	auto it = obj.find(key);
	if (it != obj.end()) {
		if (it->value().is_string()) {
			std::string val = std::string(it->value().as_string().c_str());
			std::transform(val.begin(), val.end(), val.begin(), ::tolower);
			if (val == "true" || val == "1") {
				return true;
			} else if (val == "false" || val == "0") {
				return false;
			}
		} else if (it->value().is_bool()) {
			return it->value().as_bool();
		}
	}
	return default_value;
}

std::vector<std::string> JsonConverter::getStringVector(const json::object& obj, const std::string& key) {
	std::vector<std::string> result;
	auto it = obj.find(key);
	if (it != obj.end() && it->value().is_array()) {
		for (const auto& item : it->value().as_array()) {
			if (item.is_string()) {
				result.push_back(std::string(item.as_string().c_str()));
			}
		}
	}
	return result;
}

std::set<std::string> JsonConverter::getStringSet(const json::object& obj, const std::string& key) {
	std::set<std::string> result;
	auto it = obj.find(key);
	if (it != obj.end() && it->value().is_array()) {
		for (const auto& item : it->value().as_array()) {
			if (item.is_string()) {
				result.emplace(std::string(item.as_string().c_str()));
			}
		}
	}
	return result;
}

std::vector<int> JsonConverter::getIntArray(const json::object& obj, const std::string& key) {
	std::vector<int> result;
	auto it = obj.find(key);
	if (it != obj.end() && it->value().is_array()) {
		for (const auto& item : it->value().as_array()) {
			if (item.is_int64()) {
				result.push_back(static_cast<int>(item.as_int64()));
			}
		}
	}
	return result;
}

std::map<std::string, std::string> JsonConverter::getStringMap(const json::object& obj, const std::string& key) {
	std::map<std::string, std::string> result;
	auto it = obj.find(key);
	if (it != obj.end() && it->value().is_object()) {
		for (const auto& pair : it->value().as_object()) {
			if (pair.value().is_string()) {
				result[std::string(pair.key())] = std::string(pair.value().as_string().c_str());
			}
		}
	}
	return result;
}

void JsonConverter::setString(json::object& obj, const std::string& key, const std::string& value) {
	obj[key] = value;
}

void JsonConverter::setInt(json::object& obj, const std::string& key, int value) {
	obj[key] = value;
}

void JsonConverter::setFloat(json::object& obj, const std::string& key, float value) {
	obj[key] = value;
}

void JsonConverter::setDouble(json::object& obj, const std::string& key, double value) {
	obj[key] = value;
}

void JsonConverter::setBool(json::object& obj, const std::string& key, bool value) {
	obj[key] = value;
}

void JsonConverter::setIntArray(json::object& obj, const std::string& key, const std::vector<int>& values) {
	json::array arr;
	for (int value : values) {
		arr.push_back(json::value(value));
	}
	obj[key] = arr;
}

void JsonConverter::setOptionalString(json::object& obj, const std::string& key, const std::optional<std::string>& value) {
	if (value.has_value()) {
		obj[key] = value.value();
	}
}

const json::object* JsonConverter::getNestedObject(const json::object& obj, const std::string& path) {
	std::vector<std::string> parts;
	std::stringstream ss(path);
	std::string part;

	while (std::getline(ss, part, '/')) {
		if (!part.empty()) {
			parts.push_back(part);
		}
	}

	const json::object* current = &obj;
	for (size_t i = 0; i < parts.size(); ++i) {
		auto it = current->find(parts[i]);
		if (it == current->end())
			return nullptr;

		if (i == parts.size() - 1) {
			return it->value().is_object() ? &it->value().as_object() : nullptr;
		}

		if (!it->value().is_object())
			return nullptr;
		current = &it->value().as_object();
	}

	return current;
}

const json::value* JsonConverter::getNestedValue(const json::object& obj, const std::string& path) {
	std::vector<std::string> parts;
	std::stringstream ss(path);
	std::string part;

	while (std::getline(ss, part, '/')) {
		if (!part.empty()) {
			parts.push_back(part);
		}
	}

	if (parts.empty())
		return nullptr;

	const json::object* current = &obj;
	for (size_t i = 0; i < parts.size() - 1; ++i) {
		auto it = current->find(parts[i]);
		if (it == current->end() || !it->value().is_object())
			return nullptr;
		current = &it->value().as_object();
	}

	auto it = current->find(parts.back());
	return it != current->end() ? &it->value() : nullptr;
}

int JsonConverter::getNestedInt(const json::object& obj, const std::string& path, int defaultValue) {
	if (auto* val = getNestedValue(obj, path)) {
		if (val->is_string()) {
			return atoi(val->as_string().c_str());
		} else if (val->is_int64()) {
			return static_cast<int>(val->as_int64());
		}
	}
	return defaultValue;
}

float JsonConverter::getNestedFloat(const json::object& obj, const std::string& path, float defaultValue) {
	if (auto* val = getNestedValue(obj, path)) {
		if (val->is_string()) {
			return static_cast<float>(atof(val->as_string().c_str()));
		} else if (val->is_double()) {
			return static_cast<float>(val->as_double());
		}
	}
	return defaultValue;
}

double JsonConverter::getNestedDouble(const json::object& obj, const std::string& path, double defaultValue) {
	if (auto* val = getNestedValue(obj, path)) {
		if (val->is_string()) {
			return atof(val->as_string().c_str());
		} else if (val->is_double()) {
			return static_cast<double>(val->as_double());
		}
	}
	return defaultValue;
}

std::string JsonConverter::getNestedString(const json::object& obj, const std::string& path, const std::string& defaultValue) {
	if (auto* val = getNestedValue(obj, path)) {
		if (val->is_string()) {
			return std::string(val->as_string());
		}
	}
	return defaultValue;
}

std::set<std::string> JsonConverter::getNestedStringArray(const json::object& obj, const std::string& path) {
	std::set<std::string> result;
	if (auto* val = getNestedValue(obj, path)) {
		if (val->is_array()) {
			for (const auto& item : val->as_array()) {
				if (item.is_string()) {
					result.emplace(std::string(item.as_string().c_str()));
				}
			}
		}
	}
	return result;
}

const json::array JsonConverter::getJsonArray(const json::object& obj, const std::string& key) {
	auto it = obj.find(key);
	if (it != obj.end() && it->value().is_array()) {
		return it->value().as_array();
	}
	return json::array();
}

std::set<const SubcategoriedSkillData*> JsonConverter::getSkillSet(const json::object& obj, const std::string& key, rm::PersistentObjectManager& manager) {
	std::set<const SubcategoriedSkillData*> skill_set;
	json::array skillArray = getJsonArray(obj, key);
	for (const auto& skill_val : skillArray) {
		if (!skill_val.is_object())
			continue;
		skill_set.emplace(getSkill(skill_val.as_object(), manager));
	}
	return skill_set;
}

std::map<std::string, const rm::game::character::LanguageRanks> JsonConverter::getLanguageAbilityMap(const json::object& obj, const std::string& key, rm::PersistentObjectManager& manager) {
	std::map<std::string, const rm::game::character::LanguageRanks> map;
	json::array abilityArray = getJsonArray(obj, key);
	for (const auto& ability_val : abilityArray) {
		if (!ability_val.is_object())
			continue;
		json::object abilityObj = ability_val.as_object();
		std::string language_id = getString(abilityObj, "language");
		rm::game::character::LanguageRanks ability(manager.get<LanguageData>(language_id));
		if (abilityObj.find("spoken") != abilityObj.end()) {
			int spoken = getInt(abilityObj, "spoken");
			ability.updateSpokenRanks(spoken);
		}
		if (abilityObj.find("written") != abilityObj.end()) {
			int written = getInt(abilityObj, "written");
			ability.updateWrittenRanks(written);
		}
		if (abilityObj.find("somatic") != abilityObj.end()) {
			int somatic = getInt(abilityObj, "somatic");
			ability.updateSomaticRanks(somatic);
		}
		map.emplace(language_id, ability);
	}
	return map;
}

void JsonConverter::setLanguageAbilities(json::object& obj, const std::string& key, const std::map<std::string, const rm::game::character::LanguageRanks>& language_map) {
	json::array arr;
	for (const auto& [language_id, ability] : language_map) {
		json::object abilityObj;
		abilityObj["language"] = language_id;
		if (ability.somaticRanks() > 0) {
			abilityObj["somatic"] = ability.somaticRanks();
		}
		if (ability.spokenRanks() > 0) {
			abilityObj["spoken"] = ability.spokenRanks();
		}
		if (ability.writtenRanks() > 0) {
			abilityObj["written"] = ability.writtenRanks();
		}
		arr.push_back(abilityObj);
	}
	if (!arr.empty())
		obj[key] = arr;
}

const SubcategoriedSkillData* JsonConverter::getSkill(const json::object& obj, rm::PersistentObjectManager& manager) {
	std::string id = getString(obj, "id");
	std::optional<std::string> subcategory = getOptionalString(obj, "subcategory");
	if (subcategory)
		return &manager.subcategoriedSkillData(id, subcategory.value());
	else
		return &manager.subcategoriedSkillData(id);
}

const json::object JsonConverter::setSkill(const SubcategoriedSkillData& skillData) {
	json::object obj;
	setString(obj, "id", skillData.skillData().id());
	if (skillData.subcategory()) {
		setString(obj, "subcategory", skillData.subcategory().value());
	}
	return obj;
}

const json::object* JsonConverter::getObject(const json::object& obj, std::string_view key) {
	json::object ::const_iterator it = obj.find(key);
	if (it != obj.end() && it->value().is_object()) {
		return &it->value().as_object();
	}
	// If we get here then the key was not found or the value was not an object, so we return nullptr to indicate that the object could not be retrieved.
	return nullptr;
}

rm::game::character::CharacterTraits JsonConverter::getCharacterTraits(const json::object& obj, std::string_view key) {
	const json::object* trait_obj = getObject(obj, key);
	rm::game::character::CharacterTraits traits;
	if (!trait_obj) {
		return traits; // Return default traits if the object is not found
	}
	traits.caster_ = getInt(*trait_obj, "caster");
	traits.combat_ = getInt(*trait_obj, "combat");
	traits.information_ = getInt(*trait_obj, "information");
	traits.stealth_ = getInt(*trait_obj, "stealth");
	traits.support_ = getInt(*trait_obj, "support");
	traits.utility_ = getInt(*trait_obj, "utility");
	return traits;
}

void JsonConverter::setCharacterTraits(json::object& obj, std::string_view key, const rm::game::character::CharacterTraits& traits) {
	json::object trait_obj;
	trait_obj["caster"] = traits.caster_;
	trait_obj["combat"] = traits.combat_;
	trait_obj["information"] = traits.information_;
	trait_obj["stealth"] = traits.stealth_;
	trait_obj["support"] = traits.support_;
	trait_obj["utility"] = traits.utility_;
	obj[key] = trait_obj;
}

rm::game::Location JsonConverter::getLocation(const json::object& obj, std::string_view key, rm::PersistentObjectManager& manager) {
	const json::object* location_obj = getObject(obj, key);
	rm::game::Location location{};
	std::set<std::string> features_str = JsonConverter::getStringSet(*location_obj, "features");
	for (const auto& feature_str : features_str) {
		EnvironmentType::Feature feature{};
		fromString(feature_str, feature);
		location.addFeature(feature);
	}
	std::set<std::string> terrains_str = JsonConverter::getStringSet(*location_obj, "terrains");
	for (const auto& terrain_str : terrains_str) {
		EnvironmentType::Terrain terrain{};
		fromString(terrain_str, terrain);
		location.addTerrain(terrain);
	}
	std::set<std::string> vegetation_str = JsonConverter::getStringSet(*location_obj, "	vegetation");
	for (const auto& vegetation_str : vegetation_str) {
		EnvironmentType::Vegetation vegetation{};
		fromString(vegetation_str, vegetation);
		location.addVegetation(vegetation);
	}
	std::set<std::string> water_str = JsonConverter::getStringSet(*location_obj, "waterSources");
	for (const auto& water_str : water_str) {
		EnvironmentType::Water water{};
		fromString(water_str, water);
		location.addWater(water);
	}
	std::set<std::string> climates_str = JsonConverter::getStringSet(*location_obj, "climates");
	for (const auto& climate_str : climates_str) {
		location.addClimate(&manager.get<ClimateData>(climate_str));
	}
	return location;
}

void JsonConverter::nestLocation(json::object& obj, const std::string key, const rm::game::Location& location) {
	JsonConverter::NestedBuilder location_builder = JsonConverter::createNested(obj).beginObject(key);
	if (location.features().size()) {
		std::set<std::string> features_str{};
		for (const auto& feature : location.features()) {
			features_str.emplace(EnvironmentType::toString(feature));
		}
		if (features_str.size())
			location_builder.setStringArray("features", features_str);
	}
	if (location.terrains().size()) {
		std::set<std::string> terrains_str{};
		for (const auto& terrain : location.terrains()) {
			terrains_str.emplace(EnvironmentType::toString(terrain));
		}
		if (terrains_str.size())
			location_builder.setStringArray("terrains", terrains_str);
	}
	if (location.vegetation().size()) {
		std::set<std::string> vegetation_str{};
		for (const auto& vegetation : location.vegetation()) {
			vegetation_str.emplace(EnvironmentType::toString(vegetation));
		}
		if (vegetation_str.size())
			location_builder.setStringArray("vegetation", vegetation_str);
	}
	if (location.water().size()) {
		std::set<std::string> water_str{};
		for (const auto& water : location.water()) {
			water_str.emplace(EnvironmentType::toString(water));
		}
		if (water_str.size())
			location_builder.setStringArray("waterSources", water_str);
	}
	if (location.climates().size()) {
		std::set<std::string> climates_str{};
		for (const auto& climate : location.climates()) {
			climates_str.emplace(climate->id());
		}
		if (climates_str.size())
			location_builder.setStringArray("climates", climates_str);
	}
	location_builder.endObject();
}

} // namespace rm::serial