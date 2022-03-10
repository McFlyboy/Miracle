#include "Surface.hpp"

#include <utility>

namespace Miracle::Graphics::Implementations::Vulkan {
	Surface::Surface(const Instance& instance) :
		m_surfaceTarget(instance.getSurfaceTarget())
	{
		auto result = instance.createSurface();

		if (result.index() == 0) {
			throw std::get<MiracleError>(result);
		}

		m_surface = std::move(std::get<vk::raii::SurfaceKHR>(result));
	}
}
