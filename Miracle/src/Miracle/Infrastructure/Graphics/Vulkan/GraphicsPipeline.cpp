#include "GraphicsPipeline.hpp"

#include <exception>

#include <fmt/format.h>

namespace Miracle::Infrastructure::Graphics::Vulkan {
	GraphicsPipeline::GraphicsPipeline(
		Application::ILogger& logger,
		GraphicsContext& context
	) :
		m_logger(logger),
		m_context(context)
	{
		try {
			m_pipeline = m_context.getDevice().createGraphicsPipeline(
				nullptr,
				vk::GraphicsPipelineCreateInfo{
					.flags               = {},
					.stageCount          = {},
					.pStages             = {},
					.pVertexInputState   = {},
					.pInputAssemblyState = {},
					.pTessellationState  = {},
					.pViewportState      = {},
					.pRasterizationState = {},
					.pMultisampleState   = {},
					.pDepthStencilState  = {},
					.pColorBlendState    = {},
					.pDynamicState       = {},
					.layout              = {},
					.renderPass          = {},
					.subpass             = {},
					.basePipelineHandle  = {},
					.basePipelineIndex   = {}
				}
			);
		}
		catch (const std::exception& e) {
			m_logger.error(fmt::format("Failed to create Vulkan graphics pipeline.\n{}", e.what()));
			throw Application::GraphicsPipelineErrors::CreationError();
		}
	}
}
