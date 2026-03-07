#pragma once

#include <sstream>
#include <FileSerializer.h>
#include <PersistentTsvSerializer.h>

namespace rm::rule::serial {

/**
 * @class TsvFileSerializer
 * @brief Class for loading persistent objects from a tab separated file (TSV) using a PersistentTsvSerializer to handle the deserialization of individual objects. The class is designed to be used with any type of persistent object as long as a
 * corresponding PersistentTsvSerializer is provided.
 *
 * The copy and move constructors and assignment operators are deleted to prevent slicing and ensure that the class is only used as a base class for specific serializers.
 *
 * @tparam DataType Type of persistent object to load from the TSV file. This must be a type that satisfies the persistent_object concept and for which a PersistentTsvSerializer specialization exists.
 */
template<persistent_object DataType>
class TsvFileSerializer : public FileSerializer {
public:
	/**
	 * @brief Constructor for the TsvFileSerializer class.
	 *
	 * @param serialiser Reference to a PersistentTsvSerializer that will be used to deserialize individual objects to and from the TSV file. This should be a serializer that is compatible with the DataType template parameter.
	 * @param file_path Path to the TSV file that contains the data to load. This file should contain a TSV object with a "root_key" array or a single object that can be deserialized into a DataType object.
	 * a key in the TSV file.
	 */
	TsvFileSerializer(PersistentTsvSerializer<DataType>& serialiser, const std::string& file_path) : serializer_{&serialiser}, file_path_{file_path} {
	}

	/**
	 * @brief Load the data from an input stream in TSV format and add it to the object manager
	 *
	 * @param is Input stream to read from
	 */
	void load(std::istream& is) override;

	/**
	 * @brief Load the data from the TSV file and add it to the object manager
	 *
	 * @param filename Path to the TSV file to read from
	 */
	void load(const std::string& filename) override;

	/**
	 * @brief Load the data from the TSV file and add it to the object manager
	 */
	void load() override;

	/**
	 * @brief Save the data from the object manager to an output stream in TSV format
	 *
	 * @param os Output stream to write to
	 */
	void save(std::ostream& os) const override;

	/**
	 * @brief Save the data from the object manager to a TSV file
	 *
	 * @param filename Path to the TSV file to write to
	 */
	void save(const std::string& filename) const override;

	/**
	 * @brief Save the data from the object manager to a TSV file
	 */
	void save() const override;

private:
	const PersistentTsvSerializer<DataType>* serializer_{};
	std::string file_path_{};
};

//
// Implement the member functions of the TsvFileSerializer class template
// We need to implement these in the header file because it's a class template and we want to avoid linker errors due to missing symbols when the template is instantiated in different translation units.
//
template<persistent_object DataType>
void TsvFileSerializer<DataType>::load() {
	load(file_path_);
}

template<persistent_object DataType>
void TsvFileSerializer<DataType>::load(const std::string& filename) {
	// Open the file and parse the data
	std::ifstream file(filename);
	if (!file.is_open()) {
		throw std::runtime_error("Could not open file: " + file_path_);
	}

	load(file);
	file.close();
}

template<persistent_object DataType>
void TsvFileSerializer<DataType>::load(std::istream& is) {
  serializer_->deserializeObject(is);
}

template<persistent_object DataType>
void TsvFileSerializer<DataType>::save() const {
	save(file_path_);
}

template<persistent_object DataType>
void TsvFileSerializer<DataType>::save(const std::string& filename) const {
	std::ofstream file(filename);
	if (!file.is_open()) {
		throw std::runtime_error("Could not open file for writing: " + filename);
	}
	save(file);
	file.close();
}

template<persistent_object DataType>
void TsvFileSerializer<DataType>::save(std::ostream& os) const {

	for (const auto& obj : serializer_->manager().getAll<DataType>()) {
		os << serializer_->serializeObject(obj);
	}
}

} // namespace rm::rule::serial
