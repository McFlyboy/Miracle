#pragma once

#include <cstdint>

namespace Miracle {
	struct ColorRgb {
		float redChannel = {};
		float greenChannel = {};
		float blueChannel = {};

		/* ----- COMPARISON ----- */

		constexpr bool operator==(const ColorRgb&) const = default;

		/* ----- CONVERTERS ----- */

		// Returns color-code in format #RRGGBB
		constexpr uint32_t toColorCode() const {
			return (static_cast<uint32_t>(redChannel   * 255.0f) << 16u)
				 + (static_cast<uint32_t>(greenChannel * 255.0f) <<  8u)
				 +  static_cast<uint32_t>(blueChannel  * 255.0f);
		}

		// Creates ColorRgb from color-code in format #RRGGBB
		static constexpr ColorRgb createFromColorCode(uint32_t colorCode) {
			return ColorRgb{
				.redChannel   = static_cast<float>((colorCode >> 16u) & 0xffu) / 255.0f,
				.greenChannel = static_cast<float>((colorCode >>  8u) & 0xffu) / 255.0f,
				.blueChannel  = static_cast<float>( colorCode         & 0xffu) / 255.0f,
			};
		}
	};

	class ColorRgbs {
	public:
		ColorRgbs() = delete;

		static constexpr ColorRgb red     = ColorRgb::createFromColorCode(0xFF0000);
		static constexpr ColorRgb green   = ColorRgb::createFromColorCode(0x00FF00);
		static constexpr ColorRgb blue    = ColorRgb::createFromColorCode(0x0000FF);
		static constexpr ColorRgb cyan    = ColorRgb::createFromColorCode(0x00FFFF);
		static constexpr ColorRgb magenta = ColorRgb::createFromColorCode(0xFF00FF);
		static constexpr ColorRgb yellow  = ColorRgb::createFromColorCode(0xFFFF00);
		static constexpr ColorRgb black   = ColorRgb::createFromColorCode(0x000000);
		static constexpr ColorRgb white   = ColorRgb::createFromColorCode(0xFFFFFF);
	};
}
