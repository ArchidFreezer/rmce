#pragma once
#include <mutex>
#include <map>
#include <table/PersistentMatcher.h>

namespace rm::rule::table {

	/**
	 * @brief Thread safe factory class for Persistent objects that can be used for matching dice rolls
	 *
	 * The factory stores all matchers that have been created previously and only creates a new matcher if one does not exist.
	 *
	 * The getter function returns a const* so there is no risk of a client altering a cached object.
	 */
	class PersistentMatcherFactory {
	public:

		/**
		 * @brief Get a matcher for the given Persistent object
		 *
		 * If a matcher for the given Persistent object already exists in the cache then it is returned, otherwise a new one is created and added to the cache before being returned.
		 *
		 * @param game_data Persistent object to get a matcher for
		 * @return const PersistentMatcher* Matcher for the given Persistent object
		 */
		const PersistentMatcher* matcher(const rule::GameRuleData* game_data);

	private:
		inline static std::map<std::string, PersistentMatcher> matchers_{};
		std::mutex mutex_{};
	};

} // namespace rm::rule::table