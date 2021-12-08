#pragma once

#include <string>

namespace Miracle::Diagnostics {
	class MIRACLE_API Logger {
	public:
		static void initialize();

		static void info(const char* const message);

		static void info(const std::string& message);

		static void warning(const char* const message);

		static void warning(const std::string& message);

		static void error(const char* const message);

		static void error(const std::string& message);
	};
}
