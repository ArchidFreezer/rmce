#include <AppConfig.h>
#include <RestServer.h>
#include <Logger.h>
#include <iostream>
#include <csignal>
#include <atomic>

#include <PersistentObjectSerializationManager.h>

std::atomic<bool> stop_requested{false};

void signalHandler(int signum) {
	std::cout << "\nInterrupt signal (" << signum << ") received.\n";
	stop_requested = true;
}

int main(int argc, char* argv[]) {
	try {
		// Determine the configuration file path
		std::string config_path = "config.json";
		if (argc >= 2) {
			config_path = argv[1];
		}

		std::cout << "Loading configuration from: " << config_path << std::endl;

		// Initialize the configuration singleton
		rm::AppConfig::initialize(config_path);

		// Get the configuration instance
		auto config = rm::AppConfig::instance();

		// Set log level from config file
		auto config_log_level = config->logLevel();
		auto level = spdlog::level::trace;
		if (!config_log_level.empty()) {
			level = spdlog::level::from_str(config_log_level);
		}
		rm::util::Logger::init("rmce.log", level);

		// Register signal handler
		std::signal(SIGINT, signalHandler);
		std::signal(SIGTERM, signalHandler);

		// Parse command line arguments
		std::string address = config->restHost();
		unsigned short port = config->restPort();
		int num_threads = config->restNumThreads();

		if (argc >= 2) {
			address = argv[2];
		}
		if (argc >= 4) {
			port = static_cast<unsigned short>(std::atoi(argv[3]));
		}
		if (argc >= 5) {
			num_threads = std::atoi(argv[4]);
		}

		LOG_INFO("Starting REST API Server...");
		LOG_INFO("Address: {}", address);
		LOG_INFO("Port: {}", port);
		LOG_INFO("Threads: {}\n", num_threads);
		LOG_INFO("Available endpoints:");
		LOG_INFO("  GET     /                            - Root endpoint");
		LOG_INFO("  GET     /health                      - Health check");
		LOG_INFO("  GET     /rmce/save                   - Save game data");
		LOG_INFO("  GET     /rmce/version                - API version");
		LOG_INFO("  GET     /rmce/prefixes               - Get game object prefixes");
		LOG_INFO("  GET     /rmce/data/{prefix}          - Get all game data objects");
		LOG_INFO("  GET     /rmce/data/{prefix}?count    - Count of game data objects");
		LOG_INFO("  GET     /rmce/data/{prefix}?ids      - List of game data object IDs");
		LOG_INFO("  POST    /rmce/data/{prefix}          - Create game data object");
		LOG_INFO("  GET     /rmce/data/{prefix}/{id}     - Get game data object");
		LOG_INFO("  PUT     /rmce/data/{prefix}/{id}     - Update game data object");
		LOG_INFO("  DELETE  /rmce/data/{prefix}/{id}     - Delete game data object");
		LOG_INFO("  GET     /rmce/objects/{prefix}       - Get all game objects");
		LOG_INFO("  GET     /rmce/objects/{prefix}?count - Count of game objects");
		LOG_INFO("  GET     /rmce/objects/{prefix}?ids   - List of game object IDs");
		LOG_INFO("  POST    /rmce/objects/{prefix}       - Create object");
		LOG_INFO("  GET     /rmce/objects/{prefix}/{id}  - Get game object");
		LOG_INFO("  PUT     /rmce/objects/{prefix}/{id}  - Update game object");
		LOG_INFO("  DELETE  /rmce/objects/{prefix}/{id}  - Delete game object");
		LOG_INFO("\nPress Ctrl+C to stop the server...");

		// Create the cache to store the game data and load it from file
		rm::PersistentCache cache{};
		rm::PersistentObjectManager object_factory{cache};
		rm::PersistentObjectSerializationManager json_manager{object_factory};
		json_manager.setDataDirectory(config->dataInput()); // Set the data directory for loading
		json_manager.load();

		rm::rest::RestServer server(address, port, num_threads);
		server.setObjectManager(&json_manager);
		server.start();
		json_manager.setDataDirectory(config->dataOutput()); // Set the data directory for saving (optional if already set)

		// Wait for stop signal
		while (!stop_requested && server.isRunning()) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		LOG_INFO("\nShutting down...");
		server.stop();

		json_manager.save();

		LOG_INFO("Server shutdown complete.");
		return 0;

	} catch (const std::exception& e) {
		LOG_ERROR("Fatal error: {}", e.what());
		return 1;
	}

	LOG_INFO("Application shutting down");
	rm::util::Logger::shutdown();
	return 0;
}