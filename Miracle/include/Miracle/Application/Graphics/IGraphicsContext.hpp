#pragma once

#include <functional>
#include <cstdint>

#include <Miracle/Common/MiracleError.hpp>
#include "IContextTarget.hpp"

namespace Miracle::Application {
	class IGraphicsContext {
	public:
		virtual ~IGraphicsContext() = default;

		virtual void setViewport(
			float x,
			float y,
			float width,
			float height
		) = 0;

		virtual void setScissor(
			int x,
			int y,
			unsigned int width,
			unsigned int height
		) = 0;

		virtual void draw(uint32_t vertexCount) = 0;

		virtual void drawIndexed(uint32_t indexCount) = 0;

		virtual IContextTarget& getTarget() = 0;

		virtual void recordCommands(const std::function<void()>& recording) = 0;

		virtual void submitRecording() = 0;

		virtual void waitForDeviceIdle() = 0;
	};

	namespace GraphicsContextErrors {
		class CreationError : public GraphicsContextError {
		public:
			CreationError() : GraphicsContextError(
				GraphicsContextError::ErrorValue::creationError,
				"Failed to create graphics context"
			) {}
		};

		class DebugToolsUnavailableError : public GraphicsContextError {
		public:
			DebugToolsUnavailableError() : GraphicsContextError(
				GraphicsContextError::ErrorValue::debugToolsUnavailableError,
				"Debug tools for graphics context unavailable"
			) {}
		};

		class FunctionalityNotSupportedError : public GraphicsContextError {
		public:
			FunctionalityNotSupportedError() : GraphicsContextError(
				GraphicsContextError::ErrorValue::functionalityNotSupportedError,
				"Critical graphics related functionality not supported"
			) {}
		};

		class GraphicsDeviceNotFoundError : public GraphicsContextError {
		public:
			GraphicsDeviceNotFoundError() : GraphicsContextError(
				GraphicsContextError::ErrorValue::graphicsDeviceNotFoundError,
				"Graphics card not found"
			) {}
		};

		class NoGraphicsDeviceSupportedError : public GraphicsContextError {
		public:
			NoGraphicsDeviceSupportedError() : GraphicsContextError(
				GraphicsContextError::ErrorValue::noGraphicsDeviceSupportedError,
				"No Graphics card supported"
			) {}
		};
	}
}
