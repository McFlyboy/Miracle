#pragma once

#include <Miracle/Common/MiracleError.hpp>
#include <Miracle/Common/Math/Color3f.hpp>

namespace Miracle::Application {
	struct SwapchainImageSize {
		unsigned int width = 0;
		unsigned int height = 0;
	};

	class ISwapchain {
	public:
		virtual ~ISwapchain() = default;

		virtual SwapchainImageSize getImageSize() const = 0;

		virtual void beginRenderPass(Color3f clearColor) = 0;

		virtual void endRenderPass() = 0;

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
