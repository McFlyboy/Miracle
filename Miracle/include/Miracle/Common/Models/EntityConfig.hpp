#pragma once

#include <optional>

#include <Miracle/Common/Math/Vector2f.hpp>
#include <Miracle/Common/BehaviourFactory.hpp>

namespace Miracle {
	struct EntityConfig {
		Vector2f position = {};
		std::optional<BehaviourFactory> behaviourFactory = {};
	};
}
