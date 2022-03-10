#pragma once

#include "Vulkan.hpp"
#include "Instance.hpp"
#include "Surface.hpp"
#include "DeviceSupportDetails.hpp"

namespace Miracle::Graphics::Implementations::Vulkan {
	class Device {
	private:
		DeviceSupportDetails m_supportDetails;
		vk::raii::Device m_device = nullptr;
		vk::raii::Queue m_graphicsQueue = nullptr;
		vk::raii::Queue m_presentQueue = nullptr;

	public:
		Device(
			const Instance& instance,
			const Surface& surface
		);
	};
}
