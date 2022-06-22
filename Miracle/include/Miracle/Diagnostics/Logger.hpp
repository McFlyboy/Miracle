#pragma once

#include <string>

namespace Miracle::Diagnostics {
	class Logger {
	public:
		Logger() = delete;

		static void initialize();

		static void info(const std::string_view& message);

		static void warning(const std::string_view& message);

		static void error(const std::string_view& message);
	};
}
