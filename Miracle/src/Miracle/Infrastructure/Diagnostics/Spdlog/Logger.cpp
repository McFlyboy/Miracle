#include "Logger.hpp"

#include <spdlog/spdlog.h>

namespace Miracle::Infrastructure::Diagnostics::Spdlog {
	Logger::Logger() {
		// Pattern documentation:
		// https://github.com/gabime/spdlog/wiki/3.-Custom-formatting#pattern-flags
		spdlog::set_pattern("%^[%H:%M:%S] %7l: %v%$");
	}

	Logger::~Logger() {
		spdlog::shutdown();
	}

	void Logger::info(const std::string_view& message) const {
		spdlog::info(message);
	}

	void Logger::warning(const std::string_view& message) const {
		spdlog::warn(message);
	}

	void Logger::error(const std::string_view& message) const {
		spdlog::error(message);
	}
}
