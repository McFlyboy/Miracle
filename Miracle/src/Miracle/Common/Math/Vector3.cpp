#include <Miracle/Common/Math/Vector3.hpp>

namespace Miracle {
	const Vector3 Vector3::right   = Vector3{ .x = 1.0f, .y = 0.0f, .z = 0.0f };
	const Vector3 Vector3::up      = Vector3{ .x = 0.0f, .y = 1.0f, .z = 0.0f };
	const Vector3 Vector3::forward = Vector3{ .x = 0.0f, .y = 0.0f, .z = 1.0f };
}
