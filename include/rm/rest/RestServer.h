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
#include <PersistentObjectSerializationManager.h>

// Forward declarations
namespace rm {
class PersistentObjectSerializationManager;
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
	RestServer(const std::string& address, unsigned short port, int num_threads = 1, PersistentObjectSerializationManager* object_manager = nullptr);

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
	 * @brief Set the PersistentObjectSerializationManager
	 * @param object_manager Pointer to PersistentObjectSerializationManager
	 */
	void setObjectManager(PersistentObjectSerializationManager* object_manager) {
		object_manager_ = object_manager;
	}

	/**
	 * @brief Get the PersistentObjectSerializationManager
	 * @return Pointer to PersistentObjectSerializationManager (may be nullptr)
	 */
	PersistentObjectSerializationManager* getObjectManager() const {
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
	PersistentObjectSerializationManager* object_manager_;
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
	 * @param object_manager Pointer to PersistentObjectSerializationManager for data queries
	 */
	explicit Session(tcp::socket socket, PersistentObjectSerializationManager* object_manager);

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
	PersistentObjectSerializationManager* object_manager_;
};

/**
 * @class PathParser
 * @brief Utility class for parsing request paths and query parameters
 *
 * This handles the specific case of object retrieval endpoints, where the path may contain variable segments (e.g., "api/objects/{type}/get?id=ABC").
 */
class PathParser {
public:
	/**
	 * @brief Constructs a PathParser object to parse a path with a given prefix.
	 * @param path The path string to be parsed.
	 * @param prefix The prefix string used to extract variable, operation, and pre-parameter path information.
	 */
	explicit PathParser(std::string_view path, std::string_view prefix) : path_(path), type_{extractVariable(prefix)}, op_{extractOp(prefix)}, pre_param_path_{extractPreParamPath()} {
		parseQueryParams(path);
	}

	/**
	 * @brief Checks if the pre-parameter path exactly matches the given pattern.
	 * @param pattern The string pattern to compare against the pre-parameter path.
	 * @return True if the pre-parameter path exactly matches the pattern, false otherwise.
	 */
	bool matchExact(std::string_view pattern) const {
		return pre_param_path_ == pattern;
	}

	/**
	 * @brief Checks if the pre-parameter path starts with the given pattern.
	 * @param pattern The string pattern to check as a prefix of the pre-parameter path.
	 * @return True if the pre-parameter path starts with the pattern, false otherwise.
	 */
	bool match(std::string_view pattern) const {
		return path_.starts_with(pattern);
	}

	/**
	 * @brief Gets the parameters map.
	 * @return A constant reference to the map containing string key-value pairs of parameters.
	 */
	const std::map<std::string, std::string>& params() const {
		return params_;
	}

	/**
	 * @brief Gets the type of the object to be processed.
	 * @return A string view representing the type.
	 */
	std::string_view type() const {
		return type_;
	}

	/**
	 * @brief Gets the operation to be performed on the object.
	 * @return A string view representing the operation.
	 */
	std::string_view op() const {
		return op_;
	}

private:
	std::string_view path_{};
	std::string_view type_{};
	std::string_view op_{};
	std::string_view pre_param_path_{};
	std::map<std::string, std::string> params_{};

	// Create a map of query parameters from the target string (e.g., "id=ABC&name=XYZ")
	void parseQueryParams(std::string_view target) {
		std::map<std::string, std::string> params;

		// Find the query string start
		auto query_pos = target.find('?');
		if (query_pos == std::string_view::npos) {
			params_ = params;
		}

		// Extract query string
		std::string_view query = target.substr(query_pos + 1);

		// Parse key=value pairs
		size_t start = 0;
		while (start < query.length()) {
			// Find next parameter separator
			auto amp_pos = query.find('&', start);
			auto param = query.substr(start, amp_pos - start);

			// Split on '='
			auto eq_pos = param.find('=');
			if (eq_pos != std::string_view::npos) {
				std::string key(param.substr(0, eq_pos));
				std::string value(param.substr(eq_pos + 1));

				// URL decode (basic implementation)
				// Replace '+' with space and handle %XX encoding
				for (size_t i = 0; i < value.length(); ++i) {
					if (value[i] == '+') {
						value[i] = ' ';
					}
				}

				params[key] = value;
			}

			if (amp_pos == std::string_view::npos)
				break;
			start = amp_pos + 1;
		}

		params_ = params;
	}

	// Extract the variable part of the path after the pattern and before the next segment
	std::string_view extractVariable(std::string_view pattern) const {
		if (match(pattern)) {
			// First get the part of the path after the pattern
			std::string_view postfix = path_.substr(pattern.size());
			// Then find the next '/' which indicates the end of the variable segment
			size_t nextSlash = postfix.find('/');
			if (nextSlash == std::string_view::npos) {
				// Get up the query parameters if there are any
				size_t queryPos = postfix.find('?');
				if (queryPos != std::string_view::npos) {
					return postfix.substr(0, queryPos); // Return the variable segment without query parameters
				} else {
					return {}; // No variable segment found
				}
			} else {
				return postfix.substr(0, nextSlash); // Return the variable segment
			}
		}
		return {};
	}

	std::string_view extractOp(std::string_view pattern) const {
		if (match(pattern)) {
			// First get the part of the path after the pattern
			std::string_view postfix = path_.substr(pattern.size());
			// Then find the next '/' which indicates the end of the variable segment
			size_t nextSlash = postfix.find('/');
			if (nextSlash != std::string_view::npos) {
				// Get up the query parameters if there are any
				size_t queryPos = postfix.find('?');
				if (queryPos != std::string_view::npos) {
					return postfix.substr(nextSlash + 1, queryPos - nextSlash - 1);
				} else {
					return postfix.substr(nextSlash + 1); // Return the operation segment
				}
			}
		}
		return {};
	}

	std::string_view extractPreParamPath() const {
		size_t queryPos = path_.find('?');
		if (queryPos != std::string_view::npos) {
			return path_.substr(0, queryPos);
		} else {
			return path_;
		}
	}
};

} // namespace rm::rest