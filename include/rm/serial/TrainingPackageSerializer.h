#pragma once

#include <PersistentJsonSerializer.h>
#include <TrainingPackageData.h>

namespace rm::serial {

using namespace rm::rule;

/**
 * @class TrainingPackageSerializer
 * @brief JSON serializer for TrainingPackageData objects
 */
class TrainingPackageSerializer : public PersistentJsonSerializer<TrainingPackageData> {
public:
	/* We need to bring the base class serializeObject method into scope to allow us to use the overload that takes an ID as well as the one that takes a TrainingPackageData object */
	using PersistentJsonSerializer<TrainingPackageData>::serializeObject;

	/**
	 * @brief Constructor for the TrainingPackageSerializer class.
	 * @param manager Object manager to retrive persistent objects from or add to
	 */
	TrainingPackageSerializer(rm::PersistentObjectManager& manager) : PersistentJsonSerializer<TrainingPackageData>(manager) {
	}

	/**
	 * @brief Serialize a TrainingPackageData object to a JSON value
	 *
	 * @param ref Reference to the TrainingPackageData object to serialize
	 * @return json::value containing the serialized TrainingPackageData object
	 */
	json::value serializeObject(const TrainingPackageData& ref) const override;

	/**
	 * @brief Deserialize a TrainingPackageData object from a JSON value
	 *
	 * @param jsonObj JSON object containing the data to deserialize
	 * @return Reference to the deserialized TrainingPackageData object
	 */
	const TrainingPackageData& deserializeObject(json::object& jsonObj) const override;

private:
	/**
	 * @brief Extracts a map of string keys to integer values from a JSON object array.
	 * @param obj The JSON object containing the array to extract the map from.
	 * @param key The key in the JSON object that contains the array of objects representing the map entries.
	 * @return A map where keys are strings and values are integers extracted from the JSON array.
	 */
	std::map<std::string, int> getQualifiers(const json::object& obj, const std::string& key) const;

	/**
	 * @brief Converts a map of string keys to integer values into a JSON array of objects and stores it in a JSON object under the specified key.
	 * @param obj The JSON object to store the resulting array in.
	 * @param key The key under which to store the array of objects in the JSON object.
	 * @param map A map where keys are strings and values are integers to be converted into JSON objects.
	 */
	void setQualifiers(json::object& obj, const std::string& key, const std::map<std::string, int>& map) const;

	/**
	 * @brief Extracts a vector of string-primitive pairs from a JSON object array.
	 * @param obj The JSON object containing the array to extract the vector from.
	 * @param key The key in the JSON object that contains the array of objects representing the vector entries.
	 * @return A vector of pairs, where each pair contains a string and an integer extracted from the JSON array.
	 */
	std::vector<std::pair<std::string, int>> getSpecials(const json::object& obj, const std::string& key) const;

	/**
	 * @brief Converts a vector of string-primitive pairs into a JSON array of objects and stores it in a JSON object under the specified key.
	 * @param obj The JSON object to store the resulting array in.
	 * @param key The key under which to store the array of objects in the JSON object.
	 * @param vector A vector of pairs, where each pair contains a string and an integer to be converted into JSON objects.
	 */
	void setSpecials(json::object& obj, const std::string& key, const std::vector<std::pair<std::string, int>>& vector) const;

	/**
	 * @brief Extracts an EnumChoice of StatType::Type from a JSON object.
	 * @param obj The JSON object containing the data to extract the EnumChoice from.
	 * @param key The key in the JSON object that contains the object representing the EnumChoice.
	 * @return An EnumChoice of StatType::Type extracted from the JSON object.
	 */
	EnumChoice<StatType::Type> getStatGainChoices(const json::object& obj, const std::string& key) const;

	/**
	 * @brief Converts an EnumChoice of StatType::Type into a JSON object and stores it in a JSON object under the specified key.
	 * @param obj The JSON object to store the resulting object in.
	 * @param key The key under which to store the EnumChoice object in the JSON object.
	 * @param stat_gain_choices An EnumChoice of StatType::Type to be converted into a JSON object.
	 */
	void setStatGainChoices(json::object& obj, const std::string& key, const EnumChoice<StatType::Type>& stat_gain_choices) const;

	/**
	 * @brief Extracts a set of CategoryMultiSkillRankChoice objects from a JSON object array.
	 * @param obj The JSON object containing the array to extract the set from.
	 * @param key The key in the JSON object that contains the array of objects representing the set entries.
	 * @return A set of CategoryMultiSkillRankChoice objects extracted from the JSON array.
	 */
	std::set<CategoryMultiSkillRankChoice> getCategoryMultiSkillRankChoices(const json::object& obj, const std::string& key) const;

	/**
	 * @brief Converts a set of CategoryMultiSkillRankChoice objects into a JSON array of objects and stores it in a JSON object under the specified key.
	 * @param obj The JSON object to store the resulting array in.
	 * @param key The key under which to store the array of objects in the JSON object.
	 * @param choices A set of CategoryMultiSkillRankChoice objects to be converted into JSON objects.
	 */
	void setCategoryMultiSkillRankChoices(json::object& obj, const std::string& key, const std::set<CategoryMultiSkillRankChoice>& choices) const;

	/**
	 * @brief Extracts a vector of GroupMultiSkillRankChoice objects from a JSON object array.
	 * @param obj The JSON object containing the array to extract the vector from.
	 * @param key The key in the JSON object that contains the array of objects representing the vector entries.
	 * @return A vector of GroupMultiSkillRankChoice objects extracted from the JSON array.
	 */
	std::vector<GroupMultiSkillRankChoice> getGroupMultiSkillRankChoices(const json::object& obj, const std::string& key) const;

	/**
	 * @brief Converts a vector of GroupMultiSkillRankChoice objects into a JSON array of objects and stores it in a JSON object under the specified key.
	 * @param obj The JSON object to store the resulting array in.
	 * @param key The key under which to store the array of objects in the JSON object.
	 * @param choices A vector of GroupMultiSkillRankChoice objects to be converted into JSON objects.
	 */
	void setGroupMultiSkillRankChoices(json::object& obj, const std::string& key, const std::vector<GroupMultiSkillRankChoice>& choices) const;

	/**
	 * @brief Extracts a vector of SkillGroupCategoryAndSkillRankChoice objects from a JSON object array.
	 * @param obj The JSON object containing the array to extract the vector from.
	 * @param key The key in the JSON object that contains the array of objects representing the vector entries.
	 * @return A vector of SkillGroupCategoryAndSkillRankChoice objects extracted from the JSON array.
	 */
	std::vector<SkillGroupCategoryAndSkillRankChoice> getSkillGroupCategoryAndSkillRankChoices(const json::object& obj, const std::string& key) const;

	/**
	 * @brief Converts a vector of SkillGroupCategoryAndSkillRankChoice objects into a JSON array of objects and stores it in a JSON object under the specified key.
	 * @param obj The JSON object to store the resulting array in.
	 * @param key The key under which to store the array of objects in the JSON object.
	 * @param choices A vector of SkillGroupCategoryAndSkillRankChoice objects to be converted into JSON objects.
	 */
	void setSkillGroupCategoryAndSkillRankChoices(json::object& obj, const std::string& key, const std::vector<SkillGroupCategoryAndSkillRankChoice>& choices) const;

	/**
	 * @brief Extracts a set of SpellListChoices objects from a JSON object array.
	 * @param obj The JSON object containing the array to extract the set from.
	 * @param key The key in the JSON object that contains the array of objects representing the set entries.
	 * @return A set of SpellListChoices objects extracted from the JSON array.
	 */
	std::set<SpellListChoices> getSpellListChoices(const json::object& obj, const std::string& key) const;

	/**
	 * @brief Converts a set of SpellListChoices objects into a JSON array of objects and stores it in a JSON object under the specified key.
	 * @param obj The JSON object to store the resulting array in.
	 * @param key The key under which to store the array of objects in the JSON object.
	 * @param choices A set of SpellListChoices objects to be converted into JSON objects.
	 */
	void setSpellListChoices(json::object& obj, const std::string& key, const std::set<SpellListChoices>& choices) const;

	/**
	 * @brief Extracts a set of SpellListCategoryChoices objects from a JSON object array.
	 * @param obj The JSON object containing the array to extract the set from.
	 * @param key The key in the JSON object that contains the array of objects representing the set entries.
	 * @return A set of SpellListCategoryChoices objects extracted from the JSON array.
	 */
	std::set<SpellListCategoryChoices> getSpellListCategoryChoices(const json::object& obj, const std::string& key) const;

	/**
	 * @brief Converts a set of SpellListCategoryChoices objects into a JSON array of objects and stores it in a JSON object under the specified key.
	 * @param obj The JSON object to store the resulting array in.
	 * @param key The key under which to store the array of objects in the JSON object.
	 * @param choices A set of SpellListCategoryChoices objects to be converted into JSON objects.
	 */
	void setSpellListCategoryChoices(json::object& obj, const std::string& key, const std::set<SpellListCategoryChoices>& choices) const;

};

} // namespace rm::serial