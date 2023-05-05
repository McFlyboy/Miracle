#pragma once

#include "Math/Vector3.hpp"
#include "Math/Matrix4.hpp"
#include "Math/Quaternion.hpp"

namespace Miracle {
	class Transform {
	private:
		Vector3 m_translation;
		Quaternion m_rotation;
		Vector3 m_scale;

	public:
		Transform(
			const Vector3& translation,
			const Quaternion& rotation,
			const Vector3& scale
		);

		inline const Vector3& getTranslation() const { return m_translation; }

		inline Vector3& getTranslation() { return m_translation; }

		inline const Quaternion& getRotation() const { return m_rotation; }

		inline Quaternion& getRotation() { return m_rotation; }

		inline const Vector3& getScale() const { return m_scale; }

		inline Vector3& getScale() { return m_scale; }

		Matrix4 getTransformation() const;
	};
}
