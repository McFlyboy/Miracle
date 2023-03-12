#pragma once

#include <cstddef>

#include "Vector3.hpp"

namespace Miracle {
	struct Vector4 {
		float x = {};
		float y = {};
		float z = {};
		float w = {};

		/* ----- ARRAY-STYLE ACCESSING ----- */

		inline float& operator[](size_t index) {
			return *(reinterpret_cast<float*>(this) + index);
		}

		inline float operator[](size_t index) const {
			return *(reinterpret_cast<const float*>(this) + index);
		}

		/* ----- COMPARISON ----- */

		constexpr bool operator==(const Vector4&) const = default;

		/* ----- CONVERTERS ----- */

		static constexpr inline Vector4 createFromVector3(const Vector3& xyz, float w) {
			return Vector4{
				.x = xyz.x,
				.y = xyz.y,
				.z = xyz.z,
				.w = w
			};
		}
	};
}
