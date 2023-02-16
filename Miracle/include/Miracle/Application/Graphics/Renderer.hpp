#pragma once

#include <memory>

#include <Miracle/Common/Math/Color3f.hpp>
#include <Miracle/Common/Math/Vector2f.hpp>
#include <Miracle/Common/Models/Mesh.hpp>
#include <Miracle/Application/Models/Scene.hpp>
#include <Miracle/Application/ILogger.hpp>
#include <Miracle/Application/IFileAccess.hpp>
#include "IGraphicsApi.hpp"
#include "IGraphicsContext.hpp"
#include "ISwapchain.hpp"
#include "IGraphicsPipeline.hpp"
#include "IVertexBuffer.hpp"
#include "IIndexBuffer.hpp"
#include "PushConstants.hpp"

namespace Miracle::Application {
	struct RendererInitProps{
		SwapchainInitProps swapchainInitProps = {};
		const Mesh& mesh = {};
	};

	class Renderer {
	private:
		ILogger& m_logger;
		IFileAccess& m_fileAccess;
		IGraphicsApi& m_api;
		IGraphicsContext& m_context;

		std::unique_ptr<ISwapchain> m_swapchain;
		std::unique_ptr<IGraphicsPipeline> m_pipeline;
		std::unique_ptr<IVertexBuffer> m_vertexBuffer;
		std::unique_ptr<IIndexBuffer> m_indexBuffer;

	public:
		Renderer(
			ILogger& logger,
			IFileAccess& fileAccess,
			IGraphicsApi& api,
			IGraphicsContext& context,
			const RendererInitProps& initProps
		);

		~Renderer();

		bool render(const Scene& scene);
	};
}
