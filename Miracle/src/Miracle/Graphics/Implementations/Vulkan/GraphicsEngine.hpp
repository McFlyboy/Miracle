#pragma once

#include <Miracle/Graphics/IGraphicsEngine.hpp>
#include <Miracle/Io/ResourceLoader.hpp>
#include "Vulkan.hpp"
#include "ISurfaceTarget.hpp"
#include "Instance.hpp"
#include "Surface.hpp"
#include "Device.hpp"
#include "Swapchain.hpp"
#include "GraphicsPipeline.hpp"

namespace Miracle::Graphics::Implementations::Vulkan {
	class GraphicsEngine : public IGraphicsEngine {
	private:
		vk::raii::Context m_context;
		Instance m_instance;
		Surface m_surface;
		Device m_device;
		Swapchain m_swapchain;
		GraphicsPipeline m_graphicsPipeline;

	public:
		GraphicsEngine(
			const ISurfaceTarget& surfaceTarget,
			const Io::ResourceLoader& resourceLoader
		);

		~GraphicsEngine();
	};
}
