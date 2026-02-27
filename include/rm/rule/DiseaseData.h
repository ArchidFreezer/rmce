#pragma once

#include <string>

#include <GameRuleData.h>
#include <LevelVarianceType.h>
#include <DiseaseTypeData.h>

namespace rm {

	/**
	 * @class DiseaseData
	 * @brief Details regading types of diseases that can be found in the game, including where in the body they affect and the symptoms they cause.
	 */
	class DiseaseData : public GameRuleData {
	public:

		/**
		 * @brief Default constructor is deleted to ensure the base class is initialised
		 */
		DiseaseData() = delete;

		/**
		 * @brief Constructor to initialise base class
		 * @param id Unique identifier for the disease
		 */
		DiseaseData(std::string_view id) : GameRuleData(id) {};

		/**
		 * @brief Set the name of the disease
		 * @param name Disease name
		 */
		void setName(std::string_view name) { name_ = name; }

		/**
		 * @brief Get the name of the disease
		 * @return Disease name as a string reference
		 */
		const std::string& name() const { return name_; }

		/**
		 * @brief Set the average level of the disease
		 * @param average_level Average level of the disease, used for determining the severity of the symptoms caused by the disease.
		 */
		void setAverageLevel(int average_level) { average_level_ = average_level; }

		/**
		 * @brief Get the average level of the disease
		 * @return Average level of the disease, used for determining the severity of the symptoms caused by the disease.
		 */
		int averageLevel() const { return average_level_; }

		/**
		 * @brief Set the type of disease
		 * @param disease_type_data Pointer to the DiseaseTypeData object that represents the type of disease. This is used to determine where in the body the disease affects and the symptoms it causes.
		 */
		void setType(const DiseaseTypeData& disease_type_data) { disease_type_data_ = &disease_type_data; }

		/**
		 * @brief Get the type of disease
		 * @return Pointer to the DiseaseTypeData object that represents the type of disease. This is used to determine where in the body the disease affects and the symptoms it causes.
		 */
		const DiseaseTypeData& type() const {
			if (disease_type_data_ == nullptr) {
				throw std::runtime_error("Disease type data not set for disease " + name());
			}
			return *disease_type_data_;
		}

		/**
		 * @brief Get the name of the disease type
		 * @return Disease type name as a string reference
		 */
		std::string& typeName() {
			if (disease_type_data_ == nullptr) {
				throw std::runtime_error("Disease type data not set for disease " + name());
			}
			return const_cast<std::string&>(disease_type_data_->name());
		}

		/**
		 * @brief Set the type of level variance to determine the actual level of an instance of this disease.
		 * @param level_variance_type The type of level variance to determine the actual level of an instance of this disease.
		 */
		void setLevelVarianceType(LevelVarianceType::Type level_variance_type) { level_variance_type_ = level_variance_type; }

		/**
		 * @brief Get the type of level variance to determine the actual level of an instance of this disease.
		 * @return The type of level variance to determine the actual level of an instance of this disease.
		 */
		LevelVarianceType::Type levelVarianceType() const { return level_variance_type_; }

	private:
		std::string name_{}; /**< The name of the disease type, used for flavour purposes. */
		int average_level_{}; /**< The average level of the disease, used for determining the severity of the symptoms caused by the disease. */
		const DiseaseTypeData* disease_type_data_{}; /**< Pointer to the DiseaseTypeData object that represents the type of disease. This is used to determine where in the body the disease affects and the symptoms it causes. */
		LevelVarianceType::Type level_variance_type_{}; /**< The type of level variance to determine tha actual level of an instance of this disease. */
	};

} // namespace rm