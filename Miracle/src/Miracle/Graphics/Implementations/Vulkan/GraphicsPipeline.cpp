#include "GraphicsPipeline.hpp"

#include <array>
#include <utility>

namespace Miracle::Graphics::Implementations::Vulkan {
	GraphicsPipeline::GraphicsPipeline(
		const Device& device,
		const Swapchain& swapchain,
		const Io::ResourceLoader& resourceLoader
	) :
		m_device(device),
		m_swapchain(swapchain),
		m_resourceLoader(resourceLoader)
	{
		auto pipelineLayoutCreateResult = createPipelineLayout();

		if (pipelineLayoutCreateResult.index() == 0) {
			throw std::get<MiracleError>(pipelineLayoutCreateResult);
		}

		m_layout = std::move(std::get<vk::raii::PipelineLayout>(pipelineLayoutCreateResult));

		auto pipelineCreateResult = createPipeline();

		if (pipelineCreateResult.index() == 0) {
			throw std::get<MiracleError>(pipelineCreateResult);
		}

		m_pipeline = std::move(std::get<vk::raii::Pipeline>(pipelineCreateResult));
	}

	void GraphicsPipeline::bind(const vk::raii::CommandBuffer& commandBuffer) const {
		commandBuffer.bindPipeline(
			vk::PipelineBindPoint::eGraphics,
			*m_pipeline
		);
	}

	std::optional<MiracleError> GraphicsPipeline::recreate() {
		m_pipeline.~Pipeline();
		m_layout.~PipelineLayout();

		auto pipelineLayoutCreateResult = createPipelineLayout();

		if (pipelineLayoutCreateResult.index() == 0) {
			return std::get<MiracleError>(pipelineLayoutCreateResult);
		}

		m_layout = std::move(std::get<vk::raii::PipelineLayout>(pipelineLayoutCreateResult));

		auto pipelineCreateResult = createPipeline();

		if (pipelineCreateResult.index() == 0) {
			return std::get<MiracleError>(pipelineCreateResult);
		}

		m_pipeline = std::move(std::get<vk::raii::Pipeline>(pipelineCreateResult));

		return std::nullopt;
	}

	std::variant<MiracleError, vk::raii::ShaderModule> GraphicsPipeline::createShaderModule(
		const std::vector<char>& shaderByteCode
	) const {
		return m_device.createShaderModule({
			.flags    = {},
			.codeSize = shaderByteCode.size(),
			.pCode    = reinterpret_cast<const uint32_t*>(shaderByteCode.data())
		});
	}

	std::variant<MiracleError, vk::raii::PipelineLayout> GraphicsPipeline::createPipelineLayout() const {
		return m_device.createPipelineLayout({
			.flags                  = {},
			.setLayoutCount         = 0,
			.pSetLayouts            = nullptr,
			.pushConstantRangeCount = 0,
			.pPushConstantRanges    = nullptr
		});
	}

	std::variant<MiracleError, vk::raii::Pipeline> GraphicsPipeline::createPipeline() const {
		auto vertexShaderByteCodeLoadResult = m_resourceLoader.loadFileBinary("shaders/default.vert.spv");
		auto fragmentShaderByteCodeLoadResult = m_resourceLoader.loadFileBinary("shaders/default.frag.spv");

		if (vertexShaderByteCodeLoadResult.index() == 0) {
			throw std::get<MiracleError>(vertexShaderByteCodeLoadResult);
		}

		if (fragmentShaderByteCodeLoadResult.index() == 0) {
			throw std::get<MiracleError>(fragmentShaderByteCodeLoadResult);
		}

		auto& vertexShaderByteCode = std::get<std::vector<char>>(vertexShaderByteCodeLoadResult);
		auto& fragmentShaderByteCode = std::get<std::vector<char>>(fragmentShaderByteCodeLoadResult);

		auto vertexShaderModuleCreateResult = createShaderModule(vertexShaderByteCode);

		if (vertexShaderModuleCreateResult.index() == 0) {
			throw std::get<MiracleError>(vertexShaderModuleCreateResult);
		}

		auto fragmentShaderModuleCreateResult = createShaderModule(fragmentShaderByteCode);

		if (fragmentShaderModuleCreateResult.index() == 0) {
			throw std::get<MiracleError>(fragmentShaderModuleCreateResult);
		}

		auto& vertexShaderModule = std::get<vk::raii::ShaderModule>(vertexShaderModuleCreateResult);
		auto& fragmentShaderModule = std::get<vk::raii::ShaderModule>(fragmentShaderModuleCreateResult);

		auto shaderStageCreateInfos = std::array{
			vk::PipelineShaderStageCreateInfo{
				.flags               = {},
				.stage               = vk::ShaderStageFlagBits::eVertex,
				.module              = *vertexShaderModule,
				.pName               = "main",
				.pSpecializationInfo = nullptr
			},
			vk::PipelineShaderStageCreateInfo{
				.flags               = {},
				.stage               = vk::ShaderStageFlagBits::eFragment,
				.module              = *fragmentShaderModule,
				.pName               = "main",
				.pSpecializationInfo = nullptr
			}
		};

		auto vertexInputStateCreateInfo = vk::PipelineVertexInputStateCreateInfo{
			.flags                           = {},
			.vertexBindingDescriptionCount   = 0,
			.pVertexBindingDescriptions      = nullptr,
			.vertexAttributeDescriptionCount = 0,
			.pVertexAttributeDescriptions    = nullptr
		};

		auto inputAssemblyStateCreateInfo = vk::PipelineInputAssemblyStateCreateInfo{
			.flags                  = {},
			.topology               = vk::PrimitiveTopology::eTriangleList,
			.primitiveRestartEnable = false
		};

		auto& imageExtent = m_swapchain.getImageExtent();

		auto viewport = vk::Viewport{
			.x        = 0.0f,
			.y        = 0.0f,
			.width    = static_cast<float>(imageExtent.width),
			.height   = static_cast<float>(imageExtent.height),
			.minDepth = 0.0f,
			.maxDepth = 1.0f
		};

		auto scissor = vk::Rect2D{
			.offset = vk::Offset2D{
				.x = 0,
				.y = 0
			},
			.extent = imageExtent
		};

		auto viewportStateCreateInfo = vk::PipelineViewportStateCreateInfo{
			.flags         = {},
			.viewportCount = 1,
			.pViewports    = &viewport,
			.scissorCount  = 1,
			.pScissors     = &scissor
		};

		auto rasterizationStateCreateInfo = vk::PipelineRasterizationStateCreateInfo{
			.flags                   = {},
			.depthClampEnable        = false,
			.rasterizerDiscardEnable = false,
			.polygonMode             = vk::PolygonMode::eFill,
			.cullMode                = vk::CullModeFlagBits::eBack,
			.frontFace               = vk::FrontFace::eClockwise,
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

		return m_device.createGraphicsPipeline({
			.flags               = {},
			.stageCount          = static_cast<uint32_t>(shaderStageCreateInfos.size()),
			.pStages             = shaderStageCreateInfos.data(),
			.pVertexInputState   = &vertexInputStateCreateInfo,
			.pInputAssemblyState = &inputAssemblyStateCreateInfo,
			.pTessellationState  = nullptr,
			.pViewportState      = &viewportStateCreateInfo,
			.pRasterizationState = &rasterizationStateCreateInfo,
			.pMultisampleState   = &multisampleStateCreateInfo,
			.pDepthStencilState  = nullptr,
			.pColorBlendState    = &colorBlendStateCreateInfo,
			.pDynamicState       = nullptr,
			.layout              = *m_layout,
			.renderPass          = *m_swapchain.getRenderPass().getRawRenderPass(),
			.subpass             = 0,
			.basePipelineHandle  = {},
			.basePipelineIndex   = {}
		});
	}
}
