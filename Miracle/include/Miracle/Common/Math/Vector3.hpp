#pragma once

#include <cmath>
#include <cstddef>

#include "Vector2.hpp"

namespace Miracle {
	struct Vector3 {
		float x = {};
		float y = {};
		float z = {};

		/* ----- COMPARISON ----- */

		constexpr bool operator==(const Vector3&) const = default;

		/* ----- CONVERTERS ----- */

		static constexpr Vector3 createFromVector2(const Vector2& xy, float z = 0.0f) {
			return Vector3{
				.x = xy.x,
				.y = xy.y,
				.z = z
			};
		}

		constexpr Vector2 toVector2() {
			return Vector2{
				.x = x,
				.y = y
			};
		}

		/* ----- SIGNED CONVERTION ----- */

		constexpr Vector3 operator+() const { return *this; }

		constexpr Vector3 operator-() const {
			return Vector3{
				.x = -x,
				.y = -y,
				.z = -z
			};
		}

		/* ----- ADDITION ----- */

		constexpr Vector3 operator+(const Vector3& rhs) const {
			return Vector3{
				.x = x + rhs.x,
				.y = y + rhs.y,
				.z = z + rhs.z
			};
		}

		constexpr Vector3& operator+=(const Vector3& rhs) {
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;

			return *this;
		}

		/* ----- SUBTRACTION ----- */

		constexpr Vector3 operator-(const Vector3& rhs) const {
			return Vector3{
				.x = x - rhs.x,
				.y = y - rhs.y,
				.z = z - rhs.z
			};
		}

		constexpr Vector3& operator-=(const Vector3& rhs) {
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;

			return *this;
		}

		/* ----- SCALAR MULTIPLICATION ----- */

		constexpr Vector3 operator*(float rhs) const {
			return Vector3{
				.x = x * rhs,
				.y = y * rhs,
				.z = z * rhs
			};
		}

		constexpr friend Vector3 operator*(float lhs, const Vector3& rhs) {
			return Vector3{
				.x = lhs * rhs.x,
				.y = lhs * rhs.y,
				.z = lhs * rhs.z
			};
		}

		constexpr Vector3& operator*=(float rhs) {
			x *= rhs;
			y *= rhs;
			z *= rhs;

			return *this;
		}

		/* ----- SCALAR DIVISION ----- */

		constexpr Vector3 operator/(float rhs) const {
			return Vector3{
				.x = x / rhs,
				.y = y / rhs,
				.z = z / rhs
			};
		}

		constexpr friend Vector3 operator/(float lhs, const Vector3& rhs) {
			return Vector3{
				.x = lhs / rhs.x,
				.y = lhs / rhs.y,
				.z = lhs / rhs.z
			};
		}

		constexpr Vector3& operator/=(float rhs) {
			x /= rhs;
			y /= rhs;
			z /= rhs;

			return *this;
		}

		/* ----- MISC. ----- */

		float getLength() const {
			return std::sqrt(x * x + y * y + z * z);
		}

		Vector3 toNormalized() const {
			if (*this == Vector3{}) {
				return *this;
			}

			return *this / getLength();
		}

		Vector3& normalize() {
			if (*this == Vector3{}) {
				return *this;
			}

			return *this /= getLength();
		}

		float distanceTo(const Vector3& vector) const {
			return (vector - *this).getLength();
		}

		constexpr float dot(const Vector3& rhs) const {
			return x * rhs.x + y * rhs.y + z * rhs.z;
		}

		constexpr Vector3 cross(const Vector3& rhs) const {
			return Vector3{
				.x = y * rhs.z - z * rhs.y,
				.y = z * rhs.x - x * rhs.z,
				.z = x * rhs.y - y * rhs.x
			};
		}

		constexpr Vector3 lerp(const Vector3& vector, float t) const {
			return *this + (vector - *this) * t;
		}
	};

	class Vector3s {
	public:
		Vector3s() = delete;

		static constexpr Vector3 zero    = Vector3{ .x =  0.0f, .y =  0.0f, .z =  0.0f };
		static constexpr Vector3 right	 = Vector3{ .x =  1.0f, .y =  0.0f, .z =  0.0f };
		static constexpr Vector3 left    = Vector3{ .x = -1.0f, .y =  0.0f, .z =  0.0f };
		static constexpr Vector3 up		 = Vector3{ .x =  0.0f, .y =  1.0f, .z =  0.0f };
		static constexpr Vector3 down    = Vector3{ .x =  0.0f, .y = -1.0f, .z =  0.0f };
		static constexpr Vector3 forward = Vector3{ .x =  0.0f, .y =  0.0f, .z =  1.0f };
		static constexpr Vector3 back    = Vector3{ .x =  0.0f, .y =  0.0f, .z = -1.0f };
	};
}
