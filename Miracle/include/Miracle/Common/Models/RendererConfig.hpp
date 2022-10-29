#pragma once

#include <Miracle/Common/Math/Color3f.hpp>

namespace Miracle {
	struct RendererConfig {
		Color3f clearColor = Color3f{
			.red   = 0.125f,
			.green = 0.125f,
			.blue  = 0.125f
		};
	};
}
