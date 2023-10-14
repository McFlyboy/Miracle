#pragma once

#include <string>
#include <chrono>

#include <Miracle/App.hpp>
#include <Miracle/Common/Random.hpp>

namespace Miracle {
	class CurrentApp {
	public:
		CurrentApp() = delete;

		static const std::string& getName() {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			return App::s_currentApp->getName();
		}

		static const UserData& getUserData() {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			return App::s_currentApp->getUserData();
		}

		static void setUserData(const UserData& userData) {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			App::s_currentApp->setUserData(userData);
		}

		static void close(int exitCode = 0) {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			App::s_currentApp->m_running = false;
			App::s_currentApp->m_exitCode = exitCode;
		}

		static std::chrono::duration<double> getRuntimeDuration() {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			return App::s_currentApp->m_dependencies->getMultimediaFramework()
				.getDurationSinceInitialization();
		}

		static Random& getRandom() {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			return App::s_currentApp->m_dependencies->getRandom();
		}
	};
}
