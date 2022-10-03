#pragma once

#include <memory>
#include <string_view>

#include <Miracle/Application/ILogger.hpp>
#include <Miracle/Application/IFileAccess.hpp>
#include "IContextTarget.hpp"
#include "IGraphicsContext.hpp"
#include "ISwapchain.hpp"
#include "IGraphicsPipeline.hpp"

namespace Miracle::Application {
	class IGraphicsApi {
	public:
		virtual ~IGraphicsApi() = default;

		virtual std::unique_ptr<IGraphicsContext> createGraphicsContext(
			const std::string_view& appName,
			ILogger& logger,
			IContextTarget& target
		) const = 0;

		virtual std::unique_ptr<ISwapchain> createSwapchain(
			ILogger& logger,
			IGraphicsContext& context,
			const SwapchainInitProps& initProps
		) const = 0;

		virtual std::unique_ptr<IGraphicsPipeline> createGraphicsPipeline(
			ILogger& logger,
			IFileAccess& fileAccess,
			IGraphicsContext& context,
			ISwapchain& swapchain
		) const = 0;
	};
}
