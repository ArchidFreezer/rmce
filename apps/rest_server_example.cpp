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
	// Set log level from environment variable
	auto log_level_env = std::getenv("RMCE_LOG_LEVEL");
	auto level = spdlog::level::debug;
	if (log_level_env) {
		level = spdlog::level::from_str(log_level_env);
	}
	rm::util::Logger::init("rmce.log", level);

	try {
		// Register signal handler
		std::signal(SIGINT, signalHandler);
		std::signal(SIGTERM, signalHandler);

		// Parse command line arguments
		std::string address = "0.0.0.0";
		unsigned short port = 8080;
		int num_threads = 4;

		if (argc >= 2) {
			address = argv[1];
		}
		if (argc >= 3) {
			port = static_cast<unsigned short>(std::atoi(argv[2]));
		}
		if (argc >= 4) {
			num_threads = std::atoi(argv[3]);
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
		json_manager.load();

		rm::rest::RestServer server(address, port, num_threads);
		server.setObjectManager(&json_manager);
		server.start();
		json_manager.setDataDirectory("../../../../data2/"); // Set the data directory for saving (optional if already set)

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