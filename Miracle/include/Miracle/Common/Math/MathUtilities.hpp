#pragma once

#include "Vector3.hpp"
#include "Quaternion.hpp"

namespace Miracle {
	class MathUtilities {
	public:
		MathUtilities() = delete;

		static Vector3 rotateVector(const Vector3& vector, const Quaternion& rotation) {
			return (rotation * Quaternion{ .w = 0.0f, .v = vector } * rotation.getInverse()).v;
		}
	};
}
