#pragma once

#include <Miracle/Common/MiracleError.hpp>

namespace Miracle::Application {
	class ISwapchain {
	public:
		virtual ~ISwapchain() = default;

		virtual void beginRenderPassCommand(
			float clearColorRed,
			float clearColorGreen,
			float clearColorBlue
		) = 0;

		virtual void endRenderPassCommand() = 0;

		virtual void swap() = 0;

		virtual void recreate() = 0;
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
