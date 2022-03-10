#include <Miracle/components/Miracle/Diagnostics/Logger.hpp>

#include <spdlog/spdlog.h>

namespace Miracle::Diagnostics {
	void Logger::initialize() {
		// Pattern documentation:
		// https://github.com/gabime/spdlog/wiki/3.-Custom-formatting#pattern-flags
		spdlog::set_pattern("%^[%H:%M:%S] %7l: %v%$");
	}

	void Logger::info(const char* const message) {
		spdlog::info(message);
	}

	void Logger::warning(const char* const message) {
		spdlog::warn(message);
	}

	void Logger::error(const char* const message) {
		spdlog::error(message);
	}
}
