#include <RestServer.h>
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

		std::cout << "Starting REST API Server..." << std::endl;
		std::cout << "Address: " << address << std::endl;
		std::cout << "Port: " << port << std::endl;
		std::cout << "Threads: " << num_threads << std::endl;
		std::cout << "\nAvailable endpoints:" << std::endl;
		std::cout << "  GET     /                            - Root endpoint" << std::endl;
		std::cout << "  GET     /health                      - Health check" << std::endl;
		std::cout << "  GET     /rmce/version                - API version" << std::endl;
		std::cout << "  GET     /rmce/prefixes               - Get game object prefixes" << std::endl;
		std::cout << "  GET     /rmce/objects/{prefix}       - Get all game objects" << std::endl;
		std::cout << "  GET     /rmce/objects/{prefix}?count - Count of game objects" << std::endl;
		std::cout << "  GET     /rmce/objects/{prefix}?ids   - List of game object IDs" << std::endl;
		std::cout << "  POST    /rmce/objects/{prefix}       - Create/update object" << std::endl;
		std::cout << "  GET     /rmce/objects/{prefix}/{id}  - Get game object" << std::endl;
		std::cout << "  DELETE  /rmce/objects/{prefix}/{id}  - Delete game object" << std::endl;
		std::cout << "\nPress Ctrl+C to stop the server..." << std::endl;

		// Create the cache to store the game data and load it from file
		rm::PersistentCache cache{};
		rm::PersistentObjectManager object_factory{cache};
		rm::PersistentObjectSerializationManager json_manager{object_factory};
		json_manager.load();

		rm::rest::RestServer server(address, port, num_threads);
		server.setObjectManager(&json_manager);
		server.start();

		// Wait for stop signal
		while (!stop_requested && server.isRunning()) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		std::cout << "\nShutting down..." << std::endl;
		server.stop();

		std::cout << "Server shutdown complete." << std::endl;
		return 0;

	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
}