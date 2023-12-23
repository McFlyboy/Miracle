#pragma once

#include <cstdint>
#include <cmath>

namespace Miracle {
	// Linear RGB color
	struct ColorRgb {
		float redChannel = {};
		float greenChannel = {};
		float blueChannel = {};

		/* ----- COMPARISON ----- */

		constexpr bool operator==(const ColorRgb&) const = default;

		/* ----- CONVERTERS ----- */

		// Creates ColorRgb converted from non-linear sRGB color channels
		static ColorRgb createFromNonlinearSrgb(float redChannel, float greenChannel, float blueChannel) {
			return ColorRgb{
				.redChannel   = redChannel   > 0.04045f
					? std::pow((redChannel   + 0.055f) / 1.055f, 2.4f)
					: redChannel   / 12.92f,
				.greenChannel = greenChannel > 0.04045f
					? std::pow((greenChannel + 0.055f) / 1.055f, 2.4f)
					: greenChannel / 12.92f,
				.blueChannel  = blueChannel  > 0.04045f
					? std::pow((blueChannel  + 0.055f) / 1.055f, 2.4f)
					: blueChannel  / 12.92f
			};
		}

		// Creates ColorRgb from non-linear sRGB color-code in format #RRGGBB
		static ColorRgb createFromNonlinearSrgbColorCode(uint32_t colorCode) {
			return createFromNonlinearSrgb(
				static_cast<float>((colorCode >> 16u) & 0xffu) / 255.0f,
				static_cast<float>((colorCode >>  8u) & 0xffu) / 255.0f,
				static_cast<float>( colorCode         & 0xffu) / 255.0f
			);
		}
	};

	class ColorRgbs {
	public:
		ColorRgbs() = delete;

		static inline const ColorRgb red     = ColorRgb::createFromNonlinearSrgbColorCode(0xFF0000);
		static inline const ColorRgb green   = ColorRgb::createFromNonlinearSrgbColorCode(0x00FF00);
		static inline const ColorRgb blue    = ColorRgb::createFromNonlinearSrgbColorCode(0x0000FF);
		static inline const ColorRgb cyan    = ColorRgb::createFromNonlinearSrgbColorCode(0x00FFFF);
		static inline const ColorRgb magenta = ColorRgb::createFromNonlinearSrgbColorCode(0xFF00FF);
		static inline const ColorRgb yellow  = ColorRgb::createFromNonlinearSrgbColorCode(0xFFFF00);
		static inline const ColorRgb black   = ColorRgb::createFromNonlinearSrgbColorCode(0x000000);
		static inline const ColorRgb white   = ColorRgb::createFromNonlinearSrgbColorCode(0xFFFFFF);
	};
}
