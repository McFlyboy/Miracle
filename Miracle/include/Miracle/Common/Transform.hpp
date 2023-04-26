#pragma once

#include "Math/Vector3.hpp"
#include "Math/Matrix4.hpp"
#include "Math/Quaternion.hpp"

namespace Miracle {
	class TransformView {
	protected:
		Matrix4& m_transformation;
	
		inline TransformView(Matrix4& transformation) :
			m_transformation(transformation)
		{}
	};

	class Translate : public TransformView {
	public:
		inline Translate(Matrix4& transformation) : TransformView(transformation) {}

		/* ----- COMPARISON ----- */

		constexpr bool operator==(const Vector3& rhs) const {
			return m_transformation.m41 == rhs.x
				&& m_transformation.m42 == rhs.y
				&& m_transformation.m43 == rhs.z;
		}

		/* ----- CONVERTERS ----- */

		constexpr inline operator Vector3() const {
			return Vector3{
				.x = m_transformation.m41,
				.y = m_transformation.m42,
				.z = m_transformation.m43
			};
		}

		/* ----- SIGNED CONVERTION ----- */

		constexpr inline Vector3 operator+() const {
			return *this;
		}

		constexpr inline Vector3 operator-() const {
			return Vector3{
				.x = -m_transformation.m41,
				.y = -m_transformation.m42,
				.z = -m_transformation.m43
			};
		}

		/* ----- ADDITION ----- */

		constexpr inline Vector3 operator+(const Vector3& rhs) const {
			return Vector3{
				.x = m_transformation.m41 + rhs.x,
				.y = m_transformation.m42 + rhs.y,
				.z = m_transformation.m43 + rhs.z
			};
		}

		constexpr inline Translate& operator+=(const Vector3& rhs) {
			m_transformation.m41 += rhs.x;
			m_transformation.m42 += rhs.y;
			m_transformation.m43 += rhs.z;

			return *this;
		}

		/* ----- SUBTRACTION ----- */

		constexpr inline Vector3 operator-(const Vector3& rhs) const {
			return Vector3{
				.x = m_transformation.m41 - rhs.x,
				.y = m_transformation.m42 - rhs.y,
				.z = m_transformation.m43 - rhs.z
			};
		}

		constexpr inline Translate& operator-=(const Vector3& rhs) {
			m_transformation.m41 -= rhs.x;
			m_transformation.m42 -= rhs.y;
			m_transformation.m43 -= rhs.z;

			return *this;
		}

		/* ----- SCALAR MULTIPLICATION ----- */

		constexpr inline Vector3 operator*(float rhs) const {
			return Vector3{
				.x = m_transformation.m41 * rhs,
				.y = m_transformation.m42 * rhs,
				.z = m_transformation.m43 * rhs
			};
		}

		constexpr inline friend Vector3 operator*(float lhs, const Translate& rhs) {
			return Vector3{
				.x = lhs * rhs.m_transformation.m41,
				.y = lhs * rhs.m_transformation.m42,
				.z = lhs * rhs.m_transformation.m43
			};
		}

		constexpr inline Translate& operator*=(float rhs) {
			m_transformation.m41 *= rhs;
			m_transformation.m42 *= rhs;
			m_transformation.m43 *= rhs;

			return *this;
		}

		/* ----- SCALAR DIVISION ----- */

		constexpr inline Vector3 operator/(float rhs) const {
			return Vector3{
				.x = m_transformation.m41 / rhs,
				.y = m_transformation.m42 / rhs,
				.z = m_transformation.m43 / rhs
			};
		}

		constexpr inline friend Vector3 operator/(float lhs, const Translate& rhs) {
			return Vector3{
				.x = lhs / rhs.m_transformation.m41,
				.y = lhs / rhs.m_transformation.m42,
				.z = lhs / rhs.m_transformation.m43
			};
		}

		constexpr inline Translate& operator/=(float rhs) {
			m_transformation.m41 /= rhs;
			m_transformation.m42 /= rhs;
			m_transformation.m43 /= rhs;

			return *this;
		}

		/* ----- MISC. ----- */

		inline float getLenght() const {
			return std::sqrt(
				m_transformation.m41 * m_transformation.m41
					+ m_transformation.m42 * m_transformation.m42
					+ m_transformation.m43 * m_transformation.m43
			);
		}

		inline Vector3 toNormalized() const {
			if (*this == Vector3{}) {
				return Vector3{};
			}

			return *this / getLenght();
		}

		inline Translate& normalize() {
			if (*this == Vector3{}) {
				return *this;
			}

			*this /= getLenght();

			return *this;
		}

		inline float distanceTo(const Vector3& vector) const {
			return (vector - *this).getLenght();
		}

		constexpr inline float dot(const Vector3& rhs) const {
			return m_transformation.m41 * rhs.x + m_transformation.m42 * rhs.y + m_transformation.m43 * rhs.z;
		}

		constexpr inline Vector3 cross(const Vector3& rhs) const {
			return Vector3{
				.x = m_transformation.m42 * rhs.z - m_transformation.m43 * rhs.y,
				.y = m_transformation.m43 * rhs.x - m_transformation.m41 * rhs.z,
				.z = m_transformation.m41 * rhs.y - m_transformation.m42 * rhs.x
			};
		}
	};

	class Transform {
	private:
		Matrix4 m_transformation;
		Translate m_translate;

	public:
		Transform(
			const Vector3& translation,
			const Quaternion& rotation,
			const Vector3& scale
		);

		inline const Matrix4& getTransformation() const { return m_transformation; }

		inline Matrix4& getTransformation() { return m_transformation; }

		inline const Translate& getTranslation() const { return m_translate; }

		inline Translate& getTranslation() { return m_translate; }
	};
}
