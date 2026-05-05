#include <AppConfig.h>
#include <fstream>
#include <sstream>
#include <boost/system/error_code.hpp>

namespace rm {

// Initialize static members
std::shared_ptr<AppConfig> AppConfig::instance_{nullptr};
std::mutex AppConfig::instance_mutex_{};

void AppConfig::initialize(const std::string& config_file_path) {
    std::lock_guard<std::mutex> lock(instance_mutex_);

    if (instance_) {
        throw std::runtime_error("AppConfig::initialize() - Configuration has already been initialized");
    }

    instance_ = std::shared_ptr<AppConfig>(new AppConfig());
    instance_->load(config_file_path);
}

std::shared_ptr<AppConfig> AppConfig::instance() {
    std::lock_guard<std::mutex> lock(instance_mutex_);

    if (!instance_) {
        throw std::runtime_error("AppConfig::instance() - Configuration has not been initialized. Call initialize() first.");
    }

    return instance_;
}

void AppConfig::load(const std::string& config_file_path) {
    config_file_path_ = config_file_path;

    // Read the file
    std::ifstream file(config_file_path);
    if (!file.is_open()) {
        throw std::runtime_error("AppConfig::load() - Failed to open configuration file: " + config_file_path);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();

    if (content.empty()) {
        throw std::runtime_error("AppConfig::load() - Configuration file is empty: " + config_file_path);
    }

    // Parse JSON
    boost::system::error_code ec;
    json::value jv = json::parse(content, ec);

    if (ec) {
        throw std::runtime_error("AppConfig::load() - Failed to parse JSON from file: " + config_file_path + " - " + ec.message());
    }

    if (!jv.is_object()) {
        throw std::runtime_error("AppConfig::load() - JSON root must be an object in file: " + config_file_path);
    }

    // Load configuration from JSON
    fromJson(jv.as_object());
}

void AppConfig::fromJson(const json::object& json_obj) {
    std::lock_guard<std::mutex> lock(mutex_);

    // Read log-level
    if (json_obj.contains("log-level")) {
        const json::value& log_level_val = json_obj.at("log-level");
        if (log_level_val.is_string()) {
            log_level_ = json_obj.at("log-level").as_string().c_str();
        }
    }

    // Read data-location
    if (json_obj.contains("data-location")) {
        const json::value& data_location_val = json_obj.at("data-location");
        if (data_location_val.is_string()) {
            data_location_ = json_obj.at("data-location").as_string().c_str();
        }
    }
}

json::object AppConfig::toJson() const {
    std::lock_guard<std::mutex> lock(mutex_);

    json::object obj;
    obj["log-level"] = log_level_;
    obj["data-location"] = data_location_;

    return obj;
}

void AppConfig::save(const std::string& config_file_path) const {
    std::string path = config_file_path.empty() ? config_file_path_ : config_file_path;

    if (path.empty()) {
        throw std::runtime_error("AppConfig::save() - No configuration file path specified");
    }

    // Get JSON representation
    json::object obj = toJson();

    // Write to file
    std::ofstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("AppConfig::save() - Failed to open file for writing: " + path);
    }

    // Pretty print the JSON
    file << json::serialize(obj);
}

} // namespace rm
