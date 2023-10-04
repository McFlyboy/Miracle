#pragma once

#include <optional>
#include <variant>

#include <Miracle/Common/BehaviorFactory.hpp>
#include "TransformConfig.hpp"
#include "OrthographicCameraConfig.hpp"
#include "PerspectiveCameraConfig.hpp"
#include "AppearanceConfig.hpp"

namespace Miracle {
	struct EntityConfig {
		TransformConfig transformConfig = {};
		std::optional<std::variant<OrthographicCameraConfig, PerspectiveCameraConfig>> cameraConfig = {};
		std::optional<AppearanceConfig> appearanceConfig = {};
		std::optional<BehaviorFactory> behaviorFactory = {};
	};
}
