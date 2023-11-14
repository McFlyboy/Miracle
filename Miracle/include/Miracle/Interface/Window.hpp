#pragma once

#include <string_view>

#include <Miracle/App.hpp>
#include <Miracle/Common/Models/WindowSize.hpp>

namespace Miracle {
	class Window {
	public:
		Window() = delete;

		static std::u8string_view getTitle() {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			return App::s_currentApp->m_dependencies->getWindow().getTitle();
		}

		static void setTitle(const std::u8string_view& title) {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			App::s_currentApp->m_dependencies->getWindow().setTitle(title);
		}

		static bool isResizable() {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			return App::s_currentApp->m_dependencies->getWindow().isResizable();
		}

		static void setResizable(bool resizable) {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			App::s_currentApp->m_dependencies->getWindow().setResizable(resizable);
		}

		static WindowSize getSize() {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			return App::s_currentApp->m_dependencies->getWindow().getSize();
		}

		static void setSize(WindowSize size) {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			App::s_currentApp->m_dependencies->getWindow().setSize(size);
		}
	};
}
