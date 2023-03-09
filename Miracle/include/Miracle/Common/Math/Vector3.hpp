#pragma once

#include <cmath>

#include "Vector2.hpp"

namespace Miracle {
	struct Vector3 {
		float x = {};
		float y = {};
		float z = {};

		static const Vector3 right;
		static const Vector3 up;
		static const Vector3 forward;

		/* ----- COMPARISON ----- */

		constexpr bool operator==(const Vector3&) const = default;

		/* ----- CONVERTERS ----- */
		static constexpr inline Vector3 createFromVector2(const Vector2& xy, float z) {
			return Vector3{
				.x = xy.x,
				.y = xy.y,
				.z = z
			};
		}

		/* ----- ADDITION ----- */

		constexpr inline Vector3 operator+(const Vector3& rhs) const {
			return Vector3{
				.x = x + rhs.x,
				.y = y + rhs.y,
				.z = z + rhs.z
			};
		}

		constexpr inline Vector3 operator+=(const Vector3& rhs) {
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;

			return *this;
		}

		/* ----- SUBTRACTION ----- */

		constexpr inline Vector3 operator-(const Vector3& rhs) const {
			return Vector3{
				.x = x - rhs.x,
				.y = y - rhs.y,
				.z = z - rhs.z
			};
		}

		constexpr inline Vector3 operator-=(const Vector3& rhs) {
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;

			return *this;
		}

		/* ----- SCALAR MULTIPLICATION ----- */

		constexpr inline Vector3 operator*(float rhs) const {
			return Vector3{
				.x = x * rhs,
				.y = y * rhs,
				.z = z * rhs
			};
		}

		constexpr inline friend Vector3 operator*(float lhs, const Vector3& rhs) {
			return Vector3{
				.x = lhs * rhs.x,
				.y = lhs * rhs.y,
				.z = lhs * rhs.z
			};
		}

		constexpr inline Vector3 operator*=(float rhs) {
			x *= rhs;
			y *= rhs;
			z *= rhs;

			return *this;
		}

		/* ----- SCALAR DIVISION ----- */

		constexpr inline Vector3 operator/(float rhs) const {
			return Vector3{
				.x = x / rhs,
				.y = y / rhs,
				.z = z / rhs
			};
		}

		constexpr inline friend Vector3 operator/(float lhs, const Vector3& rhs) {
			return Vector3{
				.x = lhs / rhs.x,
				.y = lhs / rhs.y,
				.z = lhs / rhs.z
			};
		}

		constexpr inline Vector3 operator/=(float rhs) {
			x /= rhs;
			y /= rhs;
			z /= rhs;

			return *this;
		}

		/* ----- MISC. ----- */

		inline float getLenght() const {
			return std::sqrt(x * x + y * y + z * z);
		}

		inline Vector3 toNormalized() const {
			if (x == 0.0f && y == 0.0f && z == 0.0f) {
				return Vector3{};
			}

			auto length = getLenght();

			return Vector3{
				.x = x / length,
				.y = y / length,
				.z = z / length
			};
		}

		inline Vector3 normalize() {
			if (x == 0.0f && y == 0.0f && z == 0.0f) {
				return *this;
			}

			auto length = getLenght();

			x /= length;
			y /= length;
			z /= length;

			return *this;
		}

		inline float distanceTo(const Vector3& vector) const {
			return (vector - *this).getLenght();
		}

		constexpr inline float dot(const Vector3& rhs) const {
			return x * rhs.x + y * rhs.y + z * rhs.z;
		}

		constexpr inline Vector3 cross(const Vector3& rhs) const {
			return Vector3{
				.x = y * rhs.z - z * rhs.y,
				.y = z * rhs.x - x * rhs.z,
				.z = x * rhs.y - y * rhs.x
			};
		}
	};
}
