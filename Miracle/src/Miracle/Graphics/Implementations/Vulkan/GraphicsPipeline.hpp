#pragma once

#include <variant>
#include <vector>

#include <Miracle/MiracleError.hpp>
#include <Miracle/Io/ResourceLoader.hpp>
#include "Vulkan.hpp"
#include "Device.hpp"

namespace Miracle::Graphics::Implementations::Vulkan {
	class GraphicsPipeline {
	private:
		const Device& m_device;

	public:
		GraphicsPipeline(
			const Device& device,
			const Io::ResourceLoader& resourceLoader
		);

	private:
		std::variant<MiracleError, vk::raii::ShaderModule> createShaderModule(
			const std::vector<char>& shaderByteCode
		) const;
	};
}
