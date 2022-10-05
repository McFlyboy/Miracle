#pragma once

#include <Miracle/Common/MiracleError.hpp>

namespace Miracle::Application {
	class IVertexBuffer {
	public:
		virtual ~IVertexBuffer() = default;
	};

	namespace VertexBufferErrors {
		class CreationError : public VertexBufferError {
		public:
			CreationError() : VertexBufferError(
				VertexBufferError::ErrorValue::creationError,
				"Failed to create vertex buffer"
			) {}
		};
	}
}
