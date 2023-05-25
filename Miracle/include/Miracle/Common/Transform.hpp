#pragma once

#include <cstdint>

#include "Math/Vector3.hpp"
#include "Math/Matrix4.hpp"
#include "Math/Quaternion.hpp"

namespace Miracle {
	enum class TransformSpace : uint8_t {
		local,
		scene
	};

	class Transform {
	private:
		Vector3 m_translation;
		Quaternion m_rotation;
		Vector3 m_scale;

		mutable Matrix4 m_cachedTransformation;
		mutable bool m_cacheOutdated = false;

	public:
		Transform(
			const Vector3& translation,
			const Quaternion& rotation,
			const Vector3& scale
		);

		inline const Vector3& getTranslation() const { return m_translation; }

		inline void setTranslation(const Vector3& translation) {
			m_translation = translation;
			m_cacheOutdated = true;
		}

		void translate(const Vector3& deltaTranslation, TransformSpace transformSpace = TransformSpace::local);

		inline const Quaternion& getRotation() const { return m_rotation; }

		inline void setRotation(const Quaternion& rotation) {
			m_rotation = rotation;
			m_cacheOutdated = true;
		}

		void rotate(const Quaternion& deltaRotation, TransformSpace transformSpace = TransformSpace::local);

		inline const Vector3& getScale() const { return m_scale; }

		inline void setScale(const Vector3& scale) {
			m_scale = scale;
			m_cacheOutdated = true;
		}

		void scale(const Vector3& deltaScale);

		const Matrix4& getTransformation() const;

	private:
		Matrix4 createTransformation() const;
	};
}
