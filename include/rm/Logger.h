#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <memory>

namespace rm::util {

/**
 * @class Logger
 * @brief Singleton logger configuration for the application
 */
class Logger {
public:
	/**
	 * @brief Initialize the logger with console and file sinks
	 * @param log_file Path to the log file
	 * @param level Logging level (trace, debug, info, warn, error, critical)
	 */
	static void init(const std::string& log_file = "rmce.log", spdlog::level::level_enum level = spdlog::level::info) {
		try {
			// Create console sink with color
			auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
			console_sink->set_level(level);
			console_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%n] %v");

			// Create rotating file sink (10MB max, 3 files)
			auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(log_file, 1024 * 1024 * 10, 3);
			file_sink->set_level(spdlog::level::trace);
			file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%n] [%s:%#] %v");

			// Create logger with both sinks
			std::vector<spdlog::sink_ptr> sinks{console_sink, file_sink};
			auto logger = std::make_shared<spdlog::logger>("rmce", sinks.begin(), sinks.end());
			logger->set_level(level);
			logger->flush_on(spdlog::level::warn); // Auto-flush on warnings and above

			// Set as default logger
			spdlog::set_default_logger(logger);
			spdlog::set_level(level);

			SPDLOG_INFO("Logger initialized successfully");
		} catch (const spdlog::spdlog_ex& ex) {
			std::cerr << "Log initialization failed: " << ex.what() << std::endl;
		}
	}

	/**
	 * @brief Get the default logger
	 */
	static std::shared_ptr<spdlog::logger> get() {
		return spdlog::default_logger();
	}

	/**
	 * @brief Shutdown the logger (call before exit)
	 */
	static void shutdown() {
		spdlog::shutdown();
	}
};

} // namespace rm::util

// Convenience macros for logging
#define LOG_TRACE(...) SPDLOG_TRACE(__VA_ARGS__)
#define LOG_DEBUG(...) SPDLOG_DEBUG(__VA_ARGS__)
#define LOG_INFO(...) SPDLOG_INFO(__VA_ARGS__)
#define LOG_WARN(...) SPDLOG_WARN(__VA_ARGS__)
#define LOG_ERROR(...) SPDLOG_ERROR(__VA_ARGS__)
#define LOG_CRITICAL(...) SPDLOG_CRITICAL(__VA_ARGS__)