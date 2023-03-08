#include <Miracle/Common/Math/ColorRgb.hpp>

namespace Miracle {
	const ColorRgb ColorRgb::red     = ColorRgb{ .redChannel = 1.0f, .greenChannel = 0.0f, .blueChannel = 0.0f };
	const ColorRgb ColorRgb::green   = ColorRgb{ .redChannel = 0.0f, .greenChannel = 1.0f, .blueChannel = 0.0f };
	const ColorRgb ColorRgb::blue    = ColorRgb{ .redChannel = 0.0f, .greenChannel = 0.0f, .blueChannel = 1.0f };
	const ColorRgb ColorRgb::cyan    = ColorRgb{ .redChannel = 0.0f, .greenChannel = 1.0f, .blueChannel = 1.0f };
	const ColorRgb ColorRgb::magenta = ColorRgb{ .redChannel = 1.0f, .greenChannel = 0.0f, .blueChannel = 1.0f };
	const ColorRgb ColorRgb::yellow  = ColorRgb{ .redChannel = 1.0f, .greenChannel = 1.0f, .blueChannel = 0.0f };
	const ColorRgb ColorRgb::black   = ColorRgb{ .redChannel = 0.0f, .greenChannel = 0.0f, .blueChannel = 0.0f };
	const ColorRgb ColorRgb::white   = ColorRgb{ .redChannel = 1.0f, .greenChannel = 1.0f, .blueChannel = 1.0f };
}
