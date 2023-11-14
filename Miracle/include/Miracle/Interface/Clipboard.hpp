#pragma once

#include <string_view>
#include <optional>

#include <Miracle/App.hpp>

namespace Miracle {
	class Clipboard {
	public:
		Clipboard() = delete;

		static std::optional<std::u8string_view> getClipboardContent() {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			return App::s_currentApp->m_dependencies->getMultimediaFramework().getClipboardContent();
		}

		static void setClipboardContent(const std::u8string_view& content) {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			App::s_currentApp->m_dependencies->getMultimediaFramework().setClipboardContent(content);
		}
	};
}
