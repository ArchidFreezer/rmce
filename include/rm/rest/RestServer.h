#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <memory>
#include <string>
#include <thread>
#include <vector>

// Forward declarations
namespace rm {
class PersistentObjectManager;
} // namespace rm

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

namespace rm::rest {

/**
 * @class RestServer
 * @brief HTTP REST API server using Boost.Beast
 *
 * Provides a multi-threaded HTTP server for REST API endpoints.
 */
class RestServer {
public:
	/**
	 * @brief Constructor
	 * @param address IP address to bind to (e.g., "0.0.0.0")
	 * @param port Port number to listen on
	 * @param num_threads Number of worker threads
	 * @param object_manager Pointer to PersistentObjectManager for data queries
	 */
	RestServer(const std::string& address, unsigned short port, int num_threads = 1, PersistentObjectManager* object_manager = nullptr);

	/**
	 * @brief Destructor - stops the server
	 */
	~RestServer();

	/**
	 * @brief Start the server
	 */
	void start();

	/**
	 * @brief Stop the server
	 */
	void stop();

	/**
	 * @brief Check if server is running
	 * @return true if running, false otherwise
	 */
	bool isRunning() const {
		return running_;
	}

	/**
	 * @brief Set the PersistentObjectManager
	 * @param object_manager Pointer to PersistentObjectManager
	 */
	void setObjectManager(PersistentObjectManager* object_manager) {
		object_manager_ = object_manager;
	}

	/**
	 * @brief Get the PersistentObjectManager
	 * @return Pointer to PersistentObjectManager (may be nullptr)
	 */
	PersistentObjectManager* getObjectManager() const {
		return object_manager_;
	}

private:
	void doAccept();
	void onAccept(beast::error_code ec, tcp::socket socket);

	net::io_context ioc_;
	tcp::acceptor acceptor_;
	std::vector<std::thread> threads_;
	bool running_;
	int num_threads_;
	PersistentObjectManager* object_manager_;
};

/**
 * @class Session
 * @brief Handles an individual HTTP session
 */
class Session : public std::enable_shared_from_this<Session> {
public:
	/**
	 * @brief Constructor
	 * @param socket TCP socket for the session
	 * @param object_manager Pointer to PersistentObjectManager for data queries
	 */
	explicit Session(tcp::socket socket, PersistentObjectManager* object_manager);

	/**
	 * @brief Start the session
	 */
	void run();

private:
	void doRead();
	void onRead(beast::error_code ec, std::size_t bytes_transferred);
	void handleRequest();
	void doWrite();
	void onWrite(beast::error_code ec, std::size_t bytes_transferred);
	void doClose();

	beast::tcp_stream stream_;
	beast::flat_buffer buffer_;
	http::request<http::string_body> request_;
	http::response<http::string_body> response_;
	PersistentObjectManager* object_manager_;
};

} // namespace rm::rest