#pragma once

#include <variant>

#include "Vulkan.hpp"

namespace Miracle::Infrastructure::Graphics::Vulkan {
	struct SurfaceExtentBounds {
		vk::Extent2D minExtent = {};
		vk::Extent2D maxExtent = {};
	};

	struct SurfaceExtent {
		std::variant<vk::Extent2D, SurfaceExtentBounds> extent = {};
	};
}
