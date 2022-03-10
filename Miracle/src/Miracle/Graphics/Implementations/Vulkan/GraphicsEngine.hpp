#pragma once

#include "Vulkan.hpp"
#include "ISurfaceTarget.hpp"
#include "Instance.hpp"
#include "Surface.hpp"
#include "Device.hpp"

namespace Miracle::Graphics::Implementations::Vulkan {
	class GraphicsEngine {
	private:
		vk::raii::Context m_context;
		Instance m_instance;
		Surface m_surface;
		Device m_device;

	public:
		GraphicsEngine(const ISurfaceTarget& surfaceTarget);
		GraphicsEngine(const GraphicsEngine&) = delete;
		~GraphicsEngine();
	};
}
