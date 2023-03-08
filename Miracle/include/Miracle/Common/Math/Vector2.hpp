#pragma once

#include <cmath>
#include <compare>

namespace Miracle {
	struct Vector2 {
		float x = {};
		float y = {};

		static const Vector2 right;
		static const Vector2 up;

		/* ----- COMPARISON ----- */

		constexpr auto operator<=>(const Vector2&) const = default;

		/* ----- ADDITION ----- */

		constexpr inline Vector2 operator+(const Vector2& rhs) const {
			return Vector2{
				.x = x + rhs.x,
				.y = y + rhs.y
			};
		}

		constexpr inline Vector2 operator+=(const Vector2& rhs) {
			x += rhs.x;
			y += rhs.y;

			return *this;
		}

		/* ----- SUBTRACTION ----- */

		constexpr inline Vector2 operator-(const Vector2& rhs) const {
			return Vector2{
				.x = x - rhs.x,
				.y = y - rhs.y
			};
		}

		constexpr inline Vector2 operator-=(const Vector2& rhs) {
			x -= rhs.x;
			y -= rhs.y;

			return *this;
		}

		/* ----- SCALAR MULTIPLICATION ----- */

		constexpr inline Vector2 operator*(float rhs) const {
			return Vector2{
				.x = x * rhs,
				.y = y * rhs
			};
		}

		constexpr inline friend Vector2 operator*(float lhs, const Vector2& rhs) {
			return Vector2{
				.x = lhs * rhs.x,
				.y = lhs * rhs.y
			};
		}

		constexpr inline Vector2 operator*=(float rhs) {
			x *= rhs;
			y *= rhs;

			return *this;
		}

		/* ----- SCALAR DIVISION ----- */

		constexpr inline Vector2 operator/(float rhs) const {
			return Vector2{
				.x = x / rhs,
				.y = y / rhs
			};
		}

		constexpr inline friend Vector2 operator/(float lhs, const Vector2& rhs) {
			return Vector2{
				.x = lhs / rhs.x,
				.y = lhs / rhs.y
			};
		}

		constexpr inline Vector2 operator/=(float rhs) {
			x /= rhs;
			y /= rhs;

			return *this;
		}

		/* ----- MISC. ----- */

		inline float getLenght() const {
			return std::sqrt(x * x + y * y);
		}

		inline Vector2 toNormalized() const {
			auto length = getLenght();

			return length != 0.0f
				? Vector2{ .x = x / length, .y = y / length }
				: Vector2{};
		}

		inline Vector2 normalize() {
			auto length = getLenght();

			if (length != 0.0f) {
				x /= length;
				y /= length;
			}

			return *this;
		}

		constexpr inline float dot(const Vector2& rhs) const {
			return x * rhs.x + y * rhs.y;
		}
	};
}
