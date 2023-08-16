#pragma once

#include <optional>

#include <Miracle/Common/BehaviorFactory.hpp>
#include "TransformConfig.hpp"
#include "CameraConfig.hpp"
#include "AppearanceConfig.hpp"

namespace Miracle {
	struct EntityConfig {
		TransformConfig transformConfig = {};
		std::optional<CameraConfig> cameraConfig = {};
		std::optional<AppearanceConfig> appearanceConfig = {};
		std::optional<BehaviorFactory> behaviorFactory = {};
	};
}
