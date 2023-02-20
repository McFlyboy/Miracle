#pragma once

#include <cmath>
#include <compare>

namespace Miracle {
	struct Vector2f {
		float x = {};
		float y = {};

		static const Vector2f right;
		static const Vector2f up;

		/* ----- COMPARISON ----- */

		constexpr auto operator<=>(const Vector2f&) const = default;

		/* ----- ADDITION ----- */

		constexpr inline Vector2f operator+(const Vector2f& rhs) const {
			return Vector2f{
				.x = x + rhs.x,
				.y = y + rhs.y
			};
		}

		constexpr inline Vector2f operator+=(const Vector2f& rhs) {
			x += rhs.x;
			y += rhs.y;

			return *this;
		}

		/* ----- SUBTRACTION ----- */

		constexpr inline Vector2f operator-(const Vector2f& rhs) const {
			return Vector2f{
				.x = x - rhs.x,
				.y = y - rhs.y
			};
		}

		constexpr inline Vector2f operator-=(const Vector2f& rhs) {
			x -= rhs.x;
			y -= rhs.y;

			return *this;
		}

		/* ----- SCALAR MULTIPLICATION ----- */

		constexpr inline Vector2f operator*(float rhs) const {
			return Vector2f{
				.x = x * rhs,
				.y = y * rhs
			};
		}

		constexpr inline friend Vector2f operator*(float lhs, const Vector2f& rhs) {
			return Vector2f{
				.x = lhs * rhs.x,
				.y = lhs * rhs.y
			};
		}

		constexpr inline Vector2f operator*=(float rhs) {
			x *= rhs;
			y *= rhs;

			return *this;
		}

		/* ----- SCALAR DIVISION ----- */

		constexpr inline Vector2f operator/(float rhs) const {
			return Vector2f{
				.x = x / rhs,
				.y = y / rhs
			};
		}

		constexpr inline friend Vector2f operator/(float lhs, const Vector2f& rhs) {
			return Vector2f{
				.x = lhs / rhs.x,
				.y = lhs / rhs.y
			};
		}

		constexpr inline Vector2f operator/=(float rhs) {
			x /= rhs;
			y /= rhs;

			return *this;
		}

		/* ----- MISC. ----- */

		inline float getLenght() const {
			return std::sqrtf(x * x + y * y);
		}

		inline Vector2f toNormalized() const {
			auto length = getLenght();

			return length != 0.0f
				? Vector2f{ .x = x / length, .y = y / length }
				: Vector2f{};
		}

		inline Vector2f normalize() {
			auto length = getLenght();

			if (length != 0.0f) {
				x /= length;
				y /= length;
			}

			return *this;
		}

		constexpr inline float dot(const Vector2f& rhs) const {
			return x * rhs.x + y * rhs.y;
		}
	};
}
