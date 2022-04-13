#pragma once

#include <Miracle/components/Miracle/Math/Vector2f.hpp>
#include <Miracle/components/Miracle/Math/Vector3f.hpp>

namespace Miracle::Graphics {
	struct Vertex {
		Math::Vector2f position;
		Math::Vector3f color;
	};
}
