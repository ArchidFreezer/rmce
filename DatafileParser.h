#pragma once
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace pt = boost::property_tree;

class DatafileParser
{
public:

	enum class FileFormat {
		json,
		xml
	};
	using enum FileFormat; // Bring enumerations into current scope (C++20) so we can use DatafileParser::<val> rather than DatafileParser::FileFormat::<val>

	DatafileParser(std::string_view filename, FileFormat const filetype);
	DatafileParser(std::string_view filename);
		
	void setDataType(FileFormat const filetype) { m_filetype = filetype; }
	FileFormat getDataType() const { return m_filetype; }

	void read();
	virtual void parse() = 0;
	virtual void save(const std::string& filename, FileFormat const filetype) = 0;
	virtual void save(const std::string& filename) = 0;

	/*
	* Creates a string that is a combination of the type and val parameters
	* The returned string will only contain ucase letters, numbers and the underscore character
	* Any character that is not an alphanum will be converted to an underscore with consecutive underscores being discarded
	*/
	static const std::string getID(std::string_view type, std::string_view val);

protected:
	std::string m_datafile{};
	FileFormat m_filetype{ FileFormat::json };
	pt::ptree m_ptree{};
};


