#pragma once

#include <optional>
#include <string>

#include "WindowSize.hpp"

namespace Miracle {
	struct WindowConfig {
		std::optional<std::u8string> title = std::nullopt;
		WindowSize size = {
			.width = 640,
			.height = 480
		};
		bool resizable = false;
	};
}
