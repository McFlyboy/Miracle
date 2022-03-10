#pragma once

#include <string>

namespace Miracle::Diagnostics {
	class MIRACLE_API Logger {
	public:
		static void initialize();

		static void info(const char* const message);
		static inline void info(const std::string& message) {
			info(message.c_str());
		}

		static void warning(const char* const message);
		static inline void warning(const std::string& message) {
			warning(message.c_str());
		}

		static void error(const char* const message);
		static inline void error(const std::string& message) {
			error(message.c_str());
		}
	};
}
