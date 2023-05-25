#pragma once

#include <vector>

#include <Miracle/Common/Math/ColorRgb.hpp>
#include "EntityConfig.hpp"

namespace Miracle {
	struct SceneConfig {
		ColorRgb backgroundColor = {
			.redChannel   = 0.125f,
			.greenChannel = 0.125f,
			.blueChannel  = 0.125f
		};
		std::vector<EntityConfig> entityConfigs = {};
	};
}
