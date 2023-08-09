#pragma once

#include <cstddef>

#include <Miracle/Common/Math/ColorRgb.hpp>

namespace Miracle {
	struct AppearanceConfig {
		bool visible = true;
		size_t meshIndex = 0;
		ColorRgb color = ColorRgb{
			.redChannel   = 0.75f,
			.greenChannel = 0.75f,
			.blueChannel  = 0.75f
		};
	};
}
