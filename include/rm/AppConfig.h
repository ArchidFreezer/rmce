#pragma once

#include <boost/json.hpp>
#include <string>
#include <memory>
#include <mutex>
#include <stdexcept>

namespace rm {

namespace json = boost::json;

/**
 * @class AppConfig
 * @brief Thread-safe singleton class for managing application configuration loaded from a JSON file.
 *
 * This class provides global access to application configuration parameters that are loaded from a JSON file at startup.
 * The configuration is accessed through a shared_ptr to ensure thread-safe access and proper lifetime management.
 *
 * Example JSON format:
 * @code
 * {
 *     "log-level": "debug",
 *     "data-location": "./data"
 * }
 * @endcode
 *
 * Example usage:
 * @code
 * // Initialize configuration at startup
 * AppConfig::initialize("config.json");
 *
 * // Access configuration
 * auto config = AppConfig::instance();
 * std::string log_level = config->logLevel();
 * @endcode
 */
class AppConfig {
public:
	/**
	 * @brief Initialize the configuration singleton from a JSON file.
	 *
	 * This method must be called once at application startup before any calls to instance().
	 * Subsequent calls will throw an exception.
	 *
	 * @param config_file_path Path to the JSON configuration file to load.
	 * @throws std::runtime_error If the configuration has already been initialized, if the file cannot be read, or if the JSON is invalid.
	 */
	static void initialize(const std::string& config_file_path);

	/**
	 * @brief Get the singleton instance of the configuration.
	 *
	 * @return A shared_ptr to the AppConfig singleton instance.
	 * @throws std::runtime_error If initialize() has not been called yet.
	 */
	static std::shared_ptr<AppConfig> instance();

	/**
	 * @brief Get the log level setting.
	 *
	 * @return The configured log level as a string (e.g., "trace", "debug", "info", "warn", "error", "critical", "off").
	 */
	const std::string& logLevel() const {
		return log_level_;
	}

	/**
	 * @brief Get the data input setting.
	 *
	 * @return The configured data input path as a string.
	 */
	const std::string& dataInput() const {
		return data_input_;
	}

	/**
	 * @brief Get the data output setting.
	 *
	 * @return The configured data output path as a string.
	 */
	const std::string& dataOutput() const {
		return data_output_;
	}

	/**
	 * @brief Set the log level setting.
	 *
	 * @param log_level The log level to set (e.g., "trace", "debug", "info", "warn", "error", "critical", "off").
	 */
	void setLogLevel(const std::string& log_level) {
		std::lock_guard<std::mutex> lock(mutex_);
		log_level_ = log_level;
	}

	/**
	 * @brief Set the data input setting.
	 *
	 * @param data_input The data input path to set.
	 */
	void setDataInput(const std::string& data_input) {
		std::lock_guard<std::mutex> lock(mutex_);
		data_input_ = data_input;
	}

	/**
	 * @brief Set the data output setting.
	 *
	 * @param data_output The data output path to set.
	 */
	void setDataOutput(const std::string& data_output) {
		std::lock_guard<std::mutex> lock(mutex_);
		data_output_ = data_output;
	}

	/**
	 * @brief Get the REST API port setting.
	 *
	 * @return The configured REST API port as an unsigned short.
	 */
	unsigned short restPort() const {
		return rest_port_;
	}

	/**
	 * @brief Set the REST API port setting.
	 *
	 * @param rest_port The REST API port to set.
	 */
	void setRestPort(unsigned short rest_port) {
		std::lock_guard<std::mutex> lock(mutex_);
		rest_port_ = rest_port;
	}

	/**
	 * @brief Get the number of REST threads to run.
	 *
	 * @return The number of REST threads to run.
	 */
	int restNumThreads() const {
		return rest_num_threads_;
	}

	/**
	 * @brief Set the number of REST threads to run setting.
	 *
	 * @param rest_num_threads The number of REST threads to run.
	 */
	void setRestNumThreads(int rest_num_threads) {
		std::lock_guard<std::mutex> lock(mutex_);
		rest_num_threads_ = rest_num_threads;
	}

	/**
	 * @brief Get the REST API host setting.
	 *
	 * @return The configured REST API host as a string.
	 */
	std::string restHost() const {
		return rest_host_;
	}

	/**
	 * @brief Set the REST API host setting.
	 *
	 * @param rest_host The REST API host to set.
	 */
	void setRestHost(const std::string& rest_host) {
		std::lock_guard<std::mutex> lock(mutex_);
		rest_host_ = rest_host;
	}

	/**
	 * @brief Save the current configuration to a JSON file.
	 *
	 * @param config_file_path Path to the JSON file to write. If empty, uses the path from initialization.
	 * @throws std::runtime_error If the file cannot be written.
	 */
	void save(const std::string& config_file_path = "") const;

	/**
	 * @brief Get the JSON representation of the configuration.
	 *
	 * @return A json::object containing the current configuration.
	 */
	json::object toJson() const;

	// Delete copy and move constructors and assignment operators
	AppConfig(const AppConfig&) = delete;
	AppConfig& operator=(const AppConfig&) = delete;
	AppConfig(AppConfig&&) = delete;
	AppConfig& operator=(AppConfig&&) = delete;

private:
	/**
	 * @brief Private constructor - use initialize() and instance() instead.
	 */
	AppConfig() = default;

	/**
	 * @brief Load configuration from a JSON file.
	 *
	 * @param config_file_path Path to the JSON configuration file to load.
	 * @throws std::runtime_error If the file cannot be read or the JSON is invalid.
	 */
	void load(const std::string& config_file_path);

	/**
	 * @brief Parse configuration from a JSON object.
	 *
	 * @param json_obj The JSON object to parse.
	 */
	void fromJson(const json::object& json_obj);

	// Configuration fields
	std::string log_level_{"info"};       /**< The log level setting (default: "info") */
	std::string data_input_{"./data"};    /**< The data input path (default: "./data") */
	std::string data_output_{"./output"}; /**< The data input path (default: "./output") */
	unsigned short rest_port_{8080};      /**< The REST API port (default: "8080") */
	int rest_num_threads_{4};             /**< The number of threads for the REST API server (default: 4) */
	std::string rest_host_{"0.0.0.0"};    /**< The REST API host (default: "0.0.0.0") */
	std::string config_file_path_{};      /**< The path to the configuration file */

	// Thread safety
	mutable std::mutex mutex_; /**< Mutex for thread-safe access to configuration fields */

	// Singleton instance
	static std::shared_ptr<AppConfig> instance_; /**< The singleton instance */
	static std::mutex instance_mutex_;           /**< Mutex for thread-safe singleton initialization */
};

} // namespace rm
