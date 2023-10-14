#pragma once

#include <cstdint>

#include <Miracle/Common/MiracleError.hpp>

namespace Miracle::Application {
	class IVertexBuffer {
	public:
		virtual ~IVertexBuffer() = default;

		virtual uint32_t getVertexCount() const = 0;

		virtual void bind() = 0;
	};

	namespace VertexBufferErrors {
		class NoVerticesProvidedError : public VertexBufferError {
		public:
			NoVerticesProvidedError() : VertexBufferError(
				VertexBufferError::ErrorValue::noVerticesProvidedError,
				"No vertices were provided for vertex buffer creation"
			) {}
		};

		class CreationError : public VertexBufferError {
		public:
			CreationError() : VertexBufferError(
				VertexBufferError::ErrorValue::creationError,
				"Failed to create vertex buffer"
			) {}
		};
	}
}
