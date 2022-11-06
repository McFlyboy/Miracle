#pragma once

#include <Miracle/Common/Math/Vector2f.hpp>

namespace Miracle::Application {
	struct PushConstants {
		Vector2f translation = {};
		float aspectRatio = 1.0f;
	};
}
