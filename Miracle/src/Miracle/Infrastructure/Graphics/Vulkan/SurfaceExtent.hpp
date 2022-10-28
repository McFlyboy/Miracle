#pragma once

#include <optional>

#include "Vulkan.hpp"

namespace Miracle::Infrastructure::Graphics::Vulkan {
	struct SurfaceExtent {
		std::optional<vk::Extent2D> extent = {};
		vk::Extent2D minExtent = {};
		vk::Extent2D maxExtent = {};
	};
}
