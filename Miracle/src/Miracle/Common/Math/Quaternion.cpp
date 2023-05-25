#include <Miracle/Common/Math/Quaternion.hpp>

namespace Miracle {
	const Quaternion Quaternion::identity = Quaternion{ .w = 1.0f, .v = Vector3{} };
}
