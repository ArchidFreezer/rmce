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
	 */
	RestServer(const std::string& address, unsigned short port, int num_threads = 1);

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

private:
	void doAccept();
	void onAccept(beast::error_code ec, tcp::socket socket);

	net::io_context ioc_;
	tcp::acceptor acceptor_;
	std::vector<std::thread> threads_;
	bool running_;
	int num_threads_;
};

/**
 * @class Session
 * @brief Handles an individual HTTP session
 */
class Session : public std::enable_shared_from_this<Session> {
public:
	/**
	 * @brief Constructs a Session object with the specified TCP socket.
	 * @param socket The TCP socket to be used for this session.
	 */
	explicit Session(tcp::socket socket);

	/**
	 * @brief Starts the asynchronous operation to read an HTTP request from the client.
	 */
	void run();

private:
	void doRead();
	void onRead(beast::error_code ec, std::size_t bytes_transferred);
	void handleRequest();
	void doWrite();
	void onWrite(beast::error_code ec, std::size_t bytes_transferred);
	void doClose();

	beast::tcp_stream stream_; // Only need tcp_stream, not both socket and stream
	beast::flat_buffer buffer_;
	http::request<http::string_body> request_;
	http::response<http::string_body> response_;
};

} // namespace rm::rest