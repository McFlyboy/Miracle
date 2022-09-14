#pragma once

#include <memory>
#include <string_view>

#include <Miracle/Application/ILogger.hpp>
#include "IGraphicsApi.hpp"
#include "IContextTarget.hpp"
#include "IGraphicsContext.hpp"
#include "ISwapchain.hpp"

namespace Miracle::Application {
	struct RendererInitProps{
		SwapchainInitProps swapchainInitProps = {};
	};

	class Renderer {
	private:
		ILogger& m_logger;
		IGraphicsApi& m_api;
		IContextTarget& m_contextTarget;

		std::unique_ptr<IGraphicsContext> m_context;
		std::unique_ptr<ISwapchain> m_swapchain;

	public:
		Renderer(
			const std::string_view& appName,
			ILogger& logger,
			IGraphicsApi& api,
			IContextTarget& contextTarget,
			const RendererInitProps& initProps
		);

		~Renderer();

		void render();
	};
}
