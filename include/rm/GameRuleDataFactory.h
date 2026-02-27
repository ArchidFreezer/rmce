#pragma once

#include <GameRuleData.h>
#include <GameRuleDataCache.h>
#include <SubcategoriedSkillData.h>
#include <table/AttackTable.h>
#include <table/SpecialAttackTable.h>

using rm::rule::GameRuleDataObject;
using rm::rule::SkillData;
using rm::rule::SubcategoriedSkillData;

namespace rm {
	/**
	 * @class GameRuleDataFactory
	 * @brief Factory class to get GameRuleData objects from their ID
	 *
	 * If the object already exists it will be retrieved from the cache and if not it will be created and moved into the cache
	 * first and then retrieved and returned. In all cases the actual object reference returned will come from the cache to
	 * ensure that the reference is to a persistent object.
	 */
	class GameRuleDataFactory {
	public:

		/**
		 * @brief Deleted default constructor to ensure the cache is initiated
		 */
		GameRuleDataFactory() = delete;

		/**
		 * @brief Constructor to initiate the cache
		 * @param cache GameRuleDataCache cache to move the created objects into
		 */
		GameRuleDataFactory(GameRuleDataCache& cache) : cache_{ cache } {}

		/**
	 * @brief Populates a set with all the rule data ids for a specific rule type
	 *
	 * The parameter is first erased and then populated with the key data so following the call it will only contain the ids
	 *
	 * @tparam T Class of the data object to be retrieved
	 *           Must be derived from GameRuleData
	 * @param keys Set of strings to populate with the ids of the data objects
	 */
		template <GameRuleDataObject T>
		void keys(std::set<std::string>& keys) {
			return cache_.keys<T>(keys);
		}


		/**
		 * @brief Get standard GameRuleData objects that may be created with an ID only
		 * @tparam T type of GameRuleData object to create
		 * @param id Unique ID of the object
		 * @return GameRuleData object from the cache of type @a T
		 */
		template<GameRuleDataObject T>
		T& get(std::string& id) {
			if (cache_.exists<T>(id)) return cache_.get<T>(id);
			cache_.add<T>(std::move(std::make_unique<T>(id)), id);
			return cache_.get<T>(id);
		}

		/**
		 * @brief Get SubcategoriedSkillData objects
		 *
		 * SubcategoriedSkillData cannot be created with ID only as the ID is derived from optional arguments. In order to
		 * allow these to be safely created and cached a custom factory method has been created
		 * @param skill_data SkillData that is being wrapped
		 * @param subcategory optional subcategory of @a skill_data
		 * @return
		 */
		SubcategoriedSkillData& subcategoriedSkillData(const SkillData& skill_data, std::optional<std::string_view> subcategory = std::nullopt) {
			std::string id{ skill_data.id() + (subcategory ? "_" + std::string(subcategory.value()) : "") };
			if (cache_.exists<SubcategoriedSkillData>(id)) return cache_.get<SubcategoriedSkillData>(id);
			if (subcategory) cache_.add<SubcategoriedSkillData>(std::move(std::make_unique<SubcategoriedSkillData>(skill_data, subcategory)), id);
			else cache_.add<SubcategoriedSkillData>(std::move(std::make_unique<SubcategoriedSkillData>(skill_data)), id);
			return cache_.get<SubcategoriedSkillData>(id);
		}

		/**
		 * @brief Get SubcategoriedSkillData objects
		 *
		 * SubcategoriedSkillData cannot be created with ID only as the ID is derived from optional arguments. In order to
		 * allow these to be safely created and cached a custom factory method has been created
		 * @param skill_id Unique identifier of the skill that is being wrapped
		 * @param subcategory optional subcategory of @a skill_data
		 * @return
		 */
		SubcategoriedSkillData& subcategoriedSkillData(std::string& skill_id, std::optional<std::string_view> subcategory = std::nullopt) {
			SkillData& skill = get<SkillData>(skill_id);
			return (subcategory ? subcategoriedSkillData(skill, subcategory) : subcategoriedSkillData(skill));
		}

	private:
		GameRuleDataCache& cache_; /**< Reference to a cache object to store the data objects */

	};

} // namespace rm