#pragma once

#include <vector>
#include <utility>

#include <Miracle/Common/Math/Vector2f.hpp>
#include <Miracle/Application/ILogger.hpp>
#include <Miracle/Application/Graphics/IVertexBuffer.hpp>
#include "Vulkan.hpp"
#include "Vma.hpp"
#include "GraphicsContext.hpp"

namespace Miracle::Infrastructure::Graphics::Vulkan {
	class VertexBuffer : public Application::IVertexBuffer {
	private:
		Application::ILogger& m_logger;
		GraphicsContext& m_context;

		vk::Buffer m_buffer = nullptr;
		vma::Allocation m_allocation = nullptr;
		uint32_t m_vertexCount = 0;

	public:
		VertexBuffer(
			Application::ILogger& logger,
			GraphicsContext& context,
			const std::vector<Vector2f>& vertices
		);

		~VertexBuffer();

		inline virtual uint32_t getVertexCount() const override { return m_vertexCount; }

		virtual void bind() override;

	private:
		std::pair<vk::Buffer, vma::Allocation> createStagingBuffer(vk::DeviceSize bufferSize) const;

		std::pair<vk::Buffer, vma::Allocation> createBuffer(vk::DeviceSize bufferSize) const;

		void copyBuffer(vk::Buffer destination, vk::Buffer source, vk::DeviceSize size) const;
	};
}
