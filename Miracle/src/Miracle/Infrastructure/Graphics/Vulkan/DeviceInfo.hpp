#pragma once

#include <string>
#include <optional>
#include <set>
#include <vector>

#include "Vulkan.hpp"

namespace Miracle::Infrastructure::Graphics::Vulkan {
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamilyIndex = {};
		std::optional<uint32_t> presentFamilyIndex = {};

		bool hasAllIndicesSet() const {
			return graphicsFamilyIndex.has_value()
				&& presentFamilyIndex.has_value();
		}

		std::set<uint32_t> createSetOfAllUniqueIndices() const {
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

	struct SwapchainSupport {
		uint32_t minImageCount = {};
		std::optional<uint32_t> maxImageCount = {};
		std::vector<vk::SurfaceFormatKHR> surfaceFormats = {};
		bool hasImmediateModePresentationSupport = {};
		bool hasMailboxModePresentationSupport = {};
	};

	struct DeviceExtensionSupport {
		std::optional<SwapchainSupport> swapchainSupport = {};
	};

	struct DeviceInfo {
		std::string name = {};
		vk::PhysicalDeviceType type = {};
		vk::DeviceSize deviceLocalMemorySize = {};
		QueueFamilyIndices queueFamilyIndices = {};
		DeviceExtensionSupport extensionSupport = {};
	};
}
