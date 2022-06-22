#pragma once

#include <utility>

#include "Vulkan.hpp"
#include "Device.hpp"

namespace Miracle::Graphics::Implementations::Vulkan {
	class RenderPass {
	private:
		vk::raii::RenderPass m_renderPass = nullptr;

	public:
		RenderPass() = default;

		RenderPass(
			const Device& device,
			vk::Format attachmentFormat
		);

		RenderPass& operator=(RenderPass&& right) noexcept {
			m_renderPass = std::move(right.m_renderPass);
			return *this;
		}

		inline const vk::raii::RenderPass& getRawRenderPass() const { return m_renderPass; }
	};
}
