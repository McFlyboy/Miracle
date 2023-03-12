#include <Miracle/Common/Math/Matrix4.hpp>

namespace Miracle {
	const Matrix4 Matrix4::identity = Matrix4{
		.m11 = 1.0f, .m12 = 0.0f, .m13 = 0.0f, .m14 = 0.0f,
		.m21 = 0.0f, .m22 = 1.0f, .m23 = 0.0f, .m24 = 0.0f,
		.m31 = 0.0f, .m32 = 0.0f, .m33 = 1.0f, .m34 = 0.0f,
		.m41 = 0.0f, .m42 = 0.0f, .m43 = 0.0f, .m44 = 1.0f
	};
}
