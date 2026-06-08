#pragma once

#include <string>

#include <GameRuleData.h>
#include <LevelVarianceType.h>
#include <PoisonTypeData.h>

using namespace rm::rule::enums;

namespace rm::rule {

/**
 * @class PoisonData
 * @brief Details regading types of poisons that can be found in the game, including where in the body they affect and the symptoms they cause.
 */
class PoisonData : public GameRuleData {
public:
	/**
	 * @brief Default constructor is deleted to ensure the base class is initialised
	 */
	PoisonData() = delete;

	/**
	 * @brief Constructor to initialise base class
	 * @param id Unique identifier for the poison
	 */
	PoisonData(std::string_view id) : GameRuleData(id) {};

	inline static std::string prefix_{"POISON"}; /**< Prefix for the ID of the data objects */

	/**
	 * @brief Get the prefix for the ID of the data objects
	 * @return Prefix for the ID of the data objects as a string view
	 */
	std::string_view prefix() const override {
		return PoisonData::prefix_;
	}

	/**
	 * @brief Set the name of the poison
	 * @param name Poison name
	 */
	void setName(std::string_view name) {
		name_ = name;
	}

	/**
	 * @brief Get the name of the poison
	 * @return Poison name as a string reference
	 */
	const std::string& name() const {
		return name_;
	}

	/**
	 * @brief Set the average level of the poison
	 * @param average_level Average level of the poison, used for determining the severity of the symptoms caused by the poison.
	 */
	void setAverageLevel(int average_level) {
		average_level_ = average_level;
	}

	/**
	 * @brief Get the average level of the poison
	 * @return Average level of the poison, used for determining the severity of the symptoms caused by the poison.
	 */
	int averageLevel() const {
		return average_level_;
	}

	/**
	 * @brief Set the type of poison
	 * @param poison_type_data Pointer to the PoisonTypeData object that represents the type of poison. This is used to determine where in the body the poison affects and the symptoms it causes.
	 */
	void setType(const PoisonTypeData& poison_type_data) {
		poison_type_data_ = &poison_type_data;
	}

	/**
	 * @brief Get the type of poison
	 * @return Pointer to the PoisonTypeData object that represents the type of poison. This is used to determine where in the body the poison affects and the symptoms it causes.
	 */
	const PoisonTypeData& type() const {
		if (poison_type_data_ == nullptr) {
			throw std::runtime_error("Poison type data not set for poison " + name());
		}
		return *poison_type_data_;
	}

	/**
	 * @brief Get the name of the poison type
	 * @return Poison type name as a string reference
	 */
	std::string& typeName() {
		if (poison_type_data_ == nullptr) {
			throw std::runtime_error("Poison type data not set for poison " + name());
		}
		return const_cast<std::string&>(poison_type_data_->name());
	}

	/**
	 * @brief Set the type of level variance to determine the actual level of an instance of this poison.
	 * @param level_variance_type The type of level variance to determine the actual level of an instance of this poison.
	 */
	void setLevelVarianceType(LevelVarianceType::Type level_variance_type) {
		level_variance_type_ = level_variance_type;
	}

	/**
	 * @brief Get the type of level variance to determine the actual level of an instance of this poison.
	 * @return The type of level variance to determine the actual level of an instance of this poison.
	 */
	LevelVarianceType::Type levelVarianceType() const {
		return level_variance_type_;
	}

	/**
	 * @brief Set any additional notes about the poison
	 * @param notes Additional notes about the poison
	 */
	void setNotes(std::string_view notes) {
		notes_ = notes;
	}

	/**
	 * @brief Get any additional notes about the poison
	 * @return Additional notes about the poison as a string reference
	 */
	const std::string& notes() const {
		return notes_;
	}

	/**
	 * @brief Set the maximum severity of the symptoms caused by the poison, used for determining the number of rounds before the symptoms of the poison start to take effect.
	 * @param max_severity The maximum severity of the symptoms caused by the poison, used for determining the number of rounds before the symptoms of the poison start to take effect.
	 */
	void setMaxSeverity(DiseasePoisonSeverityType::Type max_severity) {
		max_severity_ = max_severity;
	}

	/**
	 * @brief Get the maximum severity of the symptoms caused by the poison, used for determining the number of rounds before the symptoms of the poison start to take effect.
	 * @return The maximum severity of the symptoms caused by the poison, used for determining the number of rounds before the symptoms of the poison start to take effect.
	 */
	DiseasePoisonSeverityType::Type maxSeverity() const {
		return max_severity_;
	}

private:
	std::string name_{};                             /**< The name of the poison type, used for flavour purposes. */
	int average_level_{};                            /**< The average level of the poison, used for determining the severity of the symptoms caused by the poison. */
	const PoisonTypeData* poison_type_data_{};       /**< Pointer to the PoisonTypeData object that represents the type of poison. This is used to determine where in the body the poison affects and the symptoms it causes. */
	LevelVarianceType::Type level_variance_type_{};  /**< The type of level variance to determine tha actual level of an instance of this poison. */
	std::string notes_{};                            /**< Any additional notes about the poison. */
	DiseasePoisonSeverityType::Type max_severity_{}; /**< The maximum severity of the symptoms caused by the poison, used for determining the number of rounds before the symptoms of the poison start to take effect. */
};

} // namespace rm::rule