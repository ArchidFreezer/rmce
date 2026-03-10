#include <RestServer.h>
#include <GameRuleDatas.h>
#include <PersistentObjectManager.h>
#include <iostream>
#include <boost/asio/dispatch.hpp>
#include <boost/url.hpp>
#include <sstream>
#include <map>

namespace rm::rest {

// Helper function to escape JSON strings
std::string escapeJson(const std::string& str) {
	std::ostringstream escaped;
	for (char c : str) {
		switch (c) {
		case '"':
			escaped << "\\\"";
			break;
		case '\\':
			escaped << "\\\\";
			break;
		case '\b':
			escaped << "\\b";
			break;
		case '\f':
			escaped << "\\f";
			break;
		case '\n':
			escaped << "\\n";
			break;
		case '\r':
			escaped << "\\r";
			break;
		case '\t':
			escaped << "\\t";
			break;
		default:
			if (c < 0x20) {
				escaped << "\\u" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(c);
			} else {
				escaped << c;
			}
		}
	}
	return escaped.str();
}

// Session Implementation
Session::Session(tcp::socket socket, PersistentObjectSerializationManager* object_manager) : stream_(std::move(socket)), object_manager_(object_manager) {
}

void Session::run() {
	// Set the timeout for read operations
	stream_.expires_after(std::chrono::seconds(30));
	doRead();
}

void Session::doRead() {
	request_ = {};

	http::async_read(stream_, buffer_, request_, [self = shared_from_this()](beast::error_code ec, std::size_t bytes_transferred) { self->onRead(ec, bytes_transferred); });
}

void Session::onRead(beast::error_code ec, std::size_t bytes_transferred) {
	boost::ignore_unused(bytes_transferred);

	if (ec == http::error::end_of_stream) {
		return doClose();
	}

	if (ec) {
		std::cerr << "Read error: " << ec.message() << std::endl;
		return;
	}

	handleRequest();
}

void Session::handleRequest() {
	// Prepare response
	response_ = {};
	response_.version(request_.version());
	response_.keep_alive(request_.keep_alive());

	// Extract path and query parameters
	const PathParser path(request_.target(), "/api/objects/");

	// Route handling
	if (request_.method() == http::verb::get && path.matchExact("/")) {
		response_.result(http::status::ok);
		response_.set(http::field::server, BOOST_BEAST_VERSION_STRING);
		response_.set(http::field::content_type, "application/json");
		response_.body() = R"({"message": "REST API Server", "status": "running"})";
	} else if (request_.method() == http::verb::get && path.matchExact("/health")) {
		response_.result(http::status::ok);
		response_.set(http::field::server, BOOST_BEAST_VERSION_STRING);
		response_.set(http::field::content_type, "application/json");
		response_.body() = R"({"status": "healthy"})";
	} else if (request_.method() == http::verb::get && path.matchExact("/api/version")) {
		response_.result(http::status::ok);
		response_.set(http::field::server, BOOST_BEAST_VERSION_STRING);
		response_.set(http::field::content_type, "application/json");
		response_.body() = R"({"version": "1.0.0", "api": "v1"})";
	} else if (request_.method() == http::verb::get && path.matchExact("/api/objects/prefixes")) {
		response_.result(http::status::ok);
		response_.set(http::field::server, BOOST_BEAST_VERSION_STRING);
		response_.set(http::field::content_type, "application/json");

		if (!object_manager_) {
			response_.result(http::status::service_unavailable);
			response_.body() = R"({"error": "Object manager not available"})";
		} else {
			try {
				std::set<std::string> prefixes = object_manager_->objectManager().getAllPrefixes();

				std::ostringstream json;
				std::string json_str = object_manager_->serializeContainer(prefixes, "prefixes");
				json << json_str;

				response_.result(http::status::ok);
				response_.body() = json.str();
			} catch (const std::exception& e) {
				response_.result(http::status::internal_server_error);
				response_.body() = R"({"error": "Failed to retrieve objects", "message": ")" + escapeJson(e.what()) + R"("})";
			}
		}
	} else if (request_.method() == http::verb::get && path.match("/api/objects/") && !path.type().empty() && (path.op() == "list")) {
		// List all object IDs
		// Example: /api/objects/skill/list
		response_.set(http::field::server, BOOST_BEAST_VERSION_STRING);
		response_.set(http::field::content_type, "application/json");

		if (!object_manager_) {
			response_.result(http::status::service_unavailable);
			response_.body() = R"({"error": "Object manager not available"})";
		} else {
			try {
				std::ostringstream json;
				// json << "{\"objects\": [";
				std::string json_str = object_manager_->serializeAllObjects(path.type());
				json << json_str;
				// json << "], \"count\": " << objects.size() << "}";

				response_.result(http::status::ok);
				response_.body() = json.str();
			} catch (const std::exception& e) {
				response_.result(http::status::internal_server_error);
				response_.body() = R"({"error": "Failed to retrieve objects", "message": ")" + escapeJson(e.what()) + R"("})";
			}
		}
	} else if (request_.method() == http::verb::get && path.matchExact("/api/objects")) {
		// Get object by ID
		// Example: /api/objects?id=SKILL_123
		response_.set(http::field::server, BOOST_BEAST_VERSION_STRING);
		response_.set(http::field::content_type, "application/json");

		if (!object_manager_) {
			response_.result(http::status::service_unavailable);
			response_.body() = R"({"error": "Object manager not available"})";
		} else {
			auto id_it = path.params().find("id");
			if (id_it == path.params().end()) {
				response_.result(http::status::bad_request);
				response_.body() = R"({"error": "Missing 'id' parameter"})";
			} else {
				try {
					const std::string& id = id_it->second;
					// Get the JSON representation of the object by ID
					auto obj_json_str = object_manager_->serializeAnyObject(id);
					if (obj_json_str.empty()) {
						// Wew should never get here if the object manager is implemented correctly, but just in case
						response_.result(http::status::not_found);
						response_.body() = R"({"error": "Object not found", "id": ")" + escapeJson(id) + R"("})";
					} else {
						response_.result(http::status::ok);
						response_.body() = obj_json_str;
					}
				} catch (const std::exception& e) {
					// This could be as simple as an incorrect prefix in the ID (e.g. "SKILLS_" instead of "SKILL_") or a more serious issue with the object manager
					response_.result(http::status::internal_server_error);
					response_.body() = R"({"error": "Failed to retrieve object", "message": ")" + escapeJson(e.what()) + R"("})";
				}
			}
		}
	} else if (request_.method() == http::verb::get && path.match("/api/objects/") && !path.type().empty() && (path.op() == "count")) {
		// Get count of objects
		response_.set(http::field::server, BOOST_BEAST_VERSION_STRING);
		response_.set(http::field::content_type, "application/json");

		if (!object_manager_) {
			response_.result(http::status::service_unavailable);
			response_.body() = R"({"error": "Object manager not available"})";
		} else {
			try {
				const size_t count = object_manager_->objectManager().getAllIds(path.type()).size(); // Placeholder

				std::ostringstream json;
				json << "{\"count\": " << count << "}";

				response_.result(http::status::ok);
				response_.body() = json.str();
			} catch (const std::exception& e) {
				response_.result(http::status::internal_server_error);
				response_.body() = R"({"error": "Failed to get count", "message": ")" + escapeJson(e.what()) + R"("})";
			}
		}
	} else if (request_.method() == http::verb::get && path.match("/api/objects/") && !path.type().empty() && (path.op() == "ids")) {
		// Get count of objects
		response_.set(http::field::server, BOOST_BEAST_VERSION_STRING);
		response_.set(http::field::content_type, "application/json");

		if (!object_manager_) {
			response_.result(http::status::service_unavailable);
			response_.body() = R"({"error": "Object manager not available"})";
		} else {
			try {
				const std::set<std::string> ids = object_manager_->objectManager().getAllIds(path.type());
				std::string key = object_manager_->getRootKeyForType(path.type());

				std::ostringstream json;
				json << object_manager_->serializeContainer(ids, key);

				response_.result(http::status::ok);
				response_.body() = json.str();
			} catch (const std::exception& e) {
				response_.result(http::status::internal_server_error);
				response_.body() = R"({"error": "Failed to get ids", "message": ")" + escapeJson(e.what()) + R"("})";
			}
		}
	} else if (request_.method() == http::verb::get && path.matchExact("/api/echo")) {
		// Echo endpoint for testing
		response_.result(http::status::ok);
		response_.set(http::field::server, BOOST_BEAST_VERSION_STRING);
		response_.set(http::field::content_type, "application/json");

		std::ostringstream json;
		json << "{\"query_params\": {";

		bool first = true;
		for (const auto& [key, value] : path.params()) {
			if (!first)
				json << ", ";
			json << "\"" << escapeJson(key) << "\": \"" << escapeJson(value) << "\"";
			first = false;
		}

		json << "}}";
		response_.body() = json.str();
	} else {
		response_.result(http::status::not_found);
		response_.set(http::field::server, BOOST_BEAST_VERSION_STRING);
		response_.set(http::field::content_type, "application/json");
		response_.body() = R"({"error": "Not Found", "message": "The requested resource was not found"})";
	}

	response_.prepare_payload();
	doWrite();
}

void Session::doWrite() {
	http::async_write(stream_, response_, [self = shared_from_this()](beast::error_code ec, std::size_t bytes_transferred) { self->onWrite(ec, bytes_transferred); });
}

void Session::onWrite(beast::error_code ec, std::size_t bytes_transferred) {
	boost::ignore_unused(bytes_transferred);

	if (ec) {
		std::cerr << "Write error: " << ec.message() << std::endl;
		return;
	}

	bool close = response_.need_eof();
	if (close) {
		return doClose();
	}

	response_ = {};
	doRead();
}

void Session::doClose() {
	beast::error_code ec;
	stream_.socket().shutdown(tcp::socket::shutdown_send, ec);

	if (ec && ec != beast::errc::not_connected) {
		std::cerr << "Shutdown error: " << ec.message() << std::endl;
	}
}

// RestServer Implementation
RestServer::RestServer(const std::string& address, unsigned short port, int num_threads, PersistentObjectSerializationManager* object_manager)
    : ioc_(num_threads), acceptor_(net::make_strand(ioc_)), running_(false), num_threads_(num_threads), object_manager_(object_manager) {
	beast::error_code ec;

	if (num_threads_ <= 0) {
		throw std::runtime_error("Number of threads must be greater than 0");
	}

	if (port <= 0) {
		throw std::runtime_error("Port number must be greater than 0");
	}

	tcp::endpoint endpoint{net::ip::make_address(address), port};

	acceptor_.open(endpoint.protocol(), ec);
	if (ec) {
		throw std::runtime_error("Failed to open acceptor: " + ec.message());
	}

	acceptor_.set_option(net::socket_base::reuse_address(true), ec);
	if (ec) {
		throw std::runtime_error("Failed to set reuse_address: " + ec.message());
	}

	acceptor_.bind(endpoint, ec);
	if (ec) {
		throw std::runtime_error("Failed to bind: " + ec.message());
	}

	acceptor_.listen(net::socket_base::max_listen_connections, ec);
	if (ec) {
		throw std::runtime_error("Failed to listen: " + ec.message());
	}

	std::cout << "Server initialized on " << address << ":" << port << std::endl;
}

RestServer::~RestServer() {
	stop();
}

void RestServer::start() {
	if (running_) {
		return;
	}

	running_ = true;
	doAccept();

	threads_.reserve(num_threads_);
	for (int i = 0; i < num_threads_; ++i) {
		threads_.emplace_back([this] { ioc_.run(); });
	}

	std::cout << "Server started with " << num_threads_ << " thread(s)" << std::endl;
}

void RestServer::stop() {
	if (!running_) {
		return;
	}

	running_ = false;

	beast::error_code ec;
	acceptor_.close(ec);

	ioc_.stop();

	for (auto& thread : threads_) {
		if (thread.joinable()) {
			thread.join();
		}
	}

	threads_.clear();
	std::cout << "Server stopped" << std::endl;
}

void RestServer::doAccept() {
	acceptor_.async_accept(net::make_strand(ioc_), [this](beast::error_code ec, tcp::socket socket) { onAccept(ec, std::move(socket)); });
}

void RestServer::onAccept(beast::error_code ec, tcp::socket socket) {
	if (ec) {
		if (ec != net::error::operation_aborted) {
			std::cerr << "Accept error: " << ec.message() << std::endl;
		}
	} else {
		std::make_shared<Session>(std::move(socket), object_manager_)->run();
	}

	if (running_) {
		doAccept();
	}
}

} // namespace rm::rest