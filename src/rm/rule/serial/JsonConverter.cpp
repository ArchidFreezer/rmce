#include <JsonConverter.h>

namespace rm::rule::serial {

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

double JsonConverter::getDouble(const json::object& obj, const std::string& key, double default_value) {
	auto it = obj.find(key);
	if (it != obj.end()) {
		if (it->value().is_string()) {
			return atof(it->value().as_string().c_str());
		} else if (it->value().is_double()) {
			return static_cast<int>(it->value().as_double());
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

std::vector<std::string> JsonConverter::getStringArray(const json::object& obj, const std::string& key) {
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

void JsonConverter::setDouble(json::object& obj, const std::string& key, double value) {
	obj[key] = value;
}

void JsonConverter::setBool(json::object& obj, const std::string& key, bool value) {
	obj[key] = value;
}

void JsonConverter::setStringArray(json::object& obj, const std::string& key, const std::vector<std::string>& values) {
    json::array arr;
    for (const auto& value : values) {
        arr.push_back(json::value(value));
    }
    obj[key] = arr;
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


json::object* JsonConverter::getNestedObject(json::object& obj, const std::string& path) {
	std::vector<std::string> parts;
	std::stringstream ss(path);
	std::string part;

	while (std::getline(ss, part, '/')) {
		if (!part.empty()) {
			parts.push_back(part);
		}
	}

	json::object* current = &obj;
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

json::value* JsonConverter::getNestedValue(json::object& obj, const std::string& path) {
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

	json::object* current = &obj;
	for (size_t i = 0; i < parts.size() - 1; ++i) {
		auto it = current->find(parts[i]);
		if (it == current->end() || !it->value().is_object())
			return nullptr;
		current = &it->value().as_object();
	}

	auto it = current->find(parts.back());
	return it != current->end() ? &it->value() : nullptr;
}

int JsonConverter::getNestedInt(json::object& obj, const std::string& path, int defaultValue) {
	if (auto* val = getNestedValue(obj, path)) {
		if (val->is_string()) {
			return atoi(val->as_string().c_str());
		} else if (val->is_int64()) {
			return static_cast<int>(val->as_int64());
		}
	}
	return defaultValue;
}

std::string JsonConverter::getNestedString(json::object& obj, const std::string& path, const std::string& defaultValue) {
	if (auto* val = getNestedValue(obj, path)) {
		if (val->is_string()) {
			return std::string(val->as_string());
		}
	}
	return defaultValue;
}

std::set<std::string> JsonConverter::getNestedStringArray(json::object& obj, const std::string& path) {
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

json::array JsonConverter::getJsonArray(const json::object& obj, const std::string& key) {
	auto it = obj.find(key);
	if (it != obj.end() && it->value().is_array()) {
		return it->value().as_array();
	}
	return json::array();
}

} // namespace rm::rule::serial