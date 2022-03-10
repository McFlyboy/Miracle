#pragma once

#include <optional>
#include <utility>

#include "Vulkan.hpp"

namespace Miracle::Graphics::Implementations::Vulkan {
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamilyIndex;
		std::optional<uint32_t> presentFamilyIndex;

		QueueFamilyIndices() = default;

		QueueFamilyIndices(QueueFamilyIndices&& right) :
			graphicsFamilyIndex(std::move(right.graphicsFamilyIndex)),
			presentFamilyIndex(std::move(right.presentFamilyIndex))
		{}

		QueueFamilyIndices& operator=(QueueFamilyIndices&& right) {
			graphicsFamilyIndex = std::move(right.graphicsFamilyIndex);
			presentFamilyIndex = std::move(right.presentFamilyIndex);
			return *this;
		}

		inline bool hasAllRequired() const {
			return graphicsFamilyIndex.has_value() && presentFamilyIndex.has_value();
		}

		inline bool hasAll() const {
			return hasAllRequired();
		}
	};

	struct DeviceSupportDetails {
		QueueFamilyIndices queueFamilyIndices;

		inline bool meetsAllRequirements() const {
			return queueFamilyIndices.hasAllRequired();
		}
	};
}
