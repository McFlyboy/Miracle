#include "GraphicsPipeline.hpp"

#include <exception>
#include <array>

#include <fmt/format.h>

#include <Miracle/Common/Math/Vector2f.hpp>

namespace Miracle::Infrastructure::Graphics::Vulkan {
	GraphicsPipeline::GraphicsPipeline(
		Application::ILogger& logger,
		Application::IFileAccess& fileAccess,
		GraphicsContext& context,
		Swapchain& swapchain
	) :
		m_logger(logger),
		m_fileAccess(fileAccess),
		m_context(context),
		m_swapchain(swapchain)
	{
		auto vertexShaderBytecode = m_fileAccess.readFileAsBinary("Assets/Shaders/Default.vert.spv");
		m_logger.info("Vulkan vertex shader loaded successfully");

		auto fragmentShaderBytecode = m_fileAccess.readFileAsBinary("Assets/Shaders/Default.frag.spv");
		m_logger.info("Vulkan fragment shader loaded successfully");

		auto vertexShaderModule = createShaderModule(vertexShaderBytecode);
		auto fragmentShaderModule = createShaderModule(fragmentShaderBytecode);

		auto shaderStages = std::array{
			vk::PipelineShaderStageCreateInfo{
				.flags               = {},
				.stage               = vk::ShaderStageFlagBits::eVertex,
				.module              = *vertexShaderModule,
				.pName               = "main",
				.pSpecializationInfo = {}
			},
			vk::PipelineShaderStageCreateInfo{
				.flags               = {},
				.stage               = vk::ShaderStageFlagBits::eFragment,
				.module              = *fragmentShaderModule,
				.pName               = "main",
				.pSpecializationInfo = {}
			}
		};

		auto vertexInputBindingDescription = vk::VertexInputBindingDescription{
			.binding   = 0,
			.stride    = sizeof(Vector2f),
			.inputRate = vk::VertexInputRate::eVertex
		};

		auto vertexInputAttributeDescription = vk::VertexInputAttributeDescription{
			.location = 0,
			.binding  = 0,
			.format   = vk::Format::eR32G32Sfloat,
			.offset   = 0
		};

		auto vertexInputStateCreateInfo = vk::PipelineVertexInputStateCreateInfo{
			.flags                           = {},
			.vertexBindingDescriptionCount   = 1,
			.pVertexBindingDescriptions      = &vertexInputBindingDescription,
			.vertexAttributeDescriptionCount = 1,
			.pVertexAttributeDescriptions    = &vertexInputAttributeDescription
		};

		auto inputAssemblyStateCreateInfo = vk::PipelineInputAssemblyStateCreateInfo{
			.flags                  = {},
			.topology               = vk::PrimitiveTopology::eTriangleList,
			.primitiveRestartEnable = false
		};

		auto viewportStateCreateInfo = vk::PipelineViewportStateCreateInfo{
			.flags         = {},
			.viewportCount = 1,
			.pViewports    = nullptr,
			.scissorCount  = 1,
			.pScissors     = nullptr
		};

		auto rasterizationStateCreateInfo = vk::PipelineRasterizationStateCreateInfo{
			.flags                   = {},
			.depthClampEnable        = false,
			.rasterizerDiscardEnable = false,
			.polygonMode             = vk::PolygonMode::eFill,
			.cullMode                = vk::CullModeFlagBits::eNone,
			.frontFace               = {},
			.depthBiasEnable         = false,
			.depthBiasConstantFactor = {},
			.depthBiasClamp          = {},
			.depthBiasSlopeFactor    = {},
			.lineWidth               = 1.0f
		};

		auto multisampleStateCreateInfo = vk::PipelineMultisampleStateCreateInfo{
			.flags                 = {},
			.rasterizationSamples  = vk::SampleCountFlagBits::e1,
			.sampleShadingEnable   = false,
			.minSampleShading      = {},
			.pSampleMask           = {},
			.alphaToCoverageEnable = {},
			.alphaToOneEnable      = {}
		};

		auto colorBlendAttachmentState = vk::PipelineColorBlendAttachmentState{
			.blendEnable         = false,
			.srcColorBlendFactor = {},
			.dstColorBlendFactor = {},
			.colorBlendOp        = {},
			.srcAlphaBlendFactor = {},
			.dstAlphaBlendFactor = {},
			.alphaBlendOp        = {},
			.colorWriteMask      = vk::ColorComponentFlagBits::eR
				| vk::ColorComponentFlagBits::eG
				| vk::ColorComponentFlagBits::eB
				| vk::ColorComponentFlagBits::eA
		};

		auto colorBlendStateCreateInfo = vk::PipelineColorBlendStateCreateInfo{
			.flags           = {},
			.logicOpEnable   = false,
			.logicOp         = {},
			.attachmentCount = 1,
			.pAttachments    = &colorBlendAttachmentState,
			.blendConstants  = {}
		};

		auto dynamicStates = std::array{
			vk::DynamicState::eViewport,
			vk::DynamicState::eScissor
		};

		auto dynamicStateCreateInfo = vk::PipelineDynamicStateCreateInfo{
			.flags             = {},
			.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size()),
			.pDynamicStates    = dynamicStates.data()
		};

		try {
			m_layout = m_context.getDevice().createPipelineLayout(
				vk::PipelineLayoutCreateInfo{
					.flags                  = {},
					.setLayoutCount         = 0,
					.pSetLayouts            = nullptr,
					.pushConstantRangeCount = 0,
					.pPushConstantRanges    = nullptr
				}
			);
		}
		catch (const std::exception& e) {
			m_logger.error(fmt::format("Failed to create Vulkan pipeline layout.\n{}", e.what()));
			throw Application::GraphicsPipelineErrors::CreationError();
		}

		try {
			m_pipeline = m_context.getDevice().createGraphicsPipeline(
				nullptr,
				vk::GraphicsPipelineCreateInfo{
					.flags               = {},
					.stageCount          = static_cast<uint32_t>(shaderStages.size()),
					.pStages             = shaderStages.data(),
					.pVertexInputState   = &vertexInputStateCreateInfo,
					.pInputAssemblyState = &inputAssemblyStateCreateInfo,
					.pTessellationState  = nullptr,
					.pViewportState      = &viewportStateCreateInfo,
					.pRasterizationState = &rasterizationStateCreateInfo,
					.pMultisampleState   = &multisampleStateCreateInfo,
					.pDepthStencilState  = nullptr,
					.pColorBlendState    = &colorBlendStateCreateInfo,
					.pDynamicState       = &dynamicStateCreateInfo,
					.layout              = *m_layout,
					.renderPass          = *m_swapchain.getRenderPass(),
					.subpass             = 0,
					.basePipelineHandle  = nullptr,
					.basePipelineIndex   = {}
				}
			);
		}
		catch (const std::exception& e) {
			m_logger.error(fmt::format("Failed to create Vulkan graphics pipeline.\n{}", e.what()));
			throw Application::GraphicsPipelineErrors::CreationError();
		}

		m_logger.info("Vulkan graphics pipeline created");
	}

	GraphicsPipeline::~GraphicsPipeline() {
		m_logger.info("Destroying Vulkan graphics pipeline...");
	}

	void GraphicsPipeline::bind() {
		m_context.getCommandBuffer().bindPipeline(vk::PipelineBindPoint::eGraphics, *m_pipeline);
	}

	vk::raii::ShaderModule GraphicsPipeline::createShaderModule(const std::vector<char>& bytecode) const {
		try {
			return m_context.getDevice().createShaderModule(
				vk::ShaderModuleCreateInfo{
					.flags    = {},
					.codeSize = bytecode.size(),
					.pCode    = reinterpret_cast<const uint32_t*>(bytecode.data())
				}
			);
		}
		catch (const std::exception& e) {
			m_logger.error(fmt::format("Failed to create Vulkan shader module for pipeline.\n{}", e.what()));
			throw Application::GraphicsPipelineErrors::CreationError();
		}
	}
}
