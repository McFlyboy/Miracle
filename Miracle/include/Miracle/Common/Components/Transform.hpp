#pragma once

#include <cstdint>

#include <Miracle/Common/Math/Vector3.hpp>
#include <Miracle/Common/Math/Matrix4.hpp>
#include <Miracle/Common/Math/Quaternion.hpp>
#include <Miracle/Common/Math/MathUtilities.hpp>

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
		constexpr Transform(
			const Vector3& translation,
			const Quaternion& rotation,
			const Vector3& scale
		) :
			m_translation(translation),
			m_rotation(rotation),
			m_scale(scale),
			m_cachedTransformation(createTransformation())
		{}

		constexpr const Vector3& getTranslation() const { return m_translation; }

		constexpr void setTranslation(const Vector3& translation) {
			m_translation = translation;
			m_cacheOutdated = true;
		}

		template<TransformSpace transformSpace = TransformSpace::local>
		void translate(const Vector3& deltaTranslation) {
			if constexpr (transformSpace == TransformSpace::local) {
				m_translation += MathUtilities::rotateVector(deltaTranslation, m_rotation);
			}
			else {
				m_translation += deltaTranslation;
			}

			m_cacheOutdated = true;
		}

		void translate(const Vector3& deltaTranslation, TransformSpace transformSpace) {
			m_translation += transformSpace == TransformSpace::local
				? MathUtilities::rotateVector(deltaTranslation, m_rotation)
				: deltaTranslation;

			m_cacheOutdated = true;
		}

		constexpr const Quaternion& getRotation() const { return m_rotation; }

		constexpr void setRotation(const Quaternion& rotation) {
			m_rotation = rotation;
			m_cacheOutdated = true;
		}

		template<TransformSpace transformSpace = TransformSpace::local>
		constexpr void rotate(const Quaternion& deltaRotation) {
			if constexpr (transformSpace == TransformSpace::local) {
				m_rotation = m_rotation * deltaRotation;
			}
			else {
				m_rotation = deltaRotation * m_rotation;
			}

			m_cacheOutdated = true;
		}

		constexpr void rotate(const Quaternion& deltaRotation, TransformSpace transformSpace) {
			m_rotation = transformSpace == TransformSpace::local
				? m_rotation * deltaRotation
				: deltaRotation * m_rotation;

			m_cacheOutdated = true;
		}

		constexpr const Vector3& getScale() const { return m_scale; }

		constexpr void setScale(const Vector3& scale) {
			m_scale = scale;
			m_cacheOutdated = true;
		}

		constexpr void scale(const Vector3& deltaScale) {
			m_scale += deltaScale;
			m_cacheOutdated = true;
		}

		constexpr void scale(float scalar) {
			m_scale *= scalar;
			m_cacheOutdated = true;
		}

		constexpr const Matrix4& getTransformation() const {
			if (m_cacheOutdated) {
				m_cachedTransformation = createTransformation();
				m_cacheOutdated = false;
			}

			return m_cachedTransformation;
		}

	private:
		constexpr Matrix4 createTransformation() const {
			return Matrix4::createTransformation(m_translation, m_rotation, m_scale);
		}
	};
}
