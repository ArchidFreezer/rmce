#include <PersistentObjectJsonSerializer.h>

namespace rm::rule::parser {

void PersistentObjectJsonSerializer::read(std::istream& is) {
	try {
		pt::ptree ptree{};
		pt::read_json(is, ptree);
		parse(ptree);
	} catch (const pt::json_parser::json_parser_error& err) {
		std::cerr << err.what() << std::endl;
	}
}

} // namespace rm::rule::parser