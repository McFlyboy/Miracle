#pragma once

#include "CameraProjectionType.hpp"

namespace Miracle {
	struct CameraConfig {
		CameraProjectionType projectionType = CameraProjectionType::orthographic;
		float zoomLevel = 1.0f;
		float fieldOfView = 70.0f;
	};
}
