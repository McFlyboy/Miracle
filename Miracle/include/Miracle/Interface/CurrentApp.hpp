#pragma once

#include <string_view>
#include <optional>

#include <Miracle/App.hpp>

namespace Miracle {
	class CurrentApp {
	public:
		CurrentApp() = delete;

		static inline std::string_view getName() {
			if (App::s_currentApp == nullptr) [[unlikely]] return "";

			return App::s_currentApp->getName();
		}

		static inline const UserData& getUserData() {
			if (App::s_currentApp == nullptr) [[unlikely]] return {};

			return App::s_currentApp->getUserData();
		}

		static inline void setUserData(const UserData& userData) {
			if (App::s_currentApp == nullptr) [[unlikely]] return;

			App::s_currentApp->setUserData(userData);
		}

		static inline void close(int exitCode = 0) {
			if (App::s_currentApp == nullptr) [[unlikely]] return;

			App::s_currentApp->m_running = false;
			App::s_currentApp->m_exitCode = exitCode;
		}

		static inline std::optional<std::u8string_view> getClipboardContent() {
			if (App::s_currentApp == nullptr) [[unlikely]] return u8"";

			return App::s_currentApp->m_dependencies->getMultimediaFramework().getClipboardContent();
		}

		static inline void setClipboardContent(const std::u8string_view& content) {
			if (App::s_currentApp == nullptr) [[unlikely]] return;

			App::s_currentApp->m_dependencies->getMultimediaFramework().setClipboardContent(content);
		}
	};
}
