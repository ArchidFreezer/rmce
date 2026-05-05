#include <AppConfig.h>
#include <iostream>
#include <exception>

/**
 * @brief Example program demonstrating the usage of the AppConfig class.
 *
 * This example shows how to:
 * 1. Initialize the configuration from a JSON file
 * 2. Access configuration values
 * 3. Modify configuration values
 * 4. Save configuration back to file
 */
int main(int argc, char* argv[]) {
    try {
        // Determine the configuration file path
        std::string config_path = "config.json";
        if (argc > 1) {
            config_path = argv[1];
        }

        std::cout << "Loading configuration from: " << config_path << std::endl;

        // Initialize the configuration singleton
        rm::AppConfig::initialize(config_path);

        // Get the configuration instance
        auto config = rm::AppConfig::instance();

        // Access configuration values
        std::cout << "\nCurrent configuration:" << std::endl;
        std::cout << "  Log Level: " << config->logLevel() << std::endl;
        std::cout << "  Data Location: " << config->dataInput() << std::endl;

        // Demonstrate JSON representation
        std::cout << "\nJSON representation:" << std::endl;
        boost::json::object json_config = config->toJson();
        std::cout << boost::json::serialize(json_config) << std::endl;

        // Example: Modify configuration (thread-safe)
        std::cout << "\nModifying log level to 'debug'..." << std::endl;
        config->setLogLevel("debug");

        std::cout << "Updated Log Level: " << config->logLevel() << std::endl;

        // Example: Save modified configuration
        std::string output_path = "config_modified.json";
        std::cout << "\nSaving modified configuration to: " << output_path << std::endl;
        config->save(output_path);

        std::cout << "\nConfiguration example completed successfully!" << std::endl;

        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
