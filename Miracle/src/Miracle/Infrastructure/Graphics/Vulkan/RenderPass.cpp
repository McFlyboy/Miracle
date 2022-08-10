#include "RenderPass.hpp"

#include <exception>
#include <array>

#include <fmt/format.h>

namespace Miracle::Infrastructure::Graphics::Vulkan {
	RenderPass::RenderPass(
		Application::ILogger& logger,
		GraphicsContext& context,
		Swapchain& swapchain
	) :
		m_logger(logger),
		m_context(context),
		m_swapchain(swapchain)
	{
		auto attachments = std::array{
			vk::AttachmentDescription{
				.flags          = {},
				.format         = m_swapchain.getImageFormat(),
				.samples        = vk::SampleCountFlagBits::e1,
				.loadOp         = vk::AttachmentLoadOp::eClear,
				.storeOp        = vk::AttachmentStoreOp::eStore,
				.stencilLoadOp  = vk::AttachmentLoadOp::eDontCare,
				.stencilStoreOp = vk::AttachmentStoreOp::eDontCare,
				.initialLayout  = vk::ImageLayout::eUndefined,
				.finalLayout    = vk::ImageLayout::ePresentSrcKHR
			}
		};

		auto attachmentReference = vk::AttachmentReference{
			.attachment = 0,
			.layout     = vk::ImageLayout::eColorAttachmentOptimal
		};

		auto subpasses = std::array{
			vk::SubpassDescription{
				.flags                   = {},
				.pipelineBindPoint       = vk::PipelineBindPoint::eGraphics,
				.inputAttachmentCount    = 0,
				.pInputAttachments       = nullptr,
				.colorAttachmentCount    = 1,
				.pColorAttachments       = &attachmentReference,
				.pResolveAttachments     = 0,
				.pDepthStencilAttachment = nullptr,
				.preserveAttachmentCount = 0,
				.pPreserveAttachments    = nullptr
			}
		};

		try {
			m_renderPass = m_context.getDevice().createRenderPass(
				vk::RenderPassCreateInfo{
					.flags           = {},
					.attachmentCount = static_cast<uint32_t>(attachments.size()),
					.pAttachments    = attachments.data(),
					.subpassCount    = static_cast<uint32_t>(subpasses.size()),
					.pSubpasses      = subpasses.data(),
					.dependencyCount = 0,
					.pDependencies   = nullptr
				}
			);
		}
		catch (const std::exception e) {
			m_logger.error(fmt::format("Failed to create Vulkan render pass.\n{}", e.what()));
			throw Application::RenderPassErrors::CreationError();
		}

		m_logger.info("Vulkan render pass created");
	}

	RenderPass::~RenderPass() {
		m_logger.info("Destroying Vulkan render pass...");
	}
}
