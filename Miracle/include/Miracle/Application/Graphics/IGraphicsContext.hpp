#pragma once

#include <Miracle/Common/MiracleError.hpp>

namespace Miracle::Application {
	class IGraphicsContext {
	public:
		virtual ~IGraphicsContext() = default;
	};

	namespace GraphicsContextErrors {
		class GraphicsContextCreationError : public GraphicsContextError {
		public:
			GraphicsContextCreationError() : GraphicsContextError(
				GraphicsContextError::ErrorValue::creationError,
				"Failed to create graphics context"
			) {}
		};
	}
}
