#pragma once

#include <string>
#include <optional>
#include <set>

#include "Vulkan.hpp"

namespace Miracle::Infrastructure::Graphics::Vulkan {
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamilyIndex = std::nullopt;
		std::optional<uint32_t> presentFamilyIndex = std::nullopt;

		inline bool hasAllIndicesSet() const {
			return graphicsFamilyIndex.has_value()
				&& presentFamilyIndex.has_value();
		}

		inline std::set<uint32_t> createSetOfAllUniqueIndices() const {
			auto uniqueIndices = std::set<uint32_t>();

			if (graphicsFamilyIndex.has_value()) {
				uniqueIndices.insert(graphicsFamilyIndex.value());
			}

			if (presentFamilyIndex.has_value()) {
				uniqueIndices.insert(presentFamilyIndex.value());
			}

			return uniqueIndices;
		}
	};

	struct DeviceInfo {
		std::string name = {};
		vk::PhysicalDeviceType type = {};
		vk::DeviceSize deviceLocalMemorySize = {};
		QueueFamilyIndices queueFamilyIndices = {};
	};
}
