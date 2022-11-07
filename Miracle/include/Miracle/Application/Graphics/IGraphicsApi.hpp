#pragma once

#include <memory>
#include <string_view>
#include <vector>
#include <cstdint>

#include <Miracle/Common/Models/Vertex.hpp>
#include <Miracle/Common/Models/Face.hpp>
#include <Miracle/Application/IFileAccess.hpp>
#include "IContextTarget.hpp"
#include "IGraphicsContext.hpp"
#include "ISwapchain.hpp"
#include "IGraphicsPipeline.hpp"
#include "IVertexBuffer.hpp"
#include "IIndexBuffer.hpp"

namespace Miracle::Application {
	class IGraphicsApi {
	public:
		virtual ~IGraphicsApi() = default;

		virtual std::unique_ptr<IGraphicsContext> createGraphicsContext(
			const std::string_view& appName,
			IContextTarget& target
		) const = 0;

		virtual std::unique_ptr<ISwapchain> createSwapchain(
			IGraphicsContext& context,
			const SwapchainInitProps& initProps
		) const = 0;

		virtual std::unique_ptr<IGraphicsPipeline> createGraphicsPipeline(
			IFileAccess& fileAccess,
			IGraphicsContext& context,
			ISwapchain& swapchain
		) const = 0;

		virtual std::unique_ptr<IVertexBuffer> createVertexBuffer(
			IGraphicsContext& context,
			const std::vector<Vertex>& vertices
		) const = 0;

		virtual std::unique_ptr<IIndexBuffer> createIndexBuffer(
			IGraphicsContext& context,
			const std::vector<Face>& faces
		) const = 0;
	};
}
