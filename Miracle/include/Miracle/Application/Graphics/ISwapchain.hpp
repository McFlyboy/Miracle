#pragma once

#include <Miracle/Common/MiracleError.hpp>

namespace Miracle::Application {
	class ISwapchain {
	public:
		virtual ~ISwapchain() = default;
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
