#pragma once

#include <optional>

#include <Miracle/Common/Math/Color3f.hpp>
#include <Miracle/Common/Math/Vector2f.hpp>
#include <Miracle/Common/BehaviourFactory.hpp>

namespace Miracle {
	struct SceneConfig {
		Color3f backgroundColor = {
			.red   = 0.125f,
			.green = 0.125f,
			.blue  = 0.125f
		};
		Vector2f entityPosition = {};
		std::optional<BehaviourFactory> entityBehaviourFactory = {};
	};
}
