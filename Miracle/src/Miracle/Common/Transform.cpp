#include <Miracle/Common/Transform.hpp>

namespace Miracle {
	Transform::Transform(
		const Vector3& translation,
		const Quaternion& rotation,
		const Vector3& scale
	) :
		m_translation(translation),
		m_rotation(rotation),
		m_scale(scale)
	{}

	Matrix4 Transform::getTransformation() const {
		return Matrix4::createScale(m_scale)
			* Matrix4::createRotation(m_rotation)
			* Matrix4::createTranslation(m_translation);
	}
}
