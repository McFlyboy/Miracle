#pragma once

#include <string_view>

#include <Miracle/Application/IWindow.hpp>
#include "Models/WindowConfig.hpp"

namespace Miracle {
	class Mappings {
	public:
		Mappings() = delete;

		static inline Application::WindowInitProps toWindowInitProps(
			const WindowConfig& windowConfig,
			const std::u8string_view& defaultTitle
		) {
			return Application::WindowInitProps{
				.title     = windowConfig.title.has_value() ? windowConfig.title.value() : defaultTitle,
				.width     = windowConfig.width,
				.height    = windowConfig.height,
				.resizable = windowConfig.resizable
			};
		}
	};
}
