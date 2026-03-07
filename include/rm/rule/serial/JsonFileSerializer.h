#pragma once

#include <sstream>
#include <FileSerializer.h>
#include <PersistentJsonSerializer.h>

namespace rm::rule::serial {

/**
 * @brief Pretty prints a JSON value to an output stream with optional additional indentation.
 *
 * All elements are indented using 4 spaces per level of nesting. If the optional indent parameter is provided, it will be used as the base indentation for the entire JSON output, allowing for additional indentation to be added on top of
 * the default indentation. If the indent parameter is not provided, the JSON output will be indented starting from the leftmost column.
 *
 * @param os The output stream to write the formatted JSON to.
 * @param jv The JSON value to be pretty printed.
 * @param indent Optional pointer to a string used for indentation. If null, default indentation is used.
 */
void pretty_print(std::ostream& os, json::value const& jv, std::string* indent = nullptr);

/**
 * @class JsonFileSerializer
 * @brief Class for loading persistent objects from a JSON file using a PersistentJsonSerializer to handle the deserialization of individual objects. The class is designed to be used with any type of persistent object as long as a
 * corresponding PersistentJsonSerializer is provided.
 *
 * The copy and move constructors and assignment operators are deleted to prevent slicing and ensure that the class is only used as a base class for specific serializers.
 *
 * @tparam DataType Type of persistent object to load from the JSON file. This must be a type that satisfies the persistent_object concept and for which a PersistentJsonSerializer specialization exists.
 */
template<persistent_object DataType>
class JsonFileSerializer : public FileSerializer {
public:
	/**
	 * @brief Constructor for the JsonFileSerializer class.
	 *
	 * @param serialiser Reference to a PersistentJsonSerializer that will be used to deserialize individual objects to and from the JSON file. This should be a serializer that is compatible with the DataType template parameter.
	 * @param root_key The key in the JSON file that contains the array of objects to deserialize. If the JSON file contains a single object instead of an array, this parameter can be set to an empty string or any value that does not match
	 * @param file_path Path to the JSON file that contains the data to load. This file should contain a JSON object with a "root_key" array or a single object that can be deserialized into a DataType object.
	 * a key in the JSON file.
	 */
	JsonFileSerializer(PersistentJsonSerializer<DataType>& serialiser, const std::string& root_key, const std::string& file_path) : serializer_{&serialiser}, root_key_{root_key}, file_path_{file_path} {
	}

	/**
	 * @brief Load the data from the JSON file and add it to the object manager
	 *
	 * @param is Input stream to read from
	 */
	void load(std::istream& is) override;

	/**
	 * @brief Load the data from the JSON file and add it to the object manager
	 *
	 * @param filename Path to the JSON file to read from
	 */
	void load(const std::string& filename) override;

	/**
	 * @brief Load the data from the JSON file and add it to the object manager
	 */
	void load() override;

	/**
	 * @brief Save the data from the object manager to a JSON file
	 *
	 * @param os Output stream to write to
	 */
	void save(std::ostream& os) const override;

	/**
	 * @brief Save the data from the object manager to a JSON file
	 *
	 * @param filename Path to the JSON file to write to
	 */
	void save(const std::string& filename) const override;

	/**
	 * @brief Save the data from the object manager to a JSON file
	 */
	void save() const override;

	/**
	 * @brief Get the JSON representation of the data from the object manager
	 *
	 * @return A JSON value representing the data, with an array of objects under the specified root key.
	 */
	json::value json() const;

	/**
	 * @brief Set whether to pretty print the JSON output when saving
	 *
	 * @param pretty_print If true, the JSON output will be pretty printed with indentation and newlines. If false, the JSON output will be compact with no extra whitespace.
	 */
	void setPrettyPrint(bool pretty_print) {
		pretty_print_ = pretty_print;
	}

private:
	const PersistentJsonSerializer<DataType>* serializer_{};
	std::string file_path_{};
	std::string root_key_{};  // The key in the JSON file that contains the array of objects to deserialize.
	bool pretty_print_{true}; // Whether to pretty print the JSON output when saving
};

//
// Implement the member functions of the JsonFileSerializer class template
// We need to implement these in the header file because it's a class template and we want to avoid linker errors due to missing symbols when the template is instantiated in different translation units.
//
template<persistent_object DataType>
void JsonFileSerializer<DataType>::load() {
	load(file_path_);
}

template<persistent_object DataType>
void JsonFileSerializer<DataType>::load(const std::string& filename) {
	// Open the file and parse the JSON data
	std::ifstream file(filename);
	if (!file.is_open()) {
		throw std::runtime_error("Could not open file: " + file_path_);
	}

	load(file);
	file.close();
}

template<persistent_object DataType>
void JsonFileSerializer<DataType>::load(std::istream& is) {
	// Read the entire stream into a string
	std::stringstream buffer;
	buffer << is.rdbuf();
	std::string json_content = buffer.str();
	// Parse the JSON string using Boost.JSON
	boost::system::error_code ec;
	json::value json_value = json::parse(json_content, ec);
	// Check for parsing errors
	if (ec) {
		throw std::runtime_error("Failed to parse JSON from stream - Error: " + ec.message());
	}
	// Verify it's an object
	if (!json_value.is_object()) {
		throw std::runtime_error("Expected JSON object at root level in stream");
	}
	json::object& root_obj = json_value.as_object();
	// Check if there's a "root_key_" array in the root object
	auto objects_it = root_obj.find(root_key_);
	if (objects_it != root_obj.end() && objects_it->value().is_array()) {
		const json::array& objects_array = objects_it->value().as_array();
		// Deserialize each obect from the array
		for (const auto& object_value : objects_array) {
			if (object_value.is_object()) {
				json::object obj = object_value.as_object();
				serializer_->deserializeObject(obj);
			}
		}
	} else {
		// If no "root_key" array, try to deserialize the root object as a single object
		serializer_->deserializeObject(root_obj);
	}
}

template<persistent_object DataType>
void JsonFileSerializer<DataType>::save() const {
	save(file_path_);
}

template<persistent_object DataType>
void JsonFileSerializer<DataType>::save(const std::string& filename) const {
	std::ofstream file(filename);
	if (!file.is_open()) {
		throw std::runtime_error("Could not open file for writing: " + filename);
	}
	save(file);
	file.close();
}

template<persistent_object DataType>
void JsonFileSerializer<DataType>::save(std::ostream& os) const {
	json::value root_obj = json();

	if (pretty_print_) {
		pretty_print(os, root_obj);
	} else {
		os << json::serialize(root_obj) << "\n";
	}
}

template<persistent_object DataType>
json::value JsonFileSerializer<DataType>::json() const {
	// Create a JSON array to hold the serialized objects
	json::array json_array;
	// Iterate over all objects of type DataType in the manager and serialize them
	for (const auto& obj : serializer_->manager().getAll<DataType>()) {
		json_array.push_back(serializer_->serializeObject(obj));
	}
	// Create a root JSON object and add the array under the specified root key
	json::object root_obj;
	root_obj[root_key_] = json_array;
	return root_obj;
}

void pretty_print(std::ostream& os, json::value const& jv, std::string* indent) {
	std::string indent_;
	if (!indent)
		indent = &indent_;
	switch (jv.kind()) {
	case json::kind::object: {
		os << "{\n";
		indent->append(4, ' ');
		auto const& obj = jv.get_object();
		if (!obj.empty()) {
			auto it = obj.begin();
			for (;;) {
				os << *indent << json::serialize(it->key()) << " : ";
				pretty_print(os, it->value(), indent);
				if (++it == obj.end())
					break;
				os << ",\n";
			}
		}
		os << "\n";
		indent->resize(indent->size() - 4);
		os << *indent << "}";
		break;
	}

	case json::kind::array: {
		os << "[\n";
		indent->append(4, ' ');
		auto const& arr = jv.get_array();
		if (!arr.empty()) {
			auto it = arr.begin();
			for (;;) {
				os << *indent;
				pretty_print(os, *it, indent);
				if (++it == arr.end())
					break;
				os << ",\n";
			}
		}
		os << "\n";
		indent->resize(indent->size() - 4);
		os << *indent << "]";
		break;
	}

	case json::kind::string: {
		os << json::serialize(jv.get_string());
		break;
	}

	case json::kind::uint64:
	case json::kind::int64:
	case json::kind::double_:
		os << jv;
		break;

	case json::kind::bool_:
		if (jv.get_bool())
			os << "true";
		else
			os << "false";
		break;

	case json::kind::null:
		os << "null";
		break;
	}

	if (indent->empty())
		os << "\n";
}

} // namespace rm::rule::serial
