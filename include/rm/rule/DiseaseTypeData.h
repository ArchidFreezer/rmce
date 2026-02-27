#pragma once

#include <map>
#include <string>

#include <DiseasePoisonSeverityType.h>
#include <GameRuleData.h>
#include <NumberRange.h>
#include <DiseaseType.h>

namespace rm {

	/**
	 * @class DiseaseTypeData
	 * @brief Details regading types of diseases that can be found in the game, including where in the body they affect and the symptoms they cause.
	 */
	class DiseaseTypeData : public GameRuleData {
	public:

		/**
		 * @brief Default constructor is deleted to ensure the base class is initialised
		 */
		DiseaseTypeData() = delete;

		/**
		 * @brief Constructor to initialise base class
		 * @param id Unique identifier for the diseasetype
		 */
		DiseaseTypeData(std::string_view id) : GameRuleData(id) {};

		/**
		 * @brief Get the name of the diseasetype
		 * @return DiseaseType name as a string reference
		 */
		const std::string& name() const { return name_; }

		/**
		 * @brief Set the type of disease
		 * @param type DiseaseType::Type to set
		 */
		void setType(DiseaseType::Type type) {
			type_ = type;
			name_ = DiseaseType::toString(type);
		}

		/**
		 * @brief Get the type of disease
		 * @return DiseaseType::Type type of disease
		 */
		DiseaseType::Type type() const { return type_; }

		/**
		 * @brief Set the transmission of the disease
		 * @param transmission String describing how the disease is transmitted, used for flavour purposes.
		 */
		void setTransmission(std::string transmission) { transmission_ = std::move(transmission); }

		/**
		 * @brief Get the transmission of the disease
		 * @return String describing how the disease is transmitted, used for flavour purposes.
		 */
		const std::string& transmission() const { return transmission_; }

		/**
		 * @brief Set the description of the disease
		 * @param description String describing how disease strikes.
		 */
		void setDescription(std::string description) { description_ = std::move(description); }

		/**
		 * @brief Get the description of the disease
		 * @return String describing how disease strikes.
		 */
		const std::string& description() const { return description_; }

		/**
		 * @brief Set the symptoms caused by the disease based on the severity of the symptoms.
		 * @param severity DiseaseTypeData::Severity representing the severity of the symptoms caused by the disease.
		 * @param symptoms String describing the symptoms caused by the disease based on the severity of the symptoms.
		 */
		void addSymptom(DiseasePoisonSeverityType::Type severity, std::string symptoms) { symptomsBySeverity_[severity] = std::move(symptoms); }

		/**
		 * @brief Sets the symptoms for each severity level.
		 * @param symptomsBySeverity A map associating each severity level with a string describing the symptoms caused by the disease based on the severity of the symptoms.
		 */
		void setSymptoms(std::map<DiseasePoisonSeverityType::Type, std::string> symptomsBySeverity) { symptomsBySeverity_ = std::move(symptomsBySeverity); }

		/**
		 * @brief Get the symptoms caused by the disease based on the severity of the symptoms.
		 * @return Map associating each severity level with a string describing the symptoms caused by the disease based on the severity of the symptoms.
		 */
		const std::map<DiseasePoisonSeverityType::Type, std::string>& symptoms() const { return symptomsBySeverity_; }

		/**
		 * @brief Get the symptoms caused by the disease based on the severity of the symptoms.
		 * @param severity DiseasePoisonSeverityType::Type representing the severity of the symptoms caused by the disease.
		 * @return String describing the symptoms caused by the disease based on the severity of the symptoms.
		 */
		std::string symptoms(DiseasePoisonSeverityType::Type severity) const {
			auto it = symptomsBySeverity_.find(severity);
			if (it != symptomsBySeverity_.end()) {
				return it->second;
			} else {
				throw std::runtime_error("No symptoms defined for severity " + std::to_string(static_cast<int>(severity)) + " for disease type " + name());
			}
		}

	private:
		DiseaseType::Type type_{}; /**< The type of disease, used to determine where in the body the disease affects and the symptoms it causes. */
		std::string name_{}; /**< The name of the disease type, used for flavour purposes. */
		std::string transmission_{}; /**< String describing how the disease is transmitted, used for flavour purposes. */
		std::string description_{}; /**< String describing how disease strikes. */
		std::map<DiseasePoisonSeverityType::Type, std::string> symptomsBySeverity_{}; /**< Map of the symptoms caused by the disease based on the severity of the symptoms. */
	};

} // namespace rm