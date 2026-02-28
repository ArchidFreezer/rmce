#include <BookData.h>
#include <BookDatafileParserJson.h>

namespace rm::rule::parser {

	void BookDatafileParserJson::parse() {
		std::cout << "Loading Book data ..." << std::endl;

		// Get the books to parse and loop through them
		const pt::ptree& tree = ptree().get_child(rootNode());
		for (const auto& v : tree) {
			std::string name = v.second.get<std::string>("name");
			std::string id = v.second.get("id", GameRuleData::generateId(ruleDatatype(), name));
			std::string code = v.second.get<std::string>("code");
			std::string abbreviation = v.second.get<std::string>("abbreviation");
			std::string isbn = v.second.get<std::string>("isbn");

			// We create a BookData object and reference it with as a unique_ptr to allow us to use move semantics to transfer ownership
			// to the cache when we add it
			BookData& datum = factory().get<BookData>(id);
			datum.setAbbreviation(abbreviation);
			datum.setCode(code);
			datum.setIsbn(isbn);
			datum.setName(name);
			std::cout << "\tBook name: " << datum.name() << std::endl;

		}
		std::cout << " done" << std::endl;
	}

	void BookDatafileParserJson::populateDatum(std::string& id, pt::ptree& datum) {
		BookData& game_data = factory().get<BookData>(id);
		datum.put("id", game_data.id());
		datum.put("code", game_data.code());
		datum.put("name", game_data.name());
		datum.put("abbreviation", game_data.abbreviation());
		datum.put("isbn", game_data.isbn());
	}

} // namespace rm::rule::parser