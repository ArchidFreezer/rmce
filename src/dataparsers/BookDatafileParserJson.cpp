#include <BookData.h>
#include <BookDatafileParserJson.h>

void BookDatafileParserJson::parse(bool id_only) {
	// We know there are no references in books so we create the complete object in the cache on the first pass
	if (!id_only) return;

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
		std::unique_ptr<BookData> datum = std::make_unique<BookData>(id);
		datum->setAbbreviation(abbreviation);
		datum->setCode(code);
		datum->setIsbn(isbn);
		datum->setName(name);
		std::cout << "\tBook name: " << datum->name() << std::endl;
		cache().add<BookData>(std::move(datum), id);

	}
	std::cout << " done" << std::endl;
}

void BookDatafileParserJson::populateDatum(std::string& id, pt::ptree& datum) {
	BookData& game_data = cache().get<BookData>(id);
	datum.put("id", game_data.id());
	datum.put("code", game_data.code());
	datum.put("name", game_data.name());
	datum.put("abbreviation", game_data.abbreviation());
	datum.put("isbn", game_data.isbn());
}
