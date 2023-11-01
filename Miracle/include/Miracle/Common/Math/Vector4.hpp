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

		static constexpr Vector4 createFromVector2(const Vector2& xy, float z = 0.0f, float w = 0.0f) {
			return Vector4{
				.x = xy.x,
				.y = xy.y,
				.z = z,
				.w = w
			};
		}

		constexpr Vector2 toVector2() {
			return Vector2{
				.x = x,
				.y = y
			};
		}

		static constexpr Vector4 createFromVector3(const Vector3& xyz, float w = 0.0f) {
			return Vector4{
				.x = xyz.x,
				.y = xyz.y,
				.z = xyz.z,
				.w = w
			};
		}

		constexpr Vector3 toVector3() {
			return Vector3{
				.x = x,
				.y = y,
				.z = z
			};
		}

		/* ----- SIGNED CONVERTION ----- */

		constexpr Vector4 operator+() const { return *this; }

		constexpr Vector4 operator-() const {
			return Vector4{
				.x = -x,
				.y = -y,
				.z = -z,
				.w = -w
			};
		}

		/* ----- ADDITION ----- */

		constexpr Vector4 operator+(const Vector4& rhs) const {
			return Vector4{
				.x = x + rhs.x,
				.y = y + rhs.y,
				.z = z + rhs.z,
				.w = w + rhs.w
			};
		}

		constexpr Vector4& operator+=(const Vector4& rhs) {
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			w += rhs.w;

			return *this;
		}

		/* ----- SUBTRACTION ----- */

		constexpr Vector4 operator-(const Vector4& rhs) const {
			return Vector4{
				.x = x - rhs.x,
				.y = y - rhs.y,
				.z = z - rhs.z,
				.w = w - rhs.w
			};
		}

		constexpr Vector4& operator-=(const Vector4& rhs) {
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			w -= rhs.w;

			return *this;
		}

		/* ----- SCALAR MULTIPLICATION ----- */

		constexpr Vector4 operator*(float rhs) const {
			return Vector4{
				.x = x * rhs,
				.y = y * rhs,
				.z = z * rhs,
				.w = w * rhs
			};
		}

		constexpr friend Vector4 operator*(float lhs, const Vector4& rhs) {
			return Vector4{
				.x = lhs * rhs.x,
				.y = lhs * rhs.y,
				.z = lhs * rhs.z,
				.w = lhs * rhs.w
			};
		}

		constexpr Vector4& operator*=(float rhs) {
			x *= rhs;
			y *= rhs;
			z *= rhs;
			w *= rhs;

			return *this;
		}

		/* ----- SCALAR DIVISION ----- */

		constexpr Vector4 operator/(float rhs) const {
			return Vector4{
				.x = x / rhs,
				.y = y / rhs,
				.z = z / rhs,
				.w = w / rhs
			};
		}

		constexpr friend Vector4 operator/(float lhs, const Vector4& rhs) {
			return Vector4{
				.x = lhs / rhs.x,
				.y = lhs / rhs.y,
				.z = lhs / rhs.z,
				.w = lhs / rhs.w
			};
		}

		constexpr Vector4& operator/=(float rhs) {
			x /= rhs;
			y /= rhs;
			z /= rhs;
			w /= rhs;

			return *this;
		}

		/* ----- MISC. ----- */

		float getLength() const {
			return std::sqrt(x * x + y * y + z * z + w * w);
		}

		Vector4 toNormalized() const {
			if (*this == Vector4{}) {
				return *this;
			}

			return *this / getLength();
		}

		Vector4& normalize() {
			if (*this == Vector4{}) {
				return *this;
			}

			return *this /= getLength();
		}

		float distanceTo(const Vector4& vector) const {
			return (vector - *this).getLength();
		}

		constexpr float dot(const Vector4& rhs) const {
			return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
		}

		constexpr Vector4 lerp(const Vector4& vector, float t) const {
			return *this + (vector - *this) * t;
		}
	};

	// Directional names in 4D space
	// https://en.wikipedia.org/wiki/Four-dimensional_space#Orthogonality_and_vocabulary

	class Vector4s {
	public:
		Vector4s() = delete;

		static constexpr Vector4 zero    = Vector4{ .x =  0.0f, .y =  0.0f, .z =  0.0f, .w =  0.0f };
		static constexpr Vector4 right   = Vector4{ .x =  1.0f, .y =  0.0f, .z =  0.0f, .w =  0.0f };
		static constexpr Vector4 left    = Vector4{ .x = -1.0f, .y =  0.0f, .z =  0.0f, .w =  0.0f };
		static constexpr Vector4 up      = Vector4{ .x =  0.0f, .y =  1.0f, .z =  0.0f, .w =  0.0f };
		static constexpr Vector4 down    = Vector4{ .x =  0.0f, .y = -1.0f, .z =  0.0f, .w =  0.0f };
		static constexpr Vector4 forward = Vector4{ .x =  0.0f, .y =  0.0f, .z =  1.0f, .w =  0.0f };
		static constexpr Vector4 back    = Vector4{ .x =  0.0f, .y =  0.0f, .z = -1.0f, .w =  0.0f };
		static constexpr Vector4 ana     = Vector4{ .x =  0.0f, .y =  0.0f, .z =  0.0f, .w =  1.0f };
		static constexpr Vector4 kata    = Vector4{ .x =  0.0f, .y =  0.0f, .z =  0.0f, .w = -1.0f };
	};
}
