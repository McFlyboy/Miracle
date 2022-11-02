#pragma once

#include <cstdint>
#include <array>

namespace Miracle {
	struct Face {
		std::array<uint32_t, 3> indices = {};
	};
}
