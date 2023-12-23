#pragma once

#include <vector>
#include <functional>

#include <Miracle/Common/Math/ColorRgb.hpp>
#include <Miracle/Common/Models/EntityId.hpp>
#include "EntityConfig.hpp"

namespace Miracle {
	struct SceneConfig {
		ColorRgb backgroundColor = ColorRgb::createFromNonlinearSrgbColorCode(0x202020);
		std::vector<EntityConfig> entityConfigs = {};
		std::function<void(EntityId)> entityCreatedCallback = [](EntityId) {};
		std::function<void(EntityId)> entityDestroyedCallback = [](EntityId) {};
	};
}
