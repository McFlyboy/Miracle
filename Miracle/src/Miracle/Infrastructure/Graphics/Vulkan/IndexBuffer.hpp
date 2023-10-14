#pragma once

#include <vector>

#include <Miracle/Common/Models/Face.hpp>
#include <Miracle/Application/ILogger.hpp>
#include <Miracle/Application/Graphics/IIndexBuffer.hpp>
#include "Vulkan.hpp"
#include "Vma.hpp"
#include "GraphicsContext.hpp"

namespace Miracle::Infrastructure::Graphics::Vulkan {
	class IndexBuffer : public Application::IIndexBuffer {
	private:
		Application::ILogger& m_logger;
		GraphicsContext& m_context;

		vk::Buffer m_buffer = nullptr;
		vma::Allocation m_allocation = nullptr;
		uint32_t m_indexCount = 0;

	public:
		IndexBuffer(
			Application::ILogger& logger,
			GraphicsContext& context,
			const std::vector<Face>& faces
		);

		~IndexBuffer();

		virtual uint32_t getIndexCount() const override { return m_indexCount; }

		virtual void bind() override;
	};
}
