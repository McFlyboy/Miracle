#pragma once

#include <string>
#include <optional>

#include "Vulkan.hpp"

namespace Miracle::Infrastructure::Graphics::Vulkan {
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamilyIndex = std::nullopt;

		inline bool hasAllIndicesSet() const {
			return graphicsFamilyIndex.has_value();
		}
	};

	struct DeviceInfo {
		std::string name;
		vk::PhysicalDeviceType type;
		vk::DeviceSize deviceLocalMemorySize;
		QueueFamilyIndices queueFamilyIndices;
	};
}
