#pragma once

#include <string_view>

#include <Miracle/App.hpp>
#include <Miracle/Common/Models/WindowSize.hpp>

namespace Miracle {
	class Window {
	public:
		Window() = delete;

		static inline std::u8string_view getTitle() {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			return App::s_currentApp->m_dependencies->getWindow().getTitle();
		}

		static inline void setTitle(const std::u8string_view& title) {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			App::s_currentApp->m_dependencies->getWindow().setTitle(title);
		}

		static inline bool isResizable() {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			return App::s_currentApp->m_dependencies->getWindow().isResizable();
		}

		static inline void setResizable(bool resizable) {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			App::s_currentApp->m_dependencies->getWindow().setResizable(resizable);
		}

		static inline WindowSize getSize() {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			return App::s_currentApp->m_dependencies->getWindow().getSize();
		}

		static inline void setSize(WindowSize size) {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			App::s_currentApp->m_dependencies->getWindow().setSize(size);
		}
	};
}
