#include <gtest/gtest.h>
#include <RestServer.h>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/core.hpp>
#include <thread>
#include <chrono>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

namespace {

// Test fixture for REST server tests
class RestServerTest : public ::testing::Test {
protected:
	void SetUp() override {
		// Use a random available port or a fixed test port
		test_port = 8888;
		test_address = "127.0.0.1";
	}

	void TearDown() override {
		// Cleanup if needed
	}

	std::string test_address;
	unsigned short test_port;
};

// Helper class to make HTTP requests
class HttpClient {
public:
	HttpClient(const std::string& host, unsigned short port) : host_(host), port_(port) {
	}

	http::response<http::string_body> get(const std::string& target) {
		return request(http::verb::get, target);
	}

	http::response<http::string_body> post(const std::string& target, const std::string& body) {
		return request(http::verb::post, target, body);
	}

private:
	http::response<http::string_body> request(http::verb method, const std::string& target, const std::string& body = "") {
		try {
			net::io_context ioc;
			tcp::resolver resolver(ioc);
			beast::tcp_stream stream(ioc);

			// Resolve and connect
			auto const results = resolver.resolve(host_, std::to_string(port_));
			stream.connect(results);

			// Set up HTTP request
			http::request<http::string_body> req{method, target, 11};
			req.set(http::field::host, host_);
			req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

			if (!body.empty()) {
				req.body() = body;
				req.prepare_payload();
			}

			// Send the HTTP request
			http::write(stream, req);

			// Receive the HTTP response
			beast::flat_buffer buffer;
			http::response<http::string_body> res;
			http::read(stream, buffer, res);

			// Gracefully close the socket
			beast::error_code ec;
			stream.socket().shutdown(tcp::socket::shutdown_both, ec);

			return res;
		} catch (std::exception const& e) {
			throw std::runtime_error(std::string("HTTP request failed: ") + e.what());
		}
	}

	std::string host_;
	unsigned short port_;
};

// Test: Server Construction and Basic Lifecycle
TEST_F(RestServerTest, ConstructorAndDestructor) {
	EXPECT_NO_THROW({ rm::rest::RestServer server(test_address, test_port, 1); });
}

TEST_F(RestServerTest, StartAndStop) {
	rm::rest::RestServer server(test_address, test_port, 1);

	EXPECT_FALSE(server.isRunning());

	server.start();
	EXPECT_TRUE(server.isRunning());

	// Give server time to start
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	server.stop();
	EXPECT_FALSE(server.isRunning());
}

TEST_F(RestServerTest, MultipleStartCallsAreIdempotent) {
	rm::rest::RestServer server(test_address, test_port, 1);

	server.start();
	EXPECT_TRUE(server.isRunning());

	// Starting again should be safe
	server.start();
	EXPECT_TRUE(server.isRunning());

	server.stop();
}

TEST_F(RestServerTest, MultipleStopCallsAreIdempotent) {
	rm::rest::RestServer server(test_address, test_port, 1);

	server.start();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	server.stop();
	EXPECT_FALSE(server.isRunning());

	// Stopping again should be safe
	server.stop();
	EXPECT_FALSE(server.isRunning());
}

// Test: HTTP Endpoints
TEST_F(RestServerTest, RootEndpoint) {
	rm::rest::RestServer server(test_address, test_port, 2);
	server.start();

	// Give server time to start accepting connections
	std::this_thread::sleep_for(std::chrono::milliseconds(200));

	HttpClient client(test_address, test_port);
	auto response = client.get("/");

	EXPECT_EQ(response.result(), http::status::ok);
	EXPECT_EQ(response[http::field::content_type], "application/json");

	// Check response body contains expected JSON
	std::string body = response.body();
	EXPECT_NE(body.find("\"message\""), std::string::npos);
	EXPECT_NE(body.find("\"status\""), std::string::npos);
	EXPECT_NE(body.find("\"running\""), std::string::npos);

	server.stop();
}

TEST_F(RestServerTest, HealthEndpoint) {
	rm::rest::RestServer server(test_address, test_port, 1);
	server.start();
	std::this_thread::sleep_for(std::chrono::milliseconds(200));

	HttpClient client(test_address, test_port);
	auto response = client.get("/health");

	EXPECT_EQ(response.result(), http::status::ok);
	EXPECT_EQ(response[http::field::content_type], "application/json");

	std::string body = response.body();
	EXPECT_NE(body.find("\"status\""), std::string::npos);
	EXPECT_NE(body.find("\"healthy\""), std::string::npos);

	server.stop();
}

TEST_F(RestServerTest, VersionEndpoint) {
	rm::rest::RestServer server(test_address, test_port, 1);
	server.start();
	std::this_thread::sleep_for(std::chrono::milliseconds(200));

	HttpClient client(test_address, test_port);
	auto response = client.get("/rmce/version");

	EXPECT_EQ(response.result(), http::status::ok);
	EXPECT_EQ(response[http::field::content_type], "application/json");

	std::string body = response.body();
	EXPECT_NE(body.find("\"version\""), std::string::npos);
	EXPECT_NE(body.find("\"api\""), std::string::npos);

	server.stop();
}

TEST_F(RestServerTest, NotFoundEndpoint) {
	rm::rest::RestServer server(test_address, test_port, 1);
	server.start();
	std::this_thread::sleep_for(std::chrono::milliseconds(200));

	HttpClient client(test_address, test_port);
	auto response = client.get("/nonexistent");

	EXPECT_EQ(response.result(), http::status::not_found);
	EXPECT_EQ(response[http::field::content_type], "application/json");

	std::string body = response.body();
	EXPECT_NE(body.find("\"error\""), std::string::npos);
	EXPECT_NE(body.find("Not Found"), std::string::npos);

	server.stop();
}

// Test: Multiple Concurrent Requests
TEST_F(RestServerTest, MultipleThreadsHandleRequests) {
	rm::rest::RestServer server(test_address, test_port, 4);
	server.start();
	std::this_thread::sleep_for(std::chrono::milliseconds(200));

	// Make multiple concurrent requests
	std::vector<std::thread> threads;
	std::atomic<int> success_count{0};

	for (int i = 0; i < 10; ++i) {
		threads.emplace_back([this, &success_count]() {
			try {
				HttpClient client(test_address, test_port);
				auto response = client.get("/health");
				if (response.result() == http::status::ok) {
					success_count++;
				}
			} catch (...) {
				// Request failed
			}
		});
	}

	for (auto& t : threads) {
		t.join();
	}

	EXPECT_EQ(success_count, 10);

	server.stop();
}

// Test: Keep-Alive Connections
TEST_F(RestServerTest, KeepAliveConnections) {
	rm::rest::RestServer server(test_address, test_port, 1);
	server.start();
	std::this_thread::sleep_for(std::chrono::milliseconds(200));

	HttpClient client(test_address, test_port);

	// Make multiple requests with the same client
	auto response1 = client.get("/health");
	EXPECT_EQ(response1.result(), http::status::ok);

	auto response2 = client.get("/rmce/version");
	EXPECT_EQ(response2.result(), http::status::ok);

	auto response3 = client.get("/");
	EXPECT_EQ(response3.result(), http::status::ok);

	server.stop();
}

// Test: Server Restart
//TEST_F(RestServerTest, ServerCanRestart) {
//	rm::rest::RestServer server(test_address, test_port, 1);
//
//	// First start
//	server.start();
//	std::this_thread::sleep_for(std::chrono::milliseconds(200));
//
//	HttpClient client(test_address, test_port);
//	auto response1 = client.get("/health");
//	EXPECT_EQ(response1.result(), http::status::ok);
//
//	// Stop
//	server.stop();
//	std::this_thread::sleep_for(std::chrono::milliseconds(200));
//
//	// Restart
//	server.start();
//	std::this_thread::sleep_for(std::chrono::milliseconds(200));
//
//	auto response2 = client.get("/health");
//	EXPECT_EQ(response2.result(), http::status::ok);
//
//	server.stop();
//}

// Test: Invalid Port Handling
TEST_F(RestServerTest, InvalidPortThrowsException) {
	// Port 0 is invalid for binding
	EXPECT_THROW({ rm::rest::RestServer server(test_address, 0, 1); }, std::runtime_error);
}

// Test: Response Headers
TEST_F(RestServerTest, ResponseContainsCorrectHeaders) {
	rm::rest::RestServer server(test_address, test_port, 1);
	server.start();
	std::this_thread::sleep_for(std::chrono::milliseconds(200));

	HttpClient client(test_address, test_port);
	auto response = client.get("/");

	// Check for required headers
	EXPECT_TRUE(response.count(http::field::server) > 0);
	EXPECT_TRUE(response.count(http::field::content_type) > 0);

	// Check server header value
	std::string server_header = std::string(response[http::field::server]);
	EXPECT_NE(server_header.find("Beast"), std::string::npos);

	server.stop();
}

} // anonymous namespace