#pragma once

#include "Vector3.hpp"

namespace Miracle {
	struct Vector4 {
		float x = {};
		float y = {};
		float z = {};
		float w = {};

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
