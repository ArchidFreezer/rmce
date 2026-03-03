#include <JsonConverter.h>

namespace rm::rule::serial {

std::string JsonConverter::getString(const json::object& obj, const std::string& key, const std::string& default_value) {
	auto it = obj.find(key);
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
	if (it != obj.end() && it->value().is_int64()) {
		return static_cast<int>(it->value().as_int64());
	}
	return default_value;
}

double JsonConverter::getDouble(const json::object& obj, const std::string& key, double default_value) {
	auto it = obj.find(key);
	if (it != obj.end() && it->value().is_double()) {
		return it->value().as_double();
	}
	return default_value;
}

bool JsonConverter::getBool(const json::object& obj, const std::string& key, bool default_value) {
	auto it = obj.find(key);
	if (it != obj.end() && it->value().is_bool()) {
		return it->value().as_bool();
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

} // namespace rm::rule::serial