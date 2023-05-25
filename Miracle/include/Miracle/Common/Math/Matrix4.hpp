#pragma once

#include <cstddef>
#include <cmath>
#include <utility>

#include "Angle.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Quaternion.hpp"

namespace Miracle {
	struct Matrix4 {
		float m11 = {};
		float m12 = {};
		float m13 = {};
		float m14 = {};
		float m21 = {};
		float m22 = {};
		float m23 = {};
		float m24 = {};
		float m31 = {};
		float m32 = {};
		float m33 = {};
		float m34 = {};
		float m41 = {};
		float m42 = {};
		float m43 = {};
		float m44 = {};

		static const Matrix4 identity;

		/* ----- COMPARISON ----- */

		constexpr bool operator==(const Matrix4&) const = default;

		/* ----- SIGNED CONVERTION ----- */

		constexpr inline Matrix4 operator+() const {
			return *this;
		}

		constexpr inline Matrix4 operator-() const {
			return Matrix4{
				.m11 = -m11,
				.m12 = -m12,
				.m13 = -m13,
				.m14 = -m14,
				.m21 = -m21,
				.m22 = -m22,
				.m23 = -m23,
				.m24 = -m24,
				.m31 = -m31,
				.m32 = -m32,
				.m33 = -m33,
				.m34 = -m34,
				.m41 = -m41,
				.m42 = -m42,
				.m43 = -m43,
				.m44 = -m44
			};
		}

		/* ----- ADDITION ----- */

		constexpr inline Matrix4 operator+(const Matrix4& rhs) const {
			return Matrix4{
				.m11 = m11 + rhs.m11,
				.m12 = m12 + rhs.m12,
				.m13 = m13 + rhs.m13,
				.m14 = m14 + rhs.m14,
				.m21 = m21 + rhs.m21,
				.m22 = m22 + rhs.m22,
				.m23 = m23 + rhs.m23,
				.m24 = m24 + rhs.m24,
				.m31 = m31 + rhs.m31,
				.m32 = m32 + rhs.m32,
				.m33 = m33 + rhs.m33,
				.m34 = m34 + rhs.m34,
				.m41 = m41 + rhs.m41,
				.m42 = m42 + rhs.m42,
				.m43 = m43 + rhs.m43,
				.m44 = m44 + rhs.m44
			};
		}

		constexpr inline Matrix4& operator+=(const Matrix4& rhs) {
			m11 += rhs.m11;
			m12 += rhs.m12;
			m13 += rhs.m13;
			m14 += rhs.m14;
			m21 += rhs.m21;
			m22 += rhs.m22;
			m23 += rhs.m23;
			m24 += rhs.m24;
			m31 += rhs.m31;
			m32 += rhs.m32;
			m33 += rhs.m33;
			m34 += rhs.m34;
			m41 += rhs.m41;
			m42 += rhs.m42;
			m43 += rhs.m43;
			m44 += rhs.m44;

			return *this;
		}

		/* ----- SUBTRACTION ----- */

		constexpr inline Matrix4 operator-(const Matrix4& rhs) const {
			return Matrix4{
				.m11 = m11 - rhs.m11,
				.m12 = m12 - rhs.m12,
				.m13 = m13 - rhs.m13,
				.m14 = m14 - rhs.m14,
				.m21 = m21 - rhs.m21,
				.m22 = m22 - rhs.m22,
				.m23 = m23 - rhs.m23,
				.m24 = m24 - rhs.m24,
				.m31 = m31 - rhs.m31,
				.m32 = m32 - rhs.m32,
				.m33 = m33 - rhs.m33,
				.m34 = m34 - rhs.m34,
				.m41 = m41 - rhs.m41,
				.m42 = m42 - rhs.m42,
				.m43 = m43 - rhs.m43,
				.m44 = m44 - rhs.m44
			};
		}

		constexpr inline Matrix4& operator-=(const Matrix4& rhs) {
			m11 -= rhs.m11;
			m12 -= rhs.m12;
			m13 -= rhs.m13;
			m14 -= rhs.m14;
			m21 -= rhs.m21;
			m22 -= rhs.m22;
			m23 -= rhs.m23;
			m24 -= rhs.m24;
			m31 -= rhs.m31;
			m32 -= rhs.m32;
			m33 -= rhs.m33;
			m34 -= rhs.m34;
			m41 -= rhs.m41;
			m42 -= rhs.m42;
			m43 -= rhs.m43;
			m44 -= rhs.m44;

			return *this;
		}

		/* ----- MULTIPLICATION ----- */

		constexpr inline Matrix4 operator*(const Matrix4& rhs) const {
			return Matrix4{
				.m11 = m11 * rhs.m11 + m12 * rhs.m21 + m13 * rhs.m31 + m14 * rhs.m41,
				.m12 = m11 * rhs.m12 + m12 * rhs.m22 + m13 * rhs.m32 + m14 * rhs.m42,
				.m13 = m11 * rhs.m13 + m12 * rhs.m23 + m13 * rhs.m33 + m14 * rhs.m43,
				.m14 = m11 * rhs.m14 + m12 * rhs.m24 + m13 * rhs.m34 + m14 * rhs.m44,
				.m21 = m21 * rhs.m11 + m22 * rhs.m21 + m23 * rhs.m31 + m24 * rhs.m41,
				.m22 = m21 * rhs.m12 + m22 * rhs.m22 + m23 * rhs.m32 + m24 * rhs.m42,
				.m23 = m21 * rhs.m13 + m22 * rhs.m23 + m23 * rhs.m33 + m24 * rhs.m43,
				.m24 = m21 * rhs.m14 + m22 * rhs.m24 + m23 * rhs.m34 + m24 * rhs.m44,
				.m31 = m31 * rhs.m11 + m32 * rhs.m21 + m33 * rhs.m31 + m34 * rhs.m41,
				.m32 = m31 * rhs.m12 + m32 * rhs.m22 + m33 * rhs.m32 + m34 * rhs.m42,
				.m33 = m31 * rhs.m13 + m32 * rhs.m23 + m33 * rhs.m33 + m34 * rhs.m43,
				.m34 = m31 * rhs.m14 + m32 * rhs.m24 + m33 * rhs.m34 + m34 * rhs.m44,
				.m41 = m41 * rhs.m11 + m42 * rhs.m21 + m43 * rhs.m31 + m44 * rhs.m41,
				.m42 = m41 * rhs.m12 + m42 * rhs.m22 + m43 * rhs.m32 + m44 * rhs.m42,
				.m43 = m41 * rhs.m13 + m42 * rhs.m23 + m43 * rhs.m33 + m44 * rhs.m43,
				.m44 = m41 * rhs.m14 + m42 * rhs.m24 + m43 * rhs.m34 + m44 * rhs.m44,
			};
		}

		constexpr inline Matrix4& operator*=(const Matrix4& rhs) {
			float newM11 = m11 * rhs.m11 + m12 * rhs.m21 + m13 * rhs.m31 + m14 * rhs.m41;
			float newM12 = m11 * rhs.m12 + m12 * rhs.m22 + m13 * rhs.m32 + m14 * rhs.m42;
			float newM13 = m11 * rhs.m13 + m12 * rhs.m23 + m13 * rhs.m33 + m14 * rhs.m43;
			float newM14 = m11 * rhs.m14 + m12 * rhs.m24 + m13 * rhs.m34 + m14 * rhs.m44;
			float newM21 = m21 * rhs.m11 + m22 * rhs.m21 + m23 * rhs.m31 + m24 * rhs.m41;
			float newM22 = m21 * rhs.m12 + m22 * rhs.m22 + m23 * rhs.m32 + m24 * rhs.m42;
			float newM23 = m21 * rhs.m13 + m22 * rhs.m23 + m23 * rhs.m33 + m24 * rhs.m43;
			float newM24 = m21 * rhs.m14 + m22 * rhs.m24 + m23 * rhs.m34 + m24 * rhs.m44;
			float newM31 = m31 * rhs.m11 + m32 * rhs.m21 + m33 * rhs.m31 + m34 * rhs.m41;
			float newM32 = m31 * rhs.m12 + m32 * rhs.m22 + m33 * rhs.m32 + m34 * rhs.m42;
			float newM33 = m31 * rhs.m13 + m32 * rhs.m23 + m33 * rhs.m33 + m34 * rhs.m43;
			float newM34 = m31 * rhs.m14 + m32 * rhs.m24 + m33 * rhs.m34 + m34 * rhs.m44;
			float newM41 = m41 * rhs.m11 + m42 * rhs.m21 + m43 * rhs.m31 + m44 * rhs.m41;
			float newM42 = m41 * rhs.m12 + m42 * rhs.m22 + m43 * rhs.m32 + m44 * rhs.m42;
			float newM43 = m41 * rhs.m13 + m42 * rhs.m23 + m43 * rhs.m33 + m44 * rhs.m43;
			float newM44 = m41 * rhs.m14 + m42 * rhs.m24 + m43 * rhs.m34 + m44 * rhs.m44;

			m11 = newM11;
			m12 = newM12;
			m13 = newM13;
			m14 = newM14;
			m21 = newM21;
			m22 = newM22;
			m23 = newM23;
			m24 = newM24;
			m31 = newM31;
			m32 = newM32;
			m33 = newM33;
			m34 = newM34;
			m41 = newM41;
			m42 = newM42;
			m43 = newM43;
			m44 = newM44;

			return *this;
		}

		/* ----- VECTOR MULTIPLICATION ----- */

		constexpr inline Vector4 operator*(const Vector4& rhs) const {
			return Vector4{
				.x = m11 * rhs.x + m12 * rhs.y + m13 * rhs.z + m14 * rhs.w,
				.y = m21 * rhs.x + m22 * rhs.y + m23 * rhs.z + m24 * rhs.w,
				.z = m31 * rhs.x + m32 * rhs.y + m33 * rhs.z + m34 * rhs.w,
				.w = m41 * rhs.x + m42 * rhs.y + m43 * rhs.z + m44 * rhs.w
			};
		}

		constexpr inline friend Vector4 operator*(const Vector4& lhs, const Matrix4& rhs) {
			return Vector4{
				.x = lhs.x * rhs.m11 + lhs.y * rhs.m21 + lhs.z * rhs.m31 + lhs.w * rhs.m41,
				.y = lhs.x * rhs.m12 + lhs.y * rhs.m22 + lhs.z * rhs.m32 + lhs.w * rhs.m42,
				.z = lhs.x * rhs.m13 + lhs.y * rhs.m23 + lhs.z * rhs.m33 + lhs.w * rhs.m43,
				.w = lhs.x * rhs.m14 + lhs.y * rhs.m24 + lhs.z * rhs.m34 + lhs.w * rhs.m44
			};
		}

		constexpr inline friend Vector4& operator*=(Vector4& lhs, const Matrix4& rhs) {
			float newX = lhs.x * rhs.m11 + lhs.y * rhs.m21 + lhs.z * rhs.m31 + lhs.w * rhs.m41;
			float newY = lhs.x * rhs.m12 + lhs.y * rhs.m22 + lhs.z * rhs.m32 + lhs.w * rhs.m42;
			float newZ = lhs.x * rhs.m13 + lhs.y * rhs.m23 + lhs.z * rhs.m33 + lhs.w * rhs.m43;
			float newW = lhs.x * rhs.m14 + lhs.y * rhs.m24 + lhs.z * rhs.m34 + lhs.w * rhs.m44;

			lhs.x = newX;
			lhs.y = newY;
			lhs.z = newZ;
			lhs.w = newW;

			return lhs;
		}

		/* ----- SCALAR MULTIPLICATION ----- */

		constexpr inline Matrix4 operator*(float rhs) const {
			return Matrix4{
				.m11 = m11 * rhs,
				.m12 = m12 * rhs,
				.m13 = m13 * rhs,
				.m14 = m14 * rhs,
				.m21 = m21 * rhs,
				.m22 = m22 * rhs,
				.m23 = m23 * rhs,
				.m24 = m24 * rhs,
				.m31 = m31 * rhs,
				.m32 = m32 * rhs,
				.m33 = m33 * rhs,
				.m34 = m34 * rhs,
				.m41 = m41 * rhs,
				.m42 = m42 * rhs,
				.m43 = m43 * rhs,
				.m44 = m44 * rhs
			};
		}

		constexpr inline friend Matrix4 operator*(float lhs, const Matrix4& rhs) {
			return Matrix4{
				.m11 = lhs * rhs.m11,
				.m12 = lhs * rhs.m12,
				.m13 = lhs * rhs.m13,
				.m14 = lhs * rhs.m14,
				.m21 = lhs * rhs.m21,
				.m22 = lhs * rhs.m22,
				.m23 = lhs * rhs.m23,
				.m24 = lhs * rhs.m24,
				.m31 = lhs * rhs.m31,
				.m32 = lhs * rhs.m32,
				.m33 = lhs * rhs.m33,
				.m34 = lhs * rhs.m34,
				.m41 = lhs * rhs.m41,
				.m42 = lhs * rhs.m42,
				.m43 = lhs * rhs.m43,
				.m44 = lhs * rhs.m44
			};
		}

		constexpr inline Matrix4& operator*=(float rhs) {
			m11 *= rhs;
			m12 *= rhs;
			m13 *= rhs;
			m14 *= rhs;
			m21 *= rhs;
			m22 *= rhs;
			m23 *= rhs;
			m24 *= rhs;
			m31 *= rhs;
			m32 *= rhs;
			m33 *= rhs;
			m34 *= rhs;
			m41 *= rhs;
			m42 *= rhs;
			m43 *= rhs;
			m44 *= rhs;

			return *this;
		}

		/* ----- SCALAR DIVISION ----- */

		constexpr inline Matrix4 operator/(float rhs) const {
			return Matrix4{
				.m11 = m11 / rhs,
				.m12 = m12 / rhs,
				.m13 = m13 / rhs,
				.m14 = m14 / rhs,
				.m21 = m21 / rhs,
				.m22 = m22 / rhs,
				.m23 = m23 / rhs,
				.m24 = m24 / rhs,
				.m31 = m31 / rhs,
				.m32 = m32 / rhs,
				.m33 = m33 / rhs,
				.m34 = m34 / rhs,
				.m41 = m41 / rhs,
				.m42 = m42 / rhs,
				.m43 = m43 / rhs,
				.m44 = m44 / rhs
			};
		}

		constexpr inline friend Matrix4 operator/(float lhs, const Matrix4& rhs) {
			return Matrix4{
				.m11 = lhs / rhs.m11,
				.m12 = lhs / rhs.m12,
				.m13 = lhs / rhs.m13,
				.m14 = lhs / rhs.m14,
				.m21 = lhs / rhs.m21,
				.m22 = lhs / rhs.m22,
				.m23 = lhs / rhs.m23,
				.m24 = lhs / rhs.m24,
				.m31 = lhs / rhs.m31,
				.m32 = lhs / rhs.m32,
				.m33 = lhs / rhs.m33,
				.m34 = lhs / rhs.m34,
				.m41 = lhs / rhs.m41,
				.m42 = lhs / rhs.m42,
				.m43 = lhs / rhs.m43,
				.m44 = lhs / rhs.m44
			};
		}

		constexpr inline Matrix4& operator/=(float rhs) {
			m11 /= rhs;
			m12 /= rhs;
			m13 /= rhs;
			m14 /= rhs;
			m21 /= rhs;
			m22 /= rhs;
			m23 /= rhs;
			m24 /= rhs;
			m31 /= rhs;
			m32 /= rhs;
			m33 /= rhs;
			m34 /= rhs;
			m41 /= rhs;
			m42 /= rhs;
			m43 /= rhs;
			m44 /= rhs;

			return *this;
		}

		/* ----- TRANSFORMATION ----- */

		static constexpr inline Matrix4 createTranslation(const Vector3& translation) {
			return Matrix4{
				.m11 = 1.0f,
				.m12 = 0.0f,
				.m13 = 0.0f,
				.m14 = 0.0f,
				.m21 = 0.0f,
				.m22 = 1.0f,
				.m23 = 0.0f,
				.m24 = 0.0f,
				.m31 = 0.0f,
				.m32 = 0.0f,
				.m33 = 1.0f,
				.m34 = 0.0f,
				.m41 = translation.x,
				.m42 = translation.y,
				.m43 = translation.z,
				.m44 = 1.0f
			};
		}

		template<Angle TAngle>
		static inline Matrix4 createRotationX(TAngle angle) {
			float radians = static_cast<Radians>(angle).value;

			return Matrix4{
				.m11 =  1.0f,
				.m12 =  0.0f,
				.m13 =  0.0f,
				.m14 =  0.0f,
				.m21 =  0.0f,
				.m22 =  std::cos(radians),
				.m23 =  std::sin(radians),
				.m24 =  0.0f,
				.m31 =  0.0f,
				.m32 = -std::sin(radians),
				.m33 =  std::cos(radians),
				.m34 =  0.0f,
				.m41 =  0.0f,
				.m42 =  0.0f,
				.m43 =  0.0f,
				.m44 =  1.0f
			};
		}

		template<Angle TAngle>
		static inline Matrix4 createRotationY(TAngle angle) {
			float radians = static_cast<Radians>(angle).value;

			return Matrix4{
				.m11 =  std::cos(radians),
				.m12 =  0.0f,
				.m13 = -std::sin(radians),
				.m14 =  0.0f,
				.m21 =  0.0f,
				.m22 =  1.0f,
				.m23 =  0.0f,
				.m24 =  0.0f,
				.m31 =  std::sin(radians),
				.m32 =  0.0f,
				.m33 =  std::cos(radians),
				.m34 =  0.0f,
				.m41 =  0.0f,
				.m42 =  0.0f,
				.m43 =  0.0f,
				.m44 =  1.0f
			};
		}

		template<Angle TAngle>
		static inline Matrix4 createRotationZ(TAngle angle) {
			float radians = static_cast<Radians>(angle).value;

			return Matrix4{
				.m11 =  std::cos(radians),
				.m12 =  std::sin(radians),
				.m13 =  0.0f,
				.m14 =  0.0f,
				.m21 = -std::sin(radians),
				.m22 =  std::cos(radians),
				.m23 =  0.0f,
				.m24 =  0.0f,
				.m31 =  0.0f,
				.m32 =  0.0f,
				.m33 =  1.0f,
				.m34 =  0.0f,
				.m41 =  0.0f,
				.m42 =  0.0f,
				.m43 =  0.0f,
				.m44 =  1.0f
			};
		}

		template<Angle TAngle>
		static inline Matrix4 createRotation(const Vector3& axis, TAngle angle) {
			float radians = static_cast<Radians>(angle).value;
			float cosAngle = std::cos(radians);
			float sinAngle = std::sin(radians);
			float oneMinusCosAngle = 1.0f - cosAngle;

			return Matrix4{
				.m11 = axis.x * axis.x * oneMinusCosAngle + cosAngle,
				.m12 = axis.x * axis.y * oneMinusCosAngle + axis.z * sinAngle,
				.m13 = axis.x * axis.z * oneMinusCosAngle - axis.y * sinAngle,
				.m14 = 0.0f,
				.m21 = axis.x * axis.y * oneMinusCosAngle - axis.z * sinAngle,
				.m22 = axis.y * axis.y * oneMinusCosAngle + cosAngle,
				.m23 = axis.y * axis.z * oneMinusCosAngle + axis.x * sinAngle,
				.m24 = 0.0f,
				.m31 = axis.x * axis.z * oneMinusCosAngle + axis.y * sinAngle,
				.m32 = axis.y * axis.z * oneMinusCosAngle - axis.x * sinAngle,
				.m33 = axis.z * axis.z * oneMinusCosAngle + cosAngle,
				.m34 = 0.0f,
				.m41 = 0.0f,
				.m42 = 0.0f,
				.m43 = 0.0f,
				.m44 = 1.0f
			};
		}

		static constexpr inline Matrix4 createRotation(const Quaternion& quaternion) {
			auto& w = quaternion.w;
			auto& x = quaternion.v.x;
			auto& y = quaternion.v.y;
			auto& z = quaternion.v.z;

			return Matrix4{
				.m11 = 1.0f - 2.0f * y * y - 2.0f * z * z,
				.m12 = 2.0f * x * y + 2.0f * w * z,
				.m13 = 2.0f * x * z - 2.0f * w * y,
				.m14 = 0.0f,
				.m21 = 2.0f * x * y - 2.0f * w * z,
				.m22 = 1.0f - 2.0f * z * z - 2.0f * x * x,
				.m23 = 2.0f * y * z + 2.0f * w * x,
				.m24 = 0.0f,
				.m31 = 2.0f * x * z + 2.0f * w * y,
				.m32 = 2.0f * y * z - 2.0f * w * x,
				.m33 = 1.0f - 2.0f * x * x - 2.0f * y * y,
				.m34 = 0.0f,
				.m41 = 0.0f,
				.m42 = 0.0f,
				.m43 = 0.0f,
				.m44 = 1.0f
			};
		}

		static constexpr inline Matrix4 createScale(const Vector3& scale) {
			return Matrix4{
				.m11 = scale.x,
				.m12 = 0.0f,
				.m13 = 0.0f,
				.m14 = 0.0f,
				.m21 = 0.0f,
				.m22 = scale.y,
				.m23 = 0.0f,
				.m24 = 0.0f,
				.m31 = 0.0f,
				.m32 = 0.0f,
				.m33 = scale.z,
				.m34 = 0.0f,
				.m41 = 0.0f,
				.m42 = 0.0f,
				.m43 = 0.0f,
				.m44 = 1.0f
			};
		}

		static constexpr inline Matrix4 createScale(float uniformScale) {
			return Matrix4{
				.m11 = uniformScale,
				.m12 = 0.0f,
				.m13 = 0.0f,
				.m14 = 0.0f,
				.m21 = 0.0f,
				.m22 = uniformScale,
				.m23 = 0.0f,
				.m24 = 0.0f,
				.m31 = 0.0f,
				.m32 = 0.0f,
				.m33 = uniformScale,
				.m34 = 0.0f,
				.m41 = 0.0f,
				.m42 = 0.0f,
				.m43 = 0.0f,
				.m44 = 1.0f
			};
		}

		static constexpr inline Matrix4 createOrthographicProjection(float aspectRatio, float zoom) {
			return Matrix4{
				.m11 = zoom / aspectRatio,
				.m12 = 0.0f,
				.m13 = 0.0f,
				.m14 = 0.0f,
				.m21 = 0.0f,
				.m22 = zoom,
				.m23 = 0.0f,
				.m24 = 0.0f,
				.m31 = 0.0f,
				.m32 = 0.0f,
				.m33 = 0.0f,
				.m34 = 0.0f,
				.m41 = 0.0f,
				.m42 = 0.0f,
				.m43 = 0.0f,
				.m44 = 1.0f
			};
		}

		/* ----- MISC. ----- */

		constexpr inline Matrix4 toTransposed() const {
			return Matrix4{
				.m11 = m11,
				.m12 = m21,
				.m13 = m31,
				.m14 = m41,
				.m21 = m12,
				.m22 = m22,
				.m23 = m32,
				.m24 = m42,
				.m31 = m13,
				.m32 = m23,
				.m33 = m33,
				.m34 = m43,
				.m41 = m14,
				.m42 = m24,
				.m43 = m34,
				.m44 = m44
			};
		}

		constexpr inline Matrix4& transpose() {
			std::swap(m12, m21);
			std::swap(m13, m31);
			std::swap(m14, m41);
			std::swap(m23, m32);
			std::swap(m24, m42);
			std::swap(m34, m43);

			return *this;
		}
	};
}
