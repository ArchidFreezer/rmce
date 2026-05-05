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
     * @brief Get the data location setting.
     *
     * @return The configured data location path as a string.
     */
    const std::string& dataLocation() const {
        return data_location_;
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
     * @brief Set the data location setting.
     *
     * @param data_location The data location path to set.
     */
    void setDataLocation(const std::string& data_location) {
        std::lock_guard<std::mutex> lock(mutex_);
        data_location_ = data_location;
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
    std::string log_level_{"info"};        /**< The log level setting (default: "info") */
    std::string data_location_{"./data"};  /**< The data location path (default: "./data") */
    std::string config_file_path_{};       /**< The path to the configuration file */

    // Thread safety
    mutable std::mutex mutex_; /**< Mutex for thread-safe access to configuration fields */

    // Singleton instance
    static std::shared_ptr<AppConfig> instance_; /**< The singleton instance */
    static std::mutex instance_mutex_;           /**< Mutex for thread-safe singleton initialization */
};

} // namespace rm
