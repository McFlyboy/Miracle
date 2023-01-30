#pragma once

#include <vector>

#include <Miracle/Common/Math/Color3f.hpp>
#include "EntityConfig.hpp"

namespace Miracle {
	struct SceneConfig {
		Color3f backgroundColor = {
			.red   = 0.125f,
			.green = 0.125f,
			.blue  = 0.125f
		};
		std::vector<EntityConfig> entityConfigs = {};
	};
}
