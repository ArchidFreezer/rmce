#pragma once

#include <sstream>
#include <PersistentObjectManager.h>

namespace rm::rule::serial {

/**
 * @class FileSerializer
 * @brief Interface class for loading and saving data to a file.
 *
 */
class FileSerializer {
public:
	virtual ~FileSerializer() = default;

	/**
	 * @brief Load the data from a file
	 *
	 * @param is Input stream to read from
	 */
	virtual void load(std::istream& is) = 0;

	/**
	 * @brief Load the data from a file
	 *
	 * @param filename Path to the JSON file to read from
	 */
	virtual void load(const std::string& filename) = 0;
	 
	/**
	 * @brief Load the data
	 */
	virtual void load() = 0;

	/**
	 * @brief Save the data to a file
	 *
	 * @param os Output stream to write to
	 */
	virtual void save(std::ostream& os) const = 0;

	/**
	 * @brief Save the data to a file
	 *
	 * @param filename Path to the JSON file to write to
	 */
	virtual void save(const std::string& filename) const = 0;

	/**
	 * @brief Save the data to a file
	 */
	virtual void save() const = 0;
};

}
