#pragma once

#include "Vulkan.hpp"
#include "ISurfaceTarget.hpp"
#include "Instance.hpp"

namespace Miracle::Graphics::Implementations::Vulkan {
	class Surface {
	private:
		const ISurfaceTarget& m_surfaceTarget;

		vk::raii::SurfaceKHR m_surface = nullptr;

	public:
		Surface(const Instance& instance);

		inline const vk::raii::SurfaceKHR& getRawSurface() const { return m_surface; }

		inline const ISurfaceTarget& getSurfaceTarget() const { return m_surfaceTarget; }
	};
}
