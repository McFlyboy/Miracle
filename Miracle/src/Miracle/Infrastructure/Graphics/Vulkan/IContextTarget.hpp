#pragma once

#include <span>

#include <Miracle/Application/Graphics/IContextTarget.hpp>
#include "Vulkan.hpp"

namespace Miracle::Infrastructure::Graphics::Vulkan {
	class IContextTarget : public Application::IContextTarget {
	public:
		virtual ~IContextTarget() = default;

		virtual std::span<const char*> getRequiredVulkanExtensionNames() const = 0;

		virtual vk::raii::SurfaceKHR createVulkanSurface(
			vk::raii::Instance& instance
		) const = 0;

		virtual vk::Extent2D getCurrentVulkanExtent() const = 0;
	};
}
