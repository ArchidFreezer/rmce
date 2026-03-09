#include "RestServer.h"
#include <PersistentObjectManager.h>
#include <iostream>
#include <boost/asio/dispatch.hpp>
#include <boost/url.hpp>
#include <sstream>
#include <map>

namespace rm::rest {

// Helper function to parse query parameters
std::map<std::string, std::string> parseQueryParams(std::string_view target) {
	std::map<std::string, std::string> params;

	// Find the query string start
	auto query_pos = target.find('?');
	if (query_pos == std::string_view::npos) {
		return params;
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

	return params;
}

// Helper function to extract path without query string
std::string_view getPath(std::string_view target) {
	auto query_pos = target.find('?');
	if (query_pos == std::string_view::npos) {
		return target;
	}
	return target.substr(0, query_pos);
}

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
	std::string_view target = request_.target();
	std::string_view path = getPath(target);
	auto params = parseQueryParams(target);

	// Route handling
	if (request_.method() == http::verb::get && path == "/") {
		response_.result(http::status::ok);
		response_.set(http::field::server, BOOST_BEAST_VERSION_STRING);
		response_.set(http::field::content_type, "application/json");
		response_.body() = R"({"message": "REST API Server", "status": "running"})";
	} else if (request_.method() == http::verb::get && path == "/health") {
		response_.result(http::status::ok);
		response_.set(http::field::server, BOOST_BEAST_VERSION_STRING);
		response_.set(http::field::content_type, "application/json");
		response_.body() = R"({"status": "healthy"})";
	} else if (request_.method() == http::verb::get && path == "/api/version") {
		response_.result(http::status::ok);
		response_.set(http::field::server, BOOST_BEAST_VERSION_STRING);
		response_.set(http::field::content_type, "application/json");
		response_.body() = R"({"version": "1.0.0", "api": "v1"})";
	} else if (request_.method() == http::verb::get && path == "/api/objects/list") {
		// List all object IDs
		// Example: /api/objects/list?type=SkillData
		response_.set(http::field::server, BOOST_BEAST_VERSION_STRING);
		response_.set(http::field::content_type, "application/json");

		if (!object_manager_) {
			response_.result(http::status::service_unavailable);
			response_.body() = R"({"error": "Object manager not available"})";
		} else {
			try {
				std::ostringstream json;
				json << "{\"objects\": [";

				auto type_it = params.find("type");
				std::string type_filter = type_it != params.end() ? type_it->second : "";

				// Get all objects (you'll need to implement iterator/collection access in PersistentObjectManager)
				// TODO: This is a placeholder - adjust based on your actual API
				//auto objects = object_manager_->getAllObjectIds();

				//bool first = true;
				//for (const auto& id : objects) {
				//	if (!type_filter.empty() && id.find(type_filter) == std::string::npos) {
				//		continue;
				//	}

				//	if (!first)
				//		json << ", ";
				//	json << "\"" << escapeJson(id) << "\"";
				//	first = false;
				//}

				//json << "], \"count\": " << objects.size() << "}";

				response_.result(http::status::ok);
				response_.body() = json.str();
			} catch (const std::exception& e) {
				response_.result(http::status::internal_server_error);
				response_.body() = R"({"error": "Failed to retrieve objects", "message": ")" + escapeJson(e.what()) + R"("})";
			}
		}
	} else if (request_.method() == http::verb::get && path == "/api/objects/get") {
		// Get object by ID
		// Example: /api/objects/get?id=SKILL_ID_123
		response_.set(http::field::server, BOOST_BEAST_VERSION_STRING);
		response_.set(http::field::content_type, "application/json");

		if (!object_manager_) {
			response_.result(http::status::service_unavailable);
			response_.body() = R"({"error": "Object manager not available"})";
		} else {
			auto id_it = params.find("id");
			if (id_it == params.end()) {
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
	} else if (request_.method() == http::verb::get && path == "/api/objects/count") {
		// Get count of objects
		response_.set(http::field::server, BOOST_BEAST_VERSION_STRING);
		response_.set(http::field::content_type, "application/json");

		if (!object_manager_) {
			response_.result(http::status::service_unavailable);
			response_.body() = R"({"error": "Object manager not available"})";
		} else {
			try {
				// TODO: Implement actual count retrieval based on your PersistentObjectManager API
				//size_t count = object_manager_->size();
				size_t count = 0; // Placeholder

				std::ostringstream json;
				json << "{\"count\": " << count << "}";

				response_.result(http::status::ok);
				response_.body() = json.str();
			} catch (const std::exception& e) {
				response_.result(http::status::internal_server_error);
				response_.body() = R"({"error": "Failed to get count", "message": ")" + escapeJson(e.what()) + R"("})";
			}
		}
	} else if (request_.method() == http::verb::get && path == "/api/search") {
		// Legacy endpoint - kept for backward compatibility
		response_.result(http::status::ok);
		response_.set(http::field::server, BOOST_BEAST_VERSION_STRING);
		response_.set(http::field::content_type, "application/json");

		std::ostringstream json;
		json << "{\"results\": [";

		auto query_it = params.find("query");
		auto limit_it = params.find("limit");

		if (query_it != params.end()) {
			json << "{\"query\": \"" << escapeJson(query_it->second) << "\"";

			if (limit_it != params.end()) {
				json << ", \"limit\": " << limit_it->second;
			}

			json << ", \"message\": \"Search functionality\"}";
		} else {
			json << "{\"error\": \"Missing query parameter\"}";
		}

		json << "]}";
		response_.body() = json.str();
	} else if (request_.method() == http::verb::get && path == "/api/data") {
		// Legacy endpoint
		response_.result(http::status::ok);
		response_.set(http::field::server, BOOST_BEAST_VERSION_STRING);
		response_.set(http::field::content_type, "application/json");

		std::ostringstream json;
		json << "{";

		auto id_it = params.find("id");
		auto format_it = params.find("format");

		if (id_it != params.end()) {
			json << "\"id\": \"" << escapeJson(id_it->second) << "\"";

			if (format_it != params.end()) {
				json << ", \"format\": \"" << escapeJson(format_it->second) << "\"";
			}

			json << ", \"data\": \"Sample data for ID " << escapeJson(id_it->second) << "\"";
		} else {
			json << "\"error\": \"Missing id parameter\"";
		}

		json << "}";
		response_.body() = json.str();
	} else if (request_.method() == http::verb::get && path == "/api/echo") {
		// Echo endpoint for testing
		response_.result(http::status::ok);
		response_.set(http::field::server, BOOST_BEAST_VERSION_STRING);
		response_.set(http::field::content_type, "application/json");

		std::ostringstream json;
		json << "{\"query_params\": {";

		bool first = true;
		for (const auto& [key, value] : params) {
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