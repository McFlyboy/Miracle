#pragma once

#include <Miracle/Common/MiracleError.hpp>
#include "DeviceSynchronizer.hpp"

namespace Miracle::Application {
	class ISwapchain {
	public:
		virtual ~ISwapchain() = default;

		inline virtual DeviceSynchronizer getNextImageReadySynchronizer() const = 0;

		virtual void beginRenderPassCommand(
			float clearColorRed,
			float clearColorGreen,
			float clearColorBlue
		) = 0;

		virtual void endRenderPassCommand() = 0;

		virtual void swap() = 0;
	};

	struct SwapchainInitProps {
		bool useSrgb;
		bool useVsync;
	};

	namespace SwapchainErrors {
		class CreationError : public SwapchainError {
		public:
			CreationError() : SwapchainError(
				SwapchainError::ErrorValue::creationError,
				"Failed to create swapchain"
			) {}
		};
	}
}
