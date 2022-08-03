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
	}
}
