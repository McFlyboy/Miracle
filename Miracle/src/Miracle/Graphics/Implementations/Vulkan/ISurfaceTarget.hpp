#pragma once

#include <vector>
#include <variant>

#include <Miracle/MiracleError.hpp>
#include "Vulkan.hpp"

namespace Miracle::Graphics::Implementations::Vulkan {
	class ISurfaceTarget {
	public:
		virtual ~ISurfaceTarget() = default;

		virtual std::vector<const char*> getRequiredInstanceExtensions() const = 0;

		virtual vk::Extent2D getCurrentExtent() const = 0;

		virtual std::variant<MiracleError, vk::raii::SurfaceKHR> createSurface(
			const vk::raii::Instance& instance
		) const = 0;
	};
}
