#pragma once

#include <memory>
#include <string_view>

#include <Miracle/Common/Math/Color3f.hpp>
#include <Miracle/Common/Models/Mesh.hpp>
#include <Miracle/Application/ILogger.hpp>
#include <Miracle/Application/IFileAccess.hpp>
#include "IGraphicsApi.hpp"
#include "IContextTarget.hpp"
#include "IGraphicsContext.hpp"
#include "ISwapchain.hpp"
#include "IGraphicsPipeline.hpp"
#include "IVertexBuffer.hpp"
#include "IIndexBuffer.hpp"

namespace Miracle::Application {
	struct RendererInitProps{
		SwapchainInitProps swapchainInitProps = {};
		Color3f clearColor = {};
		Mesh mesh = {};
	};

	class Renderer {
	private:
		ILogger& m_logger;
		IFileAccess& m_fileAccess;
		IGraphicsApi& m_api;
		IContextTarget& m_contextTarget;

		std::unique_ptr<IGraphicsContext> m_context;
		std::unique_ptr<ISwapchain> m_swapchain;
		std::unique_ptr<IGraphicsPipeline> m_pipeline;
		std::unique_ptr<IVertexBuffer> m_vertexBuffer;
		std::unique_ptr<IIndexBuffer> m_indexBuffer;
		Color3f m_clearColor;

	public:
		Renderer(
			const std::string_view& appName,
			ILogger& logger,
			IFileAccess& fileAccess,
			IGraphicsApi& api,
			IContextTarget& contextTarget,
			const RendererInitProps& initProps
		);

		~Renderer();

		inline const Color3f& getClearColor() const { return m_clearColor; }

		void setClearColor(const Color3f& clearColor);

		bool render();
	};
}
