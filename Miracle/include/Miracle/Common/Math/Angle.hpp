#pragma once

#include <numbers>
#include <concepts>
#include <compare>

namespace Miracle {
	struct Degrees;
	struct Radians;

	struct Degrees {
		float value = {};

		/* ----- COMPARISON ----- */

		constexpr auto operator<=>(const Degrees&) const = default;

		/* ----- SIGNED CONVERTION ----- */

		constexpr Degrees operator+() const { return *this; }

		constexpr Degrees operator-() const { return Degrees{ .value = -value }; }

		/* ----- ADDITION ----- */

		constexpr Degrees operator+(const Degrees& rhs) const {
			return Degrees{ .value = value + rhs.value };
		}

		constexpr Degrees& operator+=(const Degrees& rhs) {
			value += rhs.value;

			return *this;
		}

		/* ----- SUBTRACTION ----- */

		constexpr Degrees operator-(const Degrees& rhs) const {
			return Degrees{ .value = value - rhs.value };
		}

		constexpr Degrees& operator-=(const Degrees& rhs) {
			value -= rhs.value;

			return *this;
		}

		/* ----- SCALAR MULTIPLICATION ----- */

		constexpr Degrees operator*(float rhs) const {
			return Degrees{ .value = value * rhs };
		}

		constexpr friend Degrees operator*(float lhs, const Degrees& rhs) {
			return Degrees{ .value = lhs * rhs.value };
		}

		constexpr Degrees& operator*=(float rhs) {
			value *= rhs;

			return *this;
		}

		/* ----- SCALAR DIVISION ----- */

		constexpr Degrees operator/(float rhs) const {
			return Degrees{ .value = value / rhs };
		}

		constexpr friend Degrees operator/(float lhs, const Degrees& rhs) {
			return Degrees{ .value = lhs / rhs.value };
		}

		constexpr Degrees& operator/=(float rhs) {
			value /= rhs;

			return *this;
		}

		/* ----- CONVERTERS ----- */

		constexpr Radians toRadians() const;

		constexpr explicit operator Radians() const;
	};

	struct Radians {
		float value = {};

		/* ----- COMPARISON ----- */

		constexpr auto operator<=>(const Radians&) const = default;

		/* ----- SIGNED CONVERTION ----- */

		constexpr Radians operator+() const { return *this; }

		constexpr Radians operator-() const { return Radians{ .value = -value }; }

		/* ----- ADDITION ----- */

		constexpr Radians operator+(const Radians& rhs) const {
			return Radians{ .value = value + rhs.value };
		}

		constexpr Radians& operator+=(const Radians& rhs) {
			value += rhs.value;

			return *this;
		}

		/* ----- SUBTRACTION ----- */

		constexpr Radians operator-(const Radians& rhs) const {
			return Radians{ .value = value - rhs.value };
		}

		constexpr Radians& operator-=(const Radians& rhs) {
			value -= rhs.value;

			return *this;
		}

		/* ----- SCALAR MULTIPLICATION ----- */

		constexpr Radians operator*(float rhs) const {
			return Radians{ .value = value * rhs };
		}

		constexpr friend Radians operator*(float lhs, const Radians& rhs) {
			return Radians{ .value = lhs * rhs.value };
		}

		constexpr Radians& operator*=(float rhs) {
			value *= rhs;

			return *this;
		}

		/* ----- SCALAR DIVISION ----- */

		constexpr Radians operator/(float rhs) const {
			return Radians{ .value = value / rhs };
		}

		constexpr friend Radians operator/(float lhs, const Radians& rhs) {
			return Radians{ .value = lhs / rhs.value };
		}

		constexpr Radians& operator/=(float rhs) {
			value /= rhs;

			return *this;
		}

		/* ----- CONVERTERS ----- */

		constexpr Degrees toDegrees() const;

		constexpr explicit operator Degrees() const;
	};

	constexpr Radians Degrees::toRadians() const {
		return Radians{ .value = value * std::numbers::pi_v<float> / 180.0f };
	}

	constexpr Degrees::operator Radians() const {
		return toRadians();
	}

	constexpr Degrees Radians::toDegrees() const {
		return Degrees{ .value = value * 180.0f / std::numbers::pi_v<float> };
	}

	constexpr Radians::operator Degrees() const {
		return toDegrees();
	}

	constexpr Degrees operator""_deg(long double value) {
		return Degrees{ .value = static_cast<float>(value) };
	}

	constexpr Radians operator""_rad(long double value) {
		return Radians{ .value = static_cast<float>(value) };
	}

	template<typename T>
	concept Angle = std::same_as<T, Degrees> || std::same_as<T, Radians>;
}
