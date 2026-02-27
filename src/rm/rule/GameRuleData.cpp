#include <sstream>
#include <string>
#include <GameRuleData.h>

namespace rm {

	const std::string GameRuleData::generateId(std::string_view type, std::string_view val) {
		std::stringstream ss;

		// Add the characters from the type, replacing non alphanum characters with an underscore
		// discarding consecutive underscores.
		bool last_alpha{ false };
		for (char c : type) {
			if (iswalnum(c)) {
				last_alpha = true;
				ss.put(toupper(c));
			} else if (last_alpha) {
				ss.put('_');
				last_alpha = false;
			}
		}

		// Add the type/val separator
		if (last_alpha) ss.put('_');

		// Add the chracters from the val, replacing non alphanum characters with an underscore
		// discarding consecutive underscores.
		last_alpha = false;
		for (char c : val) {
			if (iswalnum(c)) {
				last_alpha = true;
				ss.put(toupper(c));
			} else if (last_alpha) {
				ss.put('_');
				last_alpha = false;
			}
		}

		// Convert to a string so we can use the pop_back method
		std::string ret = ss.str();
		// If the last character was not an alphanum then remove it
		if (!last_alpha) ret.pop_back();

		return ret;
	}

} // namespace rm