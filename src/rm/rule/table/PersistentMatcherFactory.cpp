#include <PersistentMatcherFactory.h>

namespace rm::rule::table {

	const PersistentMatcher* PersistentMatcherFactory::matcher(const rule::GameRuleData* game_data) {
		std::lock_guard<std::mutex> lock(mutex_);
		auto it = matchers_.find(game_data->id());
		if (it != matchers_.end()) {
			return &(it->second);
		}
		return &matchers_.emplace(game_data->id(), PersistentMatcher(*game_data)).first->second;
	}

} // namespace rm::rule::table