#pragma once

#include "Vector3.hpp"

namespace Miracle {
	struct Vector4 {
		float x = {};
		float y = {};
		float z = {};
		float w = {};

		/* ----- COMPARISON ----- */

		constexpr bool operator==(const Vector4&) const = default;

		/* ----- CONVERTERS ----- */

		static constexpr inline Vector4 createFromVector2(const Vector2& xy, float z, float w) {
			return Vector4{
				.x = xy.x,
				.y = xy.y,
				.z = z,
				.w = w
			};
		}

		static constexpr inline Vector4 createFromVector3(const Vector3& xyz, float w) {
			return Vector4{
				.x = xyz.x,
				.y = xyz.y,
				.z = xyz.z,
				.w = w
			};
		}

		/* ----- SIGNED CONVERTION ----- */

		constexpr inline Vector4 operator+() const {
			return *this;
		}

		constexpr inline Vector4 operator-() const {
			return Vector4{
				.x = -x,
				.y = -y,
				.z = -z,
				.w = -w
			};
		}

		/* ----- ADDITION ----- */

		constexpr inline Vector4 operator+(const Vector4& rhs) const {
			return Vector4{
				.x = x + rhs.x,
				.y = y + rhs.y,
				.z = z + rhs.z,
				.w = w + rhs.w
			};
		}

		constexpr inline Vector4& operator+=(const Vector4& rhs) {
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			w += rhs.w;

			return *this;
		}

		/* ----- SUBTRACTION ----- */

		constexpr inline Vector4 operator-(const Vector4& rhs) const {
			return Vector4{
				.x = x - rhs.x,
				.y = y - rhs.y,
				.z = z - rhs.z,
				.w = w - rhs.w
			};
		}

		constexpr inline Vector4& operator-=(const Vector4& rhs) {
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			w -= rhs.w;

			return *this;
		}

		/* ----- SCALAR MULTIPLICATION ----- */

		constexpr inline Vector4 operator*(float rhs) const {
			return Vector4{
				.x = x * rhs,
				.y = y * rhs,
				.z = z * rhs,
				.w = w * rhs
			};
		}

		constexpr inline friend Vector4 operator*(float lhs, const Vector4& rhs) {
			return Vector4{
				.x = lhs * rhs.x,
				.y = lhs * rhs.y,
				.z = lhs * rhs.z,
				.w = lhs * rhs.w
			};
		}

		constexpr inline Vector4& operator*=(float rhs) {
			x *= rhs;
			y *= rhs;
			z *= rhs;
			w *= rhs;

			return *this;
		}

		/* ----- SCALAR DIVISION ----- */

		constexpr inline Vector4 operator/(float rhs) const {
			return Vector4{
				.x = x / rhs,
				.y = y / rhs,
				.z = z / rhs,
				.w = w / rhs
			};
		}

		constexpr inline friend Vector4 operator/(float lhs, const Vector4& rhs) {
			return Vector4{
				.x = lhs / rhs.x,
				.y = lhs / rhs.y,
				.z = lhs / rhs.z,
				.w = lhs / rhs.w
			};
		}

		constexpr inline Vector4& operator/=(float rhs) {
			x /= rhs;
			y /= rhs;
			z /= rhs;
			w /= rhs;

			return *this;
		}

		/* ----- MISC. ----- */

		inline float getLength() const {
			return std::sqrt(x * x + y * y + z * z + w * w);
		}

		inline Vector4 toNormalized() const {
			if (*this == Vector4{}) {
				return Vector4{};
			}

			return *this / getLength();
		}

		inline Vector4& normalize() {
			if (*this == Vector4{}) {
				return *this;
			}

			*this /= getLength();

			return *this;
		}

		inline float distanceTo(const Vector4& vector) const {
			return (vector - *this).getLength();
		}

		constexpr inline float dot(const Vector4& rhs) const {
			return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
		}

		constexpr inline Vector4 lerp(const Vector4& vector, float t) const {
			return *this + (vector - *this) * t;
		}
	};
}
