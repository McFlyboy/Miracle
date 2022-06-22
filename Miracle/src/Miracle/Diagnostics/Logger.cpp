#include <Miracle/Diagnostics/Logger.hpp>

#include <spdlog/spdlog.h>

namespace Miracle::Diagnostics {
	void Logger::initialize() {
		// Pattern documentation:
		// https://github.com/gabime/spdlog/wiki/3.-Custom-formatting#pattern-flags
		spdlog::set_pattern("%^[%H:%M:%S] %7l: %v%$");
	}

	void Logger::info(const std::string_view& message) {
		spdlog::info(message);
	}

	void Logger::warning(const std::string_view& message) {
		spdlog::warn(message);
	}

	void Logger::error(const std::string_view& message) {
		spdlog::error(message);
	}
}
