#pragma once

#include <optional>

#include <Miracle/Common/BehaviourFactory.hpp>
#include "TransformConfig.hpp"

namespace Miracle {
	struct EntityConfig {
		TransformConfig transformConfig = {};
		std::optional<BehaviourFactory> behaviourFactory = {};
	};
}
