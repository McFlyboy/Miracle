#pragma once

#include <string_view>

#include <Miracle/App.hpp>

namespace Miracle {
	class Logger {
	public:
		Logger() = delete;

		static void info(const std::string_view& message) {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			App::s_currentApp->m_logger->info(message);
		}

		static void warning(const std::string_view& message) {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			App::s_currentApp->m_logger->warning(message);
		}

		static void error(const std::string_view& message) {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			App::s_currentApp->m_logger->error(message);
		}
	};
}
