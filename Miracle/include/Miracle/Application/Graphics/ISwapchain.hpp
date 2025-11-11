#pragma once

#include <Miracle/Common/MiracleError.hpp>
#include <Miracle/Common/Math/ColorRgb.hpp>

namespace Miracle::Application {
	struct SwapchainImageSize {
		unsigned int width = 0;
		unsigned int height = 0;
	};

	class ISwapchain {
	public:
		virtual ~ISwapchain() = default;

		virtual SwapchainImageSize getImageSize() const = 0;

		// Graphics command
		virtual void beginRenderPass(ColorRgb clearColor) = 0;

		// Graphics command
		virtual void endRenderPass() = 0;

		virtual void prepareNextImage() = 0;

		virtual void swap() = 0;

		virtual void recreate() = 0;

		virtual bool isUsingVsync() const = 0;

		virtual void setVsync(bool useVsync) = 0;

		virtual bool isUsingTripleBuffering() const = 0;

		virtual void setTripleBuffering(bool useTripleBuffering) = 0;
	};

	struct SwapchainInitProps {
		bool useVsync;
		bool useTripleBuffering;
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
