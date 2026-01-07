#pragma once
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "GameRuleDataCache.h"

namespace pt = boost::property_tree;

class DatafileParser
{
public:

	DatafileParser(GameRuleDataCache& cache, const std::string& datatype);
	virtual ~DatafileParser() = default;
		
	const std::string& get_data_type() { return datatype_; }

	// Read game rule data from a file, convert to objects and store in the game rule data cache
	virtual void read(const std::string& filename) = 0;
	// Write game rule data from the cache to a file
	virtual void save(const std::string& filename) = 0;

	// Creates a string that is a combination of the type and val parameters
	// The returned string will only contain ucase letters, numbers and the underscore character
	// Any character that is not an alphanum will be converted to an underscore with consecutive underscores being discarded
	static const std::string generateId(std::string_view type, std::string_view val);

	// Get a reference to the boost ptree used when reading the datafile
	inline pt::ptree& ptree() { return ptree_; };

	// Get a reference to the game rule data cache
	inline GameRuleDataCache& cache() { return cache_; };

protected:
	// Parse a boost::ptree containing rule data from a json file, convert to objects and store in the game rule data cache
	virtual void parse() = 0;

	std::string datatype_{};

private:
	GameRuleDataCache& cache_;
	pt::ptree ptree_{};
};


