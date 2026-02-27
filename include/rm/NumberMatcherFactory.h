#pragma once
#include <mutex>
#include <map>
#include <NumberRange.h>

namespace rm {

	/**
	 * @brief Thread safe factory class for NumberRange objects that can be used for matching dice rolls
	 *
	 * The factory stores all matchers that have been created previously and only creates a new matcher if one does not exist.
	 * This works because the only state that the matcher has are the upper and lower bounds and these are constant for the
	 * life of the matcher so they are suitable for reuse.
	 *
	 * The getter function returns a const* so there is no risk of a client altering a cached object.
	 */
	class NumberMatcherFactory {
	public:
		/**
		 * @brief  Get a NumberRange matcher for dice rolls
		 * @param min int Lower bound to match
		 * @param max int Upper bound to match
		 * @return NumberRange<int>* Pointer to matcher
		 */
		const archid::NumberRange<int>* matcher(int min, int max);

	private:
		static std::map<std::pair<int, int>, archid::NumberRange<int>> matchers_;
		std::mutex mutex_{};
	};

} // namespace rm