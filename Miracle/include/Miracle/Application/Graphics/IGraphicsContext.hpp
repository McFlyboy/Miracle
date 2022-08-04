#pragma once

#include <Miracle/Common/MiracleError.hpp>

namespace Miracle::Application {
	class IGraphicsContext {
	public:
		virtual ~IGraphicsContext() = default;
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
	}
}
