#pragma once
#include <mutex>
#include <map>
#include <table/TableRowGameRuleDataMatcher.h>

namespace rm {

	/**
	 * @brief Thread safe factory class for GameRuleData objects that can be used for matching dice rolls
	 *
	 * The factory stores all matchers that have been created previously and only creates a new matcher if one does not exist.
	 *
	 * The getter function returns a const* so there is no risk of a client altering a cached object.
	 */
	class GameRuleDataMatcherFactory {
	public:

		/**
		 * @brief Get a matcher for the given GameRuleData object
		 *
		 * If a matcher for the given GameRuleData object already exists in the cache then it is returned, otherwise a new one is created and added to the cache before being returned.
		 *
		 * @param game_data GameRuleData object to get a matcher for
		 * @return const TableRowGameRuleDataMatcher* Matcher for the given GameRuleData object
		 */
		const rule::TableRowGameRuleDataMatcher* matcher(const rule::GameRuleData* game_data);

	private:
		inline static std::map<std::string, rule::TableRowGameRuleDataMatcher> matchers_{};
		std::mutex mutex_{};
	};

} // namespace rm