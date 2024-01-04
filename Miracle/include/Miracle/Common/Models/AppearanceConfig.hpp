#pragma once

#include <cstddef>

#include <Miracle/Common/Math/ColorRgb.hpp>

namespace Miracle {
	struct AppearanceConfig {
		bool visible = true;
		size_t meshIndex = 0;
		ColorRgb color = ColorRgb::createFromNonlinearSrgbColorCode(0xBFBFBF);
	};
}
