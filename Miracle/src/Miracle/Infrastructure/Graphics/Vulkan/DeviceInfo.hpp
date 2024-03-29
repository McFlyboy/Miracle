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
		std::optional<uint32_t> transferFamilyIndex = {};

		bool hasAllIndicesSet() const {
			return graphicsFamilyIndex.has_value()
				&& presentFamilyIndex.has_value()
				&& transferFamilyIndex.has_value();
		}

		std::set<uint32_t> createSetOfAllUniqueIndices() const {
			auto uniqueIndices = std::set<uint32_t>();

			if (graphicsFamilyIndex.has_value()) {
				uniqueIndices.insert(graphicsFamilyIndex.value());
			}

			if (presentFamilyIndex.has_value()) {
				uniqueIndices.insert(presentFamilyIndex.value());
			}

			if (transferFamilyIndex.has_value()) {
				uniqueIndices.insert(transferFamilyIndex.value());
			}

			return uniqueIndices;
		}
	};

	struct SwapchainSupport {
		bool hasDoubleBufferingSupport = {};
		bool hasTripleBufferingSupport = {};
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
