#pragma once

#include <Miracle/Common/Math/Vector3.hpp>

namespace Miracle::Application {
	struct PushConstants {
		Vector3 translation = {};
		float aspectRatio = 1.0f;
	};
}
