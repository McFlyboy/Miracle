#pragma once

#include <vector>
#include <variant>

#include <Miracle/MiracleError.hpp>
#include "Vulkan.hpp"

namespace Miracle::Graphics::Implementations::Vulkan {
	class ISurfaceTarget {
	protected:
		bool m_extentChanged = false;

	public:
		virtual ~ISurfaceTarget() = default;

		virtual std::vector<const char*> getRequiredInstanceExtensions() const = 0;

		virtual vk::Extent2D getCurrentExtent() const = 0;

		virtual std::variant<MiracleError, vk::raii::SurfaceKHR> createSurface(
			const vk::raii::Instance& instance
		) const = 0;

		inline bool isExtentChanged() {
			bool extentChanged = m_extentChanged;
			m_extentChanged = false;
			return extentChanged;
		}

		inline void markExtentChanged() { m_extentChanged = true; }
	};
}
