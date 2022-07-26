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
				.title  = windowConfig.title.has_value() ? windowConfig.title.value() : defaultTitle,
				.width  = windowConfig.width,
				.height = windowConfig.height
			};
		}

		static inline std::u8string_view toU8StringView(const std::string_view& stringView) {
			return std::u8string_view(
				reinterpret_cast<const char8_t*>(stringView.data()),
				stringView.size()
			);
		}
	};
}
