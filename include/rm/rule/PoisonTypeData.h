#pragma once

#include <map>
#include <string>

#include <DiseasePoisonSeverityType.h>
#include <GameRuleData.h>
#include <NumberRange.h>
#include <PoisonType.h>

/**
 * @class PoisonTypeData
 * @brief Details regading types of poisons that can be found in the game, including where in the body they affect and the symptoms they cause.
 */
class PoisonTypeData : public GameRuleData {
public:

	/**
	 * @brief Default constructor is deleted to ensure the base class is initialised
	 */
	PoisonTypeData() = delete;

	/**
	 * @brief Constructor to initialise base class
	 * @param id Unique identifier for the poisontype
	 */
	PoisonTypeData(std::string_view id) : GameRuleData(id) {};

	/**
	 * @brief Get the name of the poisontype
	 * @return PoisonType name as a string reference
	 */
	const std::string& name() const { return name_; }

	/**
	 * @brief Set the type of poison
	 * @param type PoisonType::Type to set
	 */
	void setType(PoisonType::Type type) { 
		type_ = type;
		name_ = PoisonType::toString(type);
	}

	/**
	 * @brief Get the type of poison
	 * @return PoisonType::Type type of poison
	 */
	PoisonType::Type type() const { return type_; }

	/**
	 * @brief Set the number of rounds before the symptoms of the poison start to take effect, based on the severity of the symptoms.
	 * @param severity DiseasePoisonSeverityType::Type representing the severity of the symptoms caused by the poison.
	 * @param effectOnset NumberRange<int> representing the number of rounds before the symptoms of the poison start to take effect, based on the severity of the symptoms.
	 */
	void addEffectOnset(DiseasePoisonSeverityType::Type severity, const NumberRange<int>* effectOnset) { effectOnsetBySeverity_[severity] = effectOnset; }

	/**
	 * @brief Sets the effect onset ranges for each severity level.
	 * @param effectOnsetBySeverity A map associating each severity level with a range of effect onset values.
	 */
	void setEffectOnsets(std::map<DiseasePoisonSeverityType::Type, const NumberRange<int>*> effectOnsetBySeverity) { effectOnsetBySeverity_ = std::move(effectOnsetBySeverity); }
	
	/**
	 * @brief Get the number of rounds before the symptoms of the poison start to take effect, based on the severity of the symptoms.
	 * @return Map associating each severity level with a range of effect onset values.
	 */
	const std::map<DiseasePoisonSeverityType::Type, const NumberRange<int>*>& effectOnsets() const { return effectOnsetBySeverity_; }

	/**
	 * @brief How long, in rounds before the symptoms of the poison start to take effect, based on the severity of the symptoms.
	 * @param severity DiseasePoisonSeverityType::Type representing the severity of the symptoms caused by the poison.
	 * @return NumberRange<int> representing the number of rounds before the symptoms of the poison start to take effect, based on the severity of the symptoms.
	 */
	const NumberRange<int> effectOnset(DiseasePoisonSeverityType::Type severity) const {
		auto it = effectOnsetBySeverity_.find(severity);
		if (it != effectOnsetBySeverity_.end()) {
			return *(it->second);
		} else {
			throw std::runtime_error("No effect onset defined for severity " + std::to_string(static_cast<int>(severity)) + " for poison type " + name());
		}
	}

	/**
	 * @brief Set the symptoms caused by the poison based on the severity of the symptoms.
	 * @param severity PoisonTypeData::Severity representing the severity of the symptoms caused by the poison.
	 * @param symptoms String describing the symptoms caused by the poison based on the severity of the symptoms.
	 */
	void addSymptom(DiseasePoisonSeverityType::Type severity, std::string symptoms) { symptomsBySeverity_[severity] = std::move(symptoms); }

	/**
	 * @brief Sets the symptoms for each severity level.
	 * @param symptomsBySeverity A map associating each severity level with a string describing the symptoms caused by the poison based on the severity of the symptoms.
	 */
	void setSymptoms(std::map<DiseasePoisonSeverityType::Type, std::string> symptomsBySeverity) { symptomsBySeverity_ = std::move(symptomsBySeverity); }

	/**
	 * @brief Get the symptoms caused by the poison based on the severity of the symptoms.
	 * @return Map associating each severity level with a string describing the symptoms caused by the poison based on the severity of the symptoms.
	 */
	const std::map<DiseasePoisonSeverityType::Type, std::string>& symptoms() const { return symptomsBySeverity_; }

	/**
	 * @brief Get the symptoms caused by the poison based on the severity of the symptoms.
	 * @param severity DiseasePoisonSeverityType::Type representing the severity of the symptoms caused by the poison.
	 * @return String describing the symptoms caused by the poison based on the severity of the symptoms.
	 */
	std::string symptoms(DiseasePoisonSeverityType::Type severity) const {
		auto it = symptomsBySeverity_.find(severity);
		if (it != symptomsBySeverity_.end()) {
			return it->second;
		} else {
			throw std::runtime_error("No symptoms defined for severity " + std::to_string(static_cast<int>(severity)) + " for poison type " + name());
		}
	}

	/**
	 * @brief Set the areas of the body that are affected by the poison if this is not defined.
	 * 
	 * The area affected by a poison often varies. Sometimes the victim’s actions or the attack dictate the point of entry and the area first affected
	 * (e.g., a maneuver of critical strike indicates that the snake injects the nerve poison in the victim’s leg), but occasionally there will be no 
	 * indication of the initially affected spot.
	 * 
	 * @param areasAffected String describing the areas of the body that are affected by the poison if not specified.
	 */
	void setAreasAffected(std::string areasAffected) { areasAffected_ = std::move(areasAffected); }

	/**
	 * @brief Get the areas of the body that are affected by the poison if this is not defined.
	 * 
	 * The area affected by a poison often varies. Sometimes the victim’s actions or the attack dictate the point of entry and the area first affected
	 * (e.g., a maneuver of critical strike indicates that the snake injects the nerve poison in the victim’s leg), but occasionally there will be no 
	 * indication of the initially affected spot.
	 * 
	 * @return String describing the areas of the body that are affected by the poison if not specified.
	 */
	const std::string& areasAffected() const { return areasAffected_; }
	
private:
	PoisonType::Type type_{}; /**< The type of poison, used to determine where in the body the poison affects and the symptoms it causes. */
	std::string name_{}; /**< The name of the poison type, used for flavour purposes. */
	std::map<DiseasePoisonSeverityType::Type, const NumberRange<int>*> effectOnsetBySeverity_{}; /**< Map of the number of rounds before the symptoms of the poison start to take effect based on the severity of the symptoms. */
	std::map<DiseasePoisonSeverityType::Type, std::string> symptomsBySeverity_{}; /**< Map of the symptoms caused by the poison based on the severity of the symptoms. */
	std::string areasAffected_{}; /**< String describing the areas of the body that are affected by the poison if not specified. */
};
