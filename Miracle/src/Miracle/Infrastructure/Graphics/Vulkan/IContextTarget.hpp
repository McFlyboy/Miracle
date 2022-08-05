#pragma once

#include <span>

#include "Vulkan.hpp"

namespace Miracle::Infrastructure::Graphics::Vulkan {
	class IContextTarget {
	public:
		virtual ~IContextTarget() = default;

		virtual std::span<const char*> getRequiredVulkanExtensionNames() const = 0;

		virtual vk::raii::SurfaceKHR createVulkanSurface(
			vk::raii::Instance& instance
		) const = 0;
	};
}
