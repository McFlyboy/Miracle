#include "RenderPass.hpp"

namespace Miracle::Graphics::Implementations::Vulkan {
	RenderPass::RenderPass(
		const Device& device,
		vk::Format attachmentFormat
	) {
		auto colorAttachment = vk::AttachmentDescription{
			.flags          = {},
			.format         = attachmentFormat,
			.samples        = vk::SampleCountFlagBits::e1,
			.loadOp         = vk::AttachmentLoadOp::eClear,
			.storeOp        = vk::AttachmentStoreOp::eStore,
			.stencilLoadOp  = vk::AttachmentLoadOp::eDontCare,
			.stencilStoreOp = vk::AttachmentStoreOp::eDontCare,
			.initialLayout  = vk::ImageLayout::eUndefined,
			.finalLayout    = vk::ImageLayout::ePresentSrcKHR
		};

		auto colorAttachmentRef = vk::AttachmentReference{
			.attachment = 0,
			.layout     = vk::ImageLayout::eColorAttachmentOptimal
		};

		auto subpass = vk::SubpassDescription{
			.flags                   = {},
			.pipelineBindPoint       = vk::PipelineBindPoint::eGraphics,
			.inputAttachmentCount    = 0,
			.pInputAttachments       = nullptr,
			.colorAttachmentCount    = 1,
			.pColorAttachments       = &colorAttachmentRef,
			.pResolveAttachments     = nullptr,
			.pDepthStencilAttachment = nullptr,
			.preserveAttachmentCount = 0,
			.pPreserveAttachments    = nullptr
		};

		auto result = device.createRenderPass({
			.flags           = {},
			.attachmentCount = 1,
			.pAttachments    = &colorAttachment,
			.subpassCount    = 1,
			.pSubpasses      = &subpass,
			.dependencyCount = 0,
			.pDependencies   = nullptr
		});

		if (result.index() == 0) {
			throw std::get<MiracleError>(result);
		}

		m_renderPass = std::move(std::get<vk::raii::RenderPass>(result));
	}
}
