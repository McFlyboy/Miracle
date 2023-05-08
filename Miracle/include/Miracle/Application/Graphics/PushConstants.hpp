#pragma once

#include <Miracle/Common/Math/Matrix4.hpp>

namespace Miracle::Application {
	struct PushConstants {
		Matrix4 transform = Matrix4::identity;
	};
}
