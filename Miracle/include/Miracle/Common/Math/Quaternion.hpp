#pragma once

#include <cmath>

#include "Angle.hpp"
#include "Vector3.hpp"

namespace Miracle {
	struct Quaternion {
		float w = {};
		Vector3 v = {};

		static const Quaternion identity;

		/* ----- COMPARISON ----- */

		constexpr bool operator==(const Quaternion&) const = default;

		/* ----- SIGNED CONVERTION ----- */

		constexpr inline Quaternion operator+() const {
			return *this;
		}

		constexpr inline Quaternion operator-() const {
			return Quaternion{
				.w = -w,
				.v = -v
			};
		}

		/* ----- MULTIPLICATION ----- */

		constexpr inline Quaternion operator*(const Quaternion& rhs) const {
			return Quaternion{
				.w = w * rhs.w - v.dot(rhs.v),
				.v = w * rhs.v + v * rhs.w + v.cross(rhs.v)
			};
		}

		constexpr inline Quaternion& operator*=(const Quaternion& rhs) {
			auto newW = w * rhs.w - v.dot(rhs.v);
			auto newV = w * rhs.v + v * rhs.w + v.cross(rhs.v);

			w = newW;
			v = newV;

			return *this;
		}

		/* ----- SCALAR MULTIPLICATION ----- */

		constexpr inline Quaternion operator*(float rhs) const {
			return Quaternion{
				.w = w * rhs,
				.v = v * rhs
			};
		}

		constexpr inline friend Quaternion operator*(float lhs, const Quaternion& rhs) {
			return Quaternion{
				.w = lhs * rhs.w,
				.v = lhs * rhs.v
			};
		}

		constexpr inline Quaternion& operator*=(float rhs) {
			w *= rhs;
			v *= rhs;

			return *this;
		}

		/* ----- SCALAR DIVISION ----- */

		constexpr inline Quaternion operator/(float rhs) const {
			return Quaternion{
				.w = w / rhs,
				.v = v / rhs
			};
		}

		constexpr inline friend Quaternion operator/(float lhs, const Quaternion& rhs) {
			return Quaternion{
				.w = lhs / rhs.w,
				.v = lhs / rhs.v
			};
		}

		constexpr inline Quaternion& operator/=(float rhs) {
			w /= rhs;
			v /= rhs;

			return *this;
		}

		/* ----- ROTATION ----- */

		template<Angle TAngle>
		static inline Quaternion createRotation(const Vector3& axis, TAngle angle) {
			float halfAngle = static_cast<Radians>(angle).value / 2.0f;

			return Quaternion{
				.w = std::cos(halfAngle),
				.v = std::sin(halfAngle) * axis
			};
		}

		/* ----- MISC. ----- */

		inline float getMagnitude() const {
			return std::sqrt(w * w + v.x * v.x + v.y * v.y + v.z * v.z);
		}

		constexpr inline Quaternion getConjugate() const {
			return Quaternion{ .w = w, .v = -v };
		}

		constexpr inline Quaternion getInverse() const {
			if (*this == Quaternion{}) {
				return Quaternion{};
			}

			return getConjugate() / getMagnitude();
		}

		constexpr inline Quaternion getDifference(const Quaternion& quaternion) const {
			return quaternion * getInverse();
		}

		constexpr inline float dot(const Quaternion& rhs) const {
			return w * rhs.w + v.dot(rhs.v);
		}

		inline Quaternion toExponentiated(float exponent) const {
			float halfAngle = std::acos(w);

			if (halfAngle == 0.0f) {
				return Quaternion{ .w = w, .v = v };
			}

			float newHalfAngle = halfAngle * exponent;

			return Quaternion{
				.w = std::cos(newHalfAngle),
				.v = v * std::sin(newHalfAngle) / std::sin(halfAngle)
			};
		}

		inline Quaternion& exponentiate(float exponent) {
			float halfAngle = std::acos(w);

			if (halfAngle == 0.0f) {
				return *this;
			}

			float newHalfAngle = halfAngle * exponent;
			
			w = std::cos(newHalfAngle);
			v *= std::sin(newHalfAngle) / std::sin(halfAngle);

			return *this;
		}

		inline Quaternion slerp(const Quaternion& quaternion, float t) const {
			return getDifference(quaternion).toExponentiated(t) * *this;
		}
	};
}
