#include "GraphicsEngine.hpp"

#include <Miracle/components/Miracle/Diagnostics/Logger.hpp>

using namespace Miracle::Diagnostics;

namespace Miracle::Graphics::Implementations::Vulkan {
	GraphicsEngine::GraphicsEngine(const ISurfaceTarget& surfaceTarget) :
		m_instance(m_context, surfaceTarget),
		m_surface(m_instance),
		m_device(m_instance, m_surface)
	{
		Logger::info("Vulkan graphics engine created");
	}

	GraphicsEngine::~GraphicsEngine() {
		Logger::info("Vulkan graphics engine destroyed");
	}
}
