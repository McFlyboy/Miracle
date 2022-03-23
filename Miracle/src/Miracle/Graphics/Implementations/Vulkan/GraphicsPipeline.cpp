#include "GraphicsPipeline.hpp"

#include <array>

namespace Miracle::Graphics::Implementations::Vulkan {
	GraphicsPipeline::GraphicsPipeline(
		const Device& device,
		const Swapchain& swapchain,
		const Io::ResourceLoader& resourceLoader
	) :
		m_device(device)
	{
		auto vertexShaderByteCodeLoadResult = resourceLoader.loadFileBinary("shaders/default.vert.spv");
		auto fragmentShaderByteCodeLoadResult = resourceLoader.loadFileBinary("shaders/default.frag.spv");

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

		auto extent = swapchain.getImageExtent();

		auto viewport = vk::Viewport{
			.x        = 0.0f,
			.y        = 0.0f,
			.width    = static_cast<float>(extent.width),
			.height   = static_cast<float>(extent.height),
			.minDepth = 0.0f,
			.maxDepth = 1.0f,
		};
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
}
