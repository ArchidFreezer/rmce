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

	enum class FileFormat {
		kJson,
		kXml
	};
	using enum FileFormat; // Bring enumerations into current scope (C++20) so we can use DatafileParser::<val> rather than DatafileParser::FileFormat::<val>

	DatafileParser(std::string_view filename, GameRuleDataCache& cache, FileFormat const filetype);
	DatafileParser(std::string_view filename, GameRuleDataCache& cache);
	virtual ~DatafileParser() = default;
		
	void setDataType(FileFormat const filetype) { filetype_ = filetype; }
	FileFormat getDatafileType() const { return filetype_; }
	const std::string& getDataType() { return datatype_; }

	void read();
	virtual void parse() = 0;
	virtual void saveJson(const std::string& filename) = 0;
	void save(const std::string& filename, FileFormat const filetype);
	void save(const std::string& filename);

	/*
	* Creates a string that is a combination of the type and val parameters
	* The returned string will only contain ucase letters, numbers and the underscore character
	* Any character that is not an alphanum will be converted to an underscore with consecutive underscores being discarded
	*/
	static const std::string getID(std::string_view type, std::string_view val);

protected:
	std::string datafile_{};
	std::string datatype_{};
	pt::ptree ptree_{};
	FileFormat filetype_{ FileFormat::kJson };
	GameRuleDataCache& cache_;
};


