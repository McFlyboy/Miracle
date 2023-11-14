#pragma once

#include <cmath>

#include "Angle.hpp"
#include "Vector3.hpp"

namespace Miracle {
	struct Quaternion {
		float w = {};
		Vector3 v = {};

		/* ----- COMPARISON ----- */

		constexpr bool operator==(const Quaternion&) const = default;

		/* ----- SIGNED CONVERTION ----- */

		constexpr Quaternion operator+() const { return *this; }

		constexpr Quaternion operator-() const {
			return Quaternion{
				.w = -w,
				.v = -v
			};
		}

		/* ----- MULTIPLICATION ----- */

		constexpr Quaternion operator*(const Quaternion& rhs) const {
			return Quaternion{
				.w = w * rhs.w - v.dot(rhs.v),
				.v = w * rhs.v + v * rhs.w + v.cross(rhs.v)
			};
		}

		constexpr Quaternion& operator*=(const Quaternion& rhs) {
			return *this = Quaternion{
				.w = w * rhs.w - v.dot(rhs.v),
				.v = w * rhs.v + v * rhs.w + v.cross(rhs.v)
			};
		}

		/* ----- SCALAR MULTIPLICATION ----- */

		constexpr Quaternion operator*(float rhs) const {
			return Quaternion{
				.w = w * rhs,
				.v = v * rhs
			};
		}

		constexpr friend Quaternion operator*(float lhs, const Quaternion& rhs) {
			return Quaternion{
				.w = lhs * rhs.w,
				.v = lhs * rhs.v
			};
		}

		constexpr Quaternion& operator*=(float rhs) {
			w *= rhs;
			v *= rhs;

			return *this;
		}

		/* ----- SCALAR DIVISION ----- */

		constexpr Quaternion operator/(float rhs) const {
			return Quaternion{
				.w = w / rhs,
				.v = v / rhs
			};
		}

		constexpr friend Quaternion operator/(float lhs, const Quaternion& rhs) {
			return Quaternion{
				.w = lhs / rhs.w,
				.v = lhs / rhs.v
			};
		}

		constexpr Quaternion& operator/=(float rhs) {
			w /= rhs;
			v /= rhs;

			return *this;
		}

		/* ----- ROTATION ----- */

		static Quaternion createRotation(const Vector3& axis, Angle auto angle) {
			float halfAngle = static_cast<Radians>(angle).value / 2.0f;

			return Quaternion{
				.w = std::cos(halfAngle),
				.v = std::sin(halfAngle) * axis
			};
		}

		/* ----- MISC. ----- */

		float getMagnitude() const {
			return std::sqrt(w * w + v.x * v.x + v.y * v.y + v.z * v.z);
		}

		constexpr Quaternion getConjugate() const {
			return Quaternion{ .w = w, .v = -v };
		}

		Quaternion getInverse() const {
			if (*this == Quaternion{}) {
				return *this;
			}

			return getConjugate() / getMagnitude();
		}

		Quaternion getDifference(const Quaternion& quaternion) const {
			return quaternion * getInverse();
		}

		constexpr float dot(const Quaternion& rhs) const {
			return w * rhs.w + v.dot(rhs.v);
		}

		Quaternion toExponentiated(float exponent) const {
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

		Quaternion& exponentiate(float exponent) {
			float halfAngle = std::acos(w);

			if (halfAngle == 0.0f) {
				return *this;
			}

			float newHalfAngle = halfAngle * exponent;
			
			w = std::cos(newHalfAngle);
			v *= std::sin(newHalfAngle) / std::sin(halfAngle);

			return *this;
		}

		Quaternion slerp(const Quaternion& quaternion, float t) const {
			return getDifference(quaternion).toExponentiated(t) * *this;
		}
	};

	class Quaternions {
	public:
		Quaternions() = delete;

		static constexpr Quaternion identity = Quaternion{ .w = 1.0f, .v = Vector3s::zero };
	};
}
