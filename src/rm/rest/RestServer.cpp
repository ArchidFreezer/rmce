#include "RestServer.h"
#include <iostream>
#include <boost/asio/dispatch.hpp>

namespace rm::rest {

// Session Implementation
Session::Session(tcp::socket socket) : stream_(std::move(socket)) {
}

void Session::run() {
	// Set the timeout for read operations
	stream_.expires_after(std::chrono::seconds(30));
	doRead();
}

void Session::doRead() {
	request_ = {};

	http::async_read(stream_, buffer_, request_, beast::bind_front_handler(&Session::onRead, shared_from_this()));
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

	// Route handling
	if (request_.method() == http::verb::get && request_.target() == "/") {
		response_.result(http::status::ok);
		response_.set(http::field::server, BOOST_BEAST_VERSION_STRING);
		response_.set(http::field::content_type, "application/json");
		response_.body() = R"({"message": "REST API Server", "status": "running"})";
	} else if (request_.method() == http::verb::get && request_.target() == "/health") {
		response_.result(http::status::ok);
		response_.set(http::field::server, BOOST_BEAST_VERSION_STRING);
		response_.set(http::field::content_type, "application/json");
		response_.body() = R"({"status": "healthy"})";
	} else if (request_.method() == http::verb::get && request_.target() == "/api/version") {
		response_.result(http::status::ok);
		response_.set(http::field::server, BOOST_BEAST_VERSION_STRING);
		response_.set(http::field::content_type, "application/json");
		response_.body() = R"({"version": "1.0.0", "api": "v1"})";
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
	http::async_write(stream_, response_, beast::bind_front_handler(&Session::onWrite, shared_from_this()));
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
RestServer::RestServer(const std::string& address, unsigned short port, int num_threads) : ioc_(num_threads), acceptor_(net::make_strand(ioc_)), running_(false), num_threads_(num_threads) {
	beast::error_code ec;

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
	acceptor_.async_accept(net::make_strand(ioc_), beast::bind_front_handler(&RestServer::onAccept, this));
}

void RestServer::onAccept(beast::error_code ec, tcp::socket socket) {
	if (ec) {
		if (ec != net::error::operation_aborted) {
			std::cerr << "Accept error: " << ec.message() << std::endl;
		}
	} else {
		std::make_shared<Session>(std::move(socket))->run();
	}

	if (running_) {
		doAccept();
	}
}

} // namespace rm::rest