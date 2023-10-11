#pragma once

#include <cmath>
#include <cstddef>

namespace Miracle {
	struct Vector2 {
		float x = {};
		float y = {};

		/* ----- COMPARISON ----- */

		constexpr bool operator==(const Vector2&) const = default;

		/* ----- SIGNED CONVERTION ----- */

		constexpr Vector2 operator+() const { return *this; }

		constexpr Vector2 operator-() const {
			return Vector2{
				.x = -x,
				.y = -y
			};
		}

		/* ----- ADDITION ----- */

		constexpr Vector2 operator+(const Vector2& rhs) const {
			return Vector2{
				.x = x + rhs.x,
				.y = y + rhs.y
			};
		}

		constexpr Vector2& operator+=(const Vector2& rhs) {
			x += rhs.x;
			y += rhs.y;

			return *this;
		}

		/* ----- SUBTRACTION ----- */

		constexpr Vector2 operator-(const Vector2& rhs) const {
			return Vector2{
				.x = x - rhs.x,
				.y = y - rhs.y
			};
		}

		constexpr Vector2& operator-=(const Vector2& rhs) {
			x -= rhs.x;
			y -= rhs.y;

			return *this;
		}

		/* ----- SCALAR MULTIPLICATION ----- */

		constexpr Vector2 operator*(float rhs) const {
			return Vector2{
				.x = x * rhs,
				.y = y * rhs
			};
		}

		constexpr friend Vector2 operator*(float lhs, const Vector2& rhs) {
			return Vector2{
				.x = lhs * rhs.x,
				.y = lhs * rhs.y
			};
		}

		constexpr Vector2& operator*=(float rhs) {
			x *= rhs;
			y *= rhs;

			return *this;
		}

		/* ----- SCALAR DIVISION ----- */

		constexpr Vector2 operator/(float rhs) const {
			return Vector2{
				.x = x / rhs,
				.y = y / rhs
			};
		}

		constexpr friend Vector2 operator/(float lhs, const Vector2& rhs) {
			return Vector2{
				.x = lhs / rhs.x,
				.y = lhs / rhs.y
			};
		}

		constexpr Vector2& operator/=(float rhs) {
			x /= rhs;
			y /= rhs;

			return *this;
		}

		/* ----- MISC. ----- */

		float getLength() const {
			return std::sqrt(x * x + y * y);
		}

		Vector2 toNormalized() const {
			if (*this == Vector2{}) {
				return *this;
			}

			return *this / getLength();
		}

		Vector2& normalize() {
			if (*this == Vector2{}) {
				return *this;
			}

			return *this /= getLength();
		}

		float distanceTo(const Vector2& vector) const {
			return (vector - *this).getLength();
		}

		constexpr float dot(const Vector2& rhs) const {
			return x * rhs.x + y * rhs.y;
		}

		constexpr Vector2 lerp(const Vector2& vector, float t) const {
			return *this + (vector - *this) * t;
		}
	};

	class Vector2s {
	public:
		Vector2s() = delete;

		static constexpr Vector2 zero  = Vector2{ .x =  0.0f, .y =  0.0f };
		static constexpr Vector2 right = Vector2{ .x =  1.0f, .y =  0.0f };
		static constexpr Vector2 left  = Vector2{ .x = -1.0f, .y =  0.0f };
		static constexpr Vector2 up	   = Vector2{ .x =  0.0f, .y =  1.0f };
		static constexpr Vector2 down  = Vector2{ .x =  0.0f, .y = -1.0f };
	};
}
