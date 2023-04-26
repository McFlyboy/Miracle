#pragma once

#include <numbers>
#include <type_traits>
#include <compare>

namespace Miracle {
	struct Degrees;
	struct Radians;

	struct Degrees {
		float value = {};

		/* ----- COMPARISON ----- */

		constexpr auto operator<=>(const Degrees&) const = default;

		/* ----- SIGNED CONVERTION ----- */

		constexpr inline Degrees operator+() const {
			return *this;
		}

		constexpr inline Degrees operator-() const {
			return Degrees{ .value = -value };
		}

		/* ----- ADDITION ----- */

		constexpr inline Degrees operator+(const Degrees& rhs) const {
			return Degrees{ .value = value + rhs.value };
		}

		constexpr inline Degrees& operator+=(const Degrees& rhs) {
			value += rhs.value;

			return *this;
		}

		/* ----- SUBTRACTION ----- */

		constexpr inline Degrees operator-(const Degrees& rhs) const {
			return Degrees{ .value = value - rhs.value };
		}

		constexpr inline Degrees& operator-=(const Degrees& rhs) {
			value -= rhs.value;

			return *this;
		}

		/* ----- SCALAR MULTIPLICATION ----- */

		constexpr inline Degrees operator*(float rhs) const {
			return Degrees{ .value = value * rhs };
		}

		constexpr inline friend Degrees operator*(float lhs, const Degrees& rhs) {
			return Degrees{ .value = lhs * rhs.value };
		}

		constexpr inline Degrees& operator*=(float rhs) {
			value *= rhs;

			return *this;
		}

		/* ----- SCALAR DIVISION ----- */

		constexpr inline Degrees operator/(float rhs) const {
			return Degrees{ .value = value / rhs };
		}

		constexpr inline friend Degrees operator/(float lhs, const Degrees& rhs) {
			return Degrees{ .value = lhs / rhs.value };
		}

		constexpr inline Degrees& operator/=(float rhs) {
			value /= rhs;

			return *this;
		}

		/* ----- CONVERTERS ----- */

		constexpr inline Radians toRadians() const;
	};

	struct Radians {
		float value = {};

		/* ----- COMPARISON ----- */

		constexpr auto operator<=>(const Radians&) const = default;

		/* ----- SIGNED CONVERTION ----- */

		constexpr inline Radians operator+() const {
			return *this;
		}

		constexpr inline Radians operator-() const {
			return Radians{ .value = -value };
		}

		/* ----- ADDITION ----- */

		constexpr inline Radians operator+(const Radians& rhs) const {
			return Radians{ .value = value + rhs.value };
		}

		constexpr inline Radians& operator+=(const Radians& rhs) {
			value += rhs.value;

			return *this;
		}

		/* ----- SUBTRACTION ----- */

		constexpr inline Radians operator-(const Radians& rhs) const {
			return Radians{ .value = value - rhs.value };
		}

		constexpr inline Radians& operator-=(const Radians& rhs) {
			value -= rhs.value;

			return *this;
		}

		/* ----- SCALAR MULTIPLICATION ----- */

		constexpr inline Radians operator*(float rhs) const {
			return Radians{ .value = value * rhs };
		}

		constexpr inline friend Radians operator*(float lhs, const Radians& rhs) {
			return Radians{ .value = lhs * rhs.value };
		}

		constexpr inline Radians& operator*=(float rhs) {
			value *= rhs;

			return *this;
		}

		/* ----- SCALAR DIVISION ----- */

		constexpr inline Radians operator/(float rhs) const {
			return Radians{ .value = value / rhs };
		}

		constexpr inline friend Radians operator/(float lhs, const Radians& rhs) {
			return Radians{ .value = lhs / rhs.value };
		}

		constexpr inline Radians& operator/=(float rhs) {
			value /= rhs;

			return *this;
		}

		/* ----- CONVERTERS ----- */

		constexpr inline Degrees toDegrees() const;
	};

	constexpr inline Radians Degrees::toRadians() const {
		return Radians{ .value = value * std::numbers::pi_v<float> / 180.0f };
	}

	constexpr inline Degrees Radians::toDegrees() const {
		return Degrees{ .value = value * 180.0f / std::numbers::pi_v<float> };
	}

	constexpr inline Degrees operator""_deg(long double value) {
		return Degrees{ .value = static_cast<float>(value) };
	}

	constexpr inline Radians operator""_rad(long double value) {
		return Radians{ .value = static_cast<float>(value) };
	}

	template<typename T>
	concept Angle = std::is_same_v<T, Degrees> || std::is_same_v<T, Radians>;

	class AngleUtilities {
	public:
		AngleUtilities() = delete;

		template<Angle TAngle>
		static constexpr inline Radians castToRadians(TAngle angle) {
			if constexpr (std::is_same_v<TAngle, Degrees>) {
				return angle.toRadians();
			}
			else {
				return angle;
			}
		}
	};
}
