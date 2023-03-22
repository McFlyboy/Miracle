#pragma once

#include <Miracle/Common/Math/Vector3.hpp>

namespace Miracle {
	struct TransformConfig {
		Vector3 translation = {};
		Vector3 scale = Vector3{ .x = 1.0f, .y = 1.0f, .z = 1.0f };
	};
}
