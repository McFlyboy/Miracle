#pragma once

#include <variant>

#include <Miracle/Common/Math/Angle.hpp>

namespace Miracle {
	struct PerspectiveCameraConfig {
		std::variant<Degrees, Radians> fieldOfView = 75.0_deg;
		float nearClipPlaneDistance = 0.1f;
		float farClipPlaneDistance = 1000.0f;
	};
}
