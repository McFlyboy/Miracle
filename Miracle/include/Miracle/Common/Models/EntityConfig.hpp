#pragma once

#include <optional>

#include <Miracle/Common/Math/Vector3.hpp>
#include <Miracle/Common/BehaviourFactory.hpp>

namespace Miracle {
	struct EntityConfig {
		Vector3 position = {};
		std::optional<BehaviourFactory> behaviourFactory = {};
	};
}
