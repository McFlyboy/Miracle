#pragma once

#include <string_view>

namespace Miracle::Application {
	class ILogger {
	public:
		virtual ~ILogger() = default;

		virtual void info(const std::string_view& message) const = 0;

		virtual void warning(const std::string_view& message) const = 0;

		virtual void error(const std::string_view& message) const = 0;
	};
}
