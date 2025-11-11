#pragma once

#include <memory>
#include <vector>

#include <Miracle/Common/Math/ColorRgb.hpp>
#include <Miracle/Common/Models/Mesh.hpp>
#include <Miracle/Application/Models/Scene.hpp>
#include <Miracle/Application/ILogger.hpp>
#include <Miracle/Application/IFileAccess.hpp>
#include "IGraphicsApi.hpp"
#include "IGraphicsContext.hpp"
#include "ISwapchain.hpp"
#include "IGraphicsPipeline.hpp"
#include "MeshBuffers.hpp"
#include "PushConstants.hpp"

namespace Miracle::Application {
	struct RendererInitProps{
		SwapchainInitProps swapchainInitProps = {};
		bool useDepthTesting;
		const std::vector<Mesh>& meshes = {};
	};

	class Renderer {
	private:
		ILogger& m_logger;
		IFileAccess& m_fileAccess;
		IGraphicsApi& m_api;
		IGraphicsContext& m_context;

		std::unique_ptr<ISwapchain> m_swapchain;
		std::unique_ptr<IGraphicsPipeline> m_pipeline;
		std::vector<MeshBuffers> m_meshBuffersList;
		bool m_useDepthTesting;

	public:
		Renderer(
			ILogger& logger,
			IFileAccess& fileAccess,
			IGraphicsApi& api,
			IGraphicsContext& context,
			const RendererInitProps& initProps
		);

		~Renderer();

		bool isUsingVsync() const { return m_swapchain->isUsingVsync(); }

		void setVsync(bool useVsync) {
			m_swapchain->setVsync(useVsync);
			m_context.waitForDeviceIdle();
			m_swapchain->recreate();
		}

		bool isUsingTripleBuffering() const { return m_swapchain->isUsingTripleBuffering(); }

		void setTripleBuffering(bool useTripleBuffering) {
			m_swapchain->setTripleBuffering(useTripleBuffering);
			m_context.waitForDeviceIdle();
			m_swapchain->recreate();
		}

		void setVsyncAndTripleBuffering(bool useVsync, bool useTripleBuffering) {
			m_swapchain->setVsync(useVsync);
			m_swapchain->setTripleBuffering(useTripleBuffering);
			m_context.waitForDeviceIdle();
			m_swapchain->recreate();
		}

		bool isUsingDepthTesting() const { return m_useDepthTesting; }

		void setDepthTesting(bool useDepthTesting) {
			m_useDepthTesting = useDepthTesting;
		}

		bool render(const Scene& scene);

	private:
		std::vector<MeshBuffers> createMeshBuffersList(const std::vector<Mesh>& meshes) const;
	};
}
