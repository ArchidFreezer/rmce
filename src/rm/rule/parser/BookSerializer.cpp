#include <BookSerializer.h>

namespace rm::rule::parser {

void BookSerializer::parse(pt::ptree& ptree) {
	std::cout << "Loading Book data ..." << std::endl;

	// Get the books to parse and loop through them
	const pt::ptree& tree = ptree.get_child(rootNode());
	for (const auto& v : tree) {
		std::string name = v.second.get<std::string>("name");
		std::string id = v.second.get("id", manager().generateId(ruleDatatype(), name));

		// We get a BookData object from the cache and populate it from the ptree
		BookData& datum = manager().get<BookData>(id);
		datum.setAbbreviation(v.second.get<std::string>("abbreviation"));
		datum.setCode(v.second.get<std::string>("code"));
		datum.setIsbn(v.second.get<std::string>("isbn"));
		datum.setName(name);
		std::cout << "\tBook name: " << datum.name() << std::endl;
	}
	std::cout << " done" << std::endl;
}

void BookSerializer::populateDatum(std::string key, pt::ptree& datum) {
	BookData& game_data = manager().get<BookData>(key);
	datum.put("id", game_data.id());
	datum.put("code", game_data.code());
	datum.put("name", game_data.name());
	datum.put("abbreviation", game_data.abbreviation());
	datum.put("isbn", game_data.isbn());
}

std::string BookSerializer::rootNode() const {
	return "books";
}

} // namespace rm::rule::parser