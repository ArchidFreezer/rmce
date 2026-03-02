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

template<persistent_object PersistentObject>
inline void PersistentObjectJsonSerializer::save(std::ostream& os) {
	// Root tree that will contain the array of persistent objects
	pt::ptree tree;

	// Tree of persistent objects
	pt::ptree data;

	std::set<std::string> keys{};
	manager().keys<PersistentObject>(keys);

	for (std::string key : keys) {
		try {
			pt::ptree datum;
			populateDatum(key, datum);
			data.push_back(std::make_pair("", datum));
		} catch (const std::out_of_range& e) {
			std::cout << "Error: " << e.what() << std::endl;
		}
	}

	tree.add_child(rootNode(), data);

	pt::write_json(os, tree);
}

} // namespace rm::rule::parser