#include <Miracle/Common/Components/Transform.hpp>

#include <Miracle/Common/Math/MathUtilities.hpp>

namespace Miracle {
	Transform::Transform(
		const Vector3& translation,
		const Quaternion& rotation,
		const Vector3& scale
	) :
		m_translation(translation),
		m_rotation(rotation),
		m_scale(scale),
		m_cachedTransformation(createTransformation())
	{}

	void Transform::translate(const Vector3& deltaTranslation, TransformSpace transformSpace) {
		m_translation += transformSpace == TransformSpace::local
			? MathUtilities::rotateVector(deltaTranslation, m_rotation)
			: deltaTranslation;

		m_cacheOutdated = true;
	}

	void Transform::rotate(const Quaternion& deltaRotation, TransformSpace transformSpace) {
		m_rotation = transformSpace == TransformSpace::local
			? m_rotation * deltaRotation
			: deltaRotation * m_rotation;

		m_cacheOutdated = true;
	}

	void Transform::scale(const Vector3& deltaScale) {
		m_scale += deltaScale;
		m_cacheOutdated = true;
	}

	void Transform::scale(float scalar) {
		m_scale *= scalar;
		m_cacheOutdated = true;
	}

	const Matrix4& Transform::getTransformation() const {
		if (m_cacheOutdated) {
			m_cachedTransformation = createTransformation();
			m_cacheOutdated = false;
		}

		return m_cachedTransformation;
	}

	Matrix4 Transform::createTransformation() const {
		return Matrix4::createTransformation(m_translation, m_rotation, m_scale);
	}
}
