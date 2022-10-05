#include "VertexBuffer.hpp"

#include <exception>

#include <fmt/format.h>

namespace Miracle::Infrastructure::Graphics::Vulkan {
	VertexBuffer::VertexBuffer(
		Application::ILogger& logger,
		GraphicsContext& context
	) :
		m_logger(logger),
		m_context(context)
	{
		try {
			m_buffer = m_context.getDevice().createBuffer(
				vk::BufferCreateInfo{
					.flags                 = {},
					.size                  = static_cast<vk::DeviceSize>(sizeof(float) * 6),
					.usage                 = vk::BufferUsageFlagBits::eVertexBuffer,
					.sharingMode           = vk::SharingMode::eExclusive,
					.queueFamilyIndexCount = 0,
					.pQueueFamilyIndices   = nullptr
				}
			);
		}
		catch (const std::exception& e) {
			m_logger.error(fmt::format("Failed to create Vulkan vertex buffer.\n{}", e.what()));
			throw Application::VertexBufferErrors::CreationError();
		}
	}
}
