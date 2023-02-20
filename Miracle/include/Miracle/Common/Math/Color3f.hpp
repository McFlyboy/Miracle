#pragma once

#include <cstdint>
#include <compare>

namespace Miracle {
	struct Color3f {
		float red = {};
		float green = {};
		float blue = {};

		/* ----- COMPARISON ----- */

		constexpr auto operator<=>(const Color3f&) const = default;

		/* ----- MISC. ----- */

		// Returns color-code in format #AARRGGBB
		constexpr inline uint32_t toColorCode() const {
			return (static_cast<int>(red   * 255.0f) << 16)
				 + (static_cast<int>(green * 255.0f) <<  8)
				 +  static_cast<int>(blue  * 255.0f);
		}

		// Creates Color3f from color-code in format #AARRGGBB
		static constexpr inline Color3f createFromColorCode(uint32_t colorCode) {
			return Color3f{
				.red   = static_cast<float>((colorCode >> 16) & 0xffu) / 255.0f,
				.green = static_cast<float>((colorCode >>  8) & 0xffu) / 255.0f,
				.blue  = static_cast<float>( colorCode        & 0xffu) / 255.0f,
			};
		}
	};
}
