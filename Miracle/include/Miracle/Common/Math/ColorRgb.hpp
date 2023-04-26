#pragma once

#include <cstdint>

namespace Miracle {
	struct ColorRgb {
		float redChannel = {};
		float greenChannel = {};
		float blueChannel = {};

		static const ColorRgb red;
		static const ColorRgb green;
		static const ColorRgb blue;
		static const ColorRgb cyan;
		static const ColorRgb magenta;
		static const ColorRgb yellow;
		static const ColorRgb black;
		static const ColorRgb white;

		/* ----- COMPARISON ----- */

		constexpr bool operator==(const ColorRgb&) const = default;

		/* ----- CONVERTERS ----- */

		// Returns color-code in format #RRGGBB
		constexpr inline uint32_t toColorCode() const {
			return (static_cast<int>(redChannel   * 255.0f) << 16)
				 + (static_cast<int>(greenChannel * 255.0f) <<  8)
				 +  static_cast<int>(blueChannel  * 255.0f);
		}

		// Creates ColorRgb from color-code in format #RRGGBB
		static constexpr inline ColorRgb createFromColorCode(uint32_t colorCode) {
			return ColorRgb{
				.redChannel   = static_cast<float>((colorCode >> 16) & 0xffu) / 255.0f,
				.greenChannel = static_cast<float>((colorCode >>  8) & 0xffu) / 255.0f,
				.blueChannel  = static_cast<float>( colorCode        & 0xffu) / 255.0f,
			};
		}
	};
}
