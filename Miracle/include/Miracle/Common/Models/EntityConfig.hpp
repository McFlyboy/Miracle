#pragma once

#include <optional>

#include <Miracle/Common/BehaviourFactory.hpp>
#include "TransformConfig.hpp"
#include "AppearanceConfig.hpp"

namespace Miracle {
	struct EntityConfig {
		TransformConfig transformConfig = {};
		std::optional<AppearanceConfig> appearanceConfig = {};
		std::optional<BehaviourFactory> behaviourFactory = {};
	};
}
