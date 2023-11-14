#pragma once

#include <Miracle/Common/Math/Matrix4.hpp>
#include <Miracle/Common/Math/ColorRgb.hpp>

namespace Miracle::Application {
	struct VertexStagePushConstants {
		Matrix4 transform = Matrix4s::identity;
	};

	struct FragmentStagePushConstants {
		ColorRgb color = {};
	};

	struct PushConstants {
		VertexStagePushConstants vertexStageConstants = {};
		FragmentStagePushConstants fragmentStageConstants = {};
	};
}
