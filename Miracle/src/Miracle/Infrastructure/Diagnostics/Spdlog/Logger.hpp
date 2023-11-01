#pragma once

#include <Miracle/Application/ILogger.hpp>

namespace Miracle::Infrastructure::Diagnostics::Spdlog {
	class Logger : public Application::ILogger {
	public:
		Logger();

		~Logger();

		virtual void info(const std::string_view& message) const override;

		virtual void warning(const std::string_view& message) const override;

		virtual void error(const std::string_view& message) const override;
	};
}
