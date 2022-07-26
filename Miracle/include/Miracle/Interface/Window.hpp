#pragma once

#include <string_view>

#include <Miracle/App.hpp>

namespace Miracle {
	class Window {
	public:
		Window() = delete;

		static inline std::u8string_view getTitle() {
			if (App::s_currentApp == nullptr) [[unlikely]] return u8"";

			return App::s_currentApp->m_dependencies->getWindow().getTitle();
		}

		static inline void setTitle(const std::u8string_view& title) {
			if (App::s_currentApp == nullptr) [[unlikely]] return;

			App::s_currentApp->m_dependencies->getWindow().setTitle(title);
		}
	};
}
