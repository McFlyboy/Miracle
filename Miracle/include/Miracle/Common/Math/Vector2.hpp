#pragma once

#include <cmath>
#include <cstddef>

namespace Miracle {
	struct Vector2 {
		float x = {};
		float y = {};

		static const Vector2 right;
		static const Vector2 up;

		/* ----- COMPARISON ----- */

		constexpr bool operator==(const Vector2&) const = default;

		/* ----- SIGNED CONVERTION ----- */

		constexpr inline Vector2 operator+() const {
			return *this;
		}

		constexpr inline Vector2 operator-() const {
			return Vector2{
				.x = -x,
				.y = -y
			};
		}

		/* ----- ADDITION ----- */

		constexpr inline Vector2 operator+(const Vector2& rhs) const {
			return Vector2{
				.x = x + rhs.x,
				.y = y + rhs.y
			};
		}

		constexpr inline Vector2& operator+=(const Vector2& rhs) {
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

		constexpr inline Vector2& operator-=(const Vector2& rhs) {
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

		constexpr inline Vector2& operator*=(float rhs) {
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

		constexpr inline Vector2& operator/=(float rhs) {
			x /= rhs;
			y /= rhs;

			return *this;
		}

		/* ----- MISC. ----- */

		inline float getLenght() const {
			return std::sqrt(x * x + y * y);
		}

		inline Vector2 toNormalized() const {
			if (*this == Vector2{}) {
				return Vector2{};
			}

			return *this / getLenght();
		}

		inline Vector2& normalize() {
			if (*this == Vector2{}) {
				return *this;
			}

			*this /= getLenght();

			return *this;
		}

		inline float distanceTo(const Vector2& vector) const {
			return (vector - *this).getLenght();
		}

		constexpr inline float dot(const Vector2& rhs) const {
			return x * rhs.x + y * rhs.y;
		}
	};
}
