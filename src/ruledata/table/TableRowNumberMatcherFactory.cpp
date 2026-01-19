#include <table/TableRowNumberMatcherFactory.h>

// Static members must be initialised outside the header
std::map<std::pair<int, int>, NumberRange<int>> TableRowNumberMatcherFactory::matchers_{};

const NumberRange<int>* TableRowNumberMatcherFactory::matcher(int min, int max) {
	std::lock_guard<std::mutex> lock(mutex_);
	for (auto& v : matchers_) {
		if (v.first.first == min && v.first.second == max) {
			return &(v.second);
		}
	}
	std::pair<int, int> key({ min, max });
	return &matchers_.emplace(key, NumberRange<int>(min, max)).first->second;
}