#include "GraphicsPipeline.hpp"

#include <array>

namespace Miracle::Graphics::Implementations::Vulkan {
	GraphicsPipeline::GraphicsPipeline(
		const Device& device,
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
