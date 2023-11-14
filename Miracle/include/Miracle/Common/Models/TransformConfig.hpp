#pragma once

#include <Miracle/Common/Math/Vector3.hpp>
#include <Miracle/Common/Math/Quaternion.hpp>

namespace Miracle {
	struct TransformConfig {
		Vector3 translation = {};
		Quaternion rotation = Quaternions::identity;
		Vector3 scale = Vector3{ .x = 1.0f, .y = 1.0f, .z = 1.0f };
	};
}
