#include <Miracle/Common/Transform.hpp>

namespace Miracle {
	Transform::Transform(
		const Vector3& translation,
		const Vector3& scale
	) :
		m_transformation(Matrix4::createScale(scale) * Matrix4::createTranslation(translation))
	{}
}
