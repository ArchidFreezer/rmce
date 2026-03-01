#include <NumberMatcherFactory.h>

namespace rm::rule::table {

	// Static members must be initialised outside the header
	std::map<std::pair<int, int>, archid::NumberRange<int>> NumberMatcherFactory::matchers_{};

	const archid::NumberRange<int>* NumberMatcherFactory::matcher(int min, int max) {
		std::lock_guard<std::mutex> lock(mutex_);
		for (auto& v : matchers_) {
			if (v.first.first == min && v.first.second == max) {
				return &(v.second);
			}
		}
		std::pair<int, int> key({ min, max });
		return &matchers_.emplace(key, archid::NumberRange<int>(min, max)).first->second;
	}

} // namespace rm::rule::table