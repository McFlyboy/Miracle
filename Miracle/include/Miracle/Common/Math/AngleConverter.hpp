#pragma once

#include <numbers>

namespace Miracle {
	class AngleConverter {
	public:
		AngleConverter() = delete;

		static constexpr inline float toRadians(float degrees) {
			return degrees * std::numbers::pi_v<float> / 180.0f;
		}

		static constexpr inline float toDegrees(float radians) {
			return radians * 180.0f / std::numbers::pi_v<float>;
		}
	};
}
