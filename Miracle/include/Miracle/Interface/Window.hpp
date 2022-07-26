#pragma once

#include <string_view>

#include <Miracle/App.hpp>

namespace Miracle {
	class Window {
	public:
		Window() = delete;

		static inline void setTitle(const std::string_view& title) {
			if (App::s_currentApp == nullptr) [[unlikely]] return;

			App::s_currentApp->m_dependencies->getWindow().setTitle(title);
		}
	};
}
