#pragma once

#include <optional>
#include <utility>
#include <vector>

#include "Vulkan.hpp"

namespace Miracle::Graphics::Implementations::Vulkan {
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamilyIndex;
		std::optional<uint32_t> presentFamilyIndex;

		QueueFamilyIndices() = default;

		QueueFamilyIndices(QueueFamilyIndices&& right) noexcept :
			graphicsFamilyIndex(std::move(right.graphicsFamilyIndex)),
			presentFamilyIndex(std::move(right.presentFamilyIndex))
		{}

		QueueFamilyIndices& operator=(QueueFamilyIndices&& right) noexcept {
			graphicsFamilyIndex = std::move(right.graphicsFamilyIndex);
			presentFamilyIndex = std::move(right.presentFamilyIndex);
			return *this;
		}

		inline bool hasAllRequired() const {
			return graphicsFamilyIndex.has_value()
				&& presentFamilyIndex.has_value();
		}

		inline bool hasAll() const {
			return hasAllRequired();
		}
	};

	struct ExtensionsSupported {
		bool khrSwapchainExtensionSupported;

		inline bool supportsAllRequired() const {
			return khrSwapchainExtensionSupported;
		}
	};

	struct PresentModesSupported {
		bool immediateModeSupported;
		bool fifoModeSupported;
		bool mailboxModeSupported;

		inline bool supportsAllRequired() const {
			return immediateModeSupported
				&& fifoModeSupported;
		}

		inline bool supportsAll() const {
			return supportsAllRequired()
				&& mailboxModeSupported;
		}
	};

	struct SwapchainSupportDetails {
		vk::SurfaceCapabilitiesKHR capabilities;
		std::vector<vk::SurfaceFormatKHR> formats;
		PresentModesSupported presentModesSupported;

		SwapchainSupportDetails& operator=(SwapchainSupportDetails&& right) noexcept {
			capabilities = right.capabilities;
			formats = std::move(right.formats);
			presentModesSupported = right.presentModesSupported;
			return *this;
		}

		inline bool meetsAllRequirements() const {
			return !formats.empty()
				&& presentModesSupported.supportsAllRequired();
		}
	};

	struct DeviceSupportDetails {
		QueueFamilyIndices queueFamilyIndices;
		ExtensionsSupported extensionsSupported;
		vk::PhysicalDeviceMemoryProperties memoryProperties;
		SwapchainSupportDetails swapchainSupportDetails;

		DeviceSupportDetails& operator=(DeviceSupportDetails&& right) noexcept {
			queueFamilyIndices = std::move(right.queueFamilyIndices);
			extensionsSupported = std::move(right.extensionsSupported);
			memoryProperties = std::move(right.memoryProperties),
			swapchainSupportDetails = std::move(right.swapchainSupportDetails);
			return *this;
		}

		inline bool meetsAllRequirements() const {
			return queueFamilyIndices.hasAllRequired()
				&& extensionsSupported.supportsAllRequired()
				&& swapchainSupportDetails.meetsAllRequirements();
		}
	};
}
