#include "GraphicsEngine.hpp"

#include <Miracle/components/Miracle/Diagnostics/Logger.hpp>

using namespace Miracle::Diagnostics;

namespace Miracle::Graphics::Implementations::Vulkan {
	GraphicsEngine::GraphicsEngine(
		const ISurfaceTarget& surfaceTarget,
		const Io::ResourceLoader& resourceLoader
	) :
		m_instance(m_context, surfaceTarget),
		m_surface(m_instance),
		m_device(m_instance, m_surface),
		m_swapchain(m_device, m_surface),
		m_graphicsPipeline(m_device, resourceLoader)
	{
		Logger::info("Vulkan graphics engine created");
	}

	GraphicsEngine::~GraphicsEngine() {
		Logger::info("Destroying Vulkan graphics engine...");
	}
}
