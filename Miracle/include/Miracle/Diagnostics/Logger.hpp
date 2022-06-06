#pragma once

#include <string>

#include <Miracle/Definitions.hpp>

namespace Miracle::Diagnostics {
	class MIRACLE_API Logger {
	public:
		Logger() = delete;

		static void initialize();

		static void info(const std::string_view& message);

		static void warning(const std::string_view& message);

		static void error(const std::string_view& message);
	};
}
