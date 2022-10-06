#pragma once

#include <cstdint>

#include <Miracle/Common/MiracleError.hpp>

namespace Miracle::Application {
	class IVertexBuffer {
	public:
		virtual ~IVertexBuffer() = default;

		inline virtual uint32_t getVertexCount() const = 0;

		virtual void bind() = 0;
	};

	namespace VertexBufferErrors {
		class CreationError : public VertexBufferError {
		public:
			CreationError() : VertexBufferError(
				VertexBufferError::ErrorValue::creationError,
				"Failed to create vertex buffer"
			) {}
		};

		class AllocationError : public VertexBufferError {
		public:
			AllocationError() : VertexBufferError(
				VertexBufferError::ErrorValue::allocationError,
				"Failed to allocate memory for vertex buffer"
			) {}
		};

		class BindError : public VertexBufferError {
		public:
			BindError() : VertexBufferError(
				VertexBufferError::ErrorValue::bindError,
				"Failed to bind allocated memory to vertex buffer"
			) {}
		};

		class MapError : public VertexBufferError {
		public:
			MapError() : VertexBufferError(
				VertexBufferError::ErrorValue::mapError,
				"Failed to map allocated device memory to CPU accessible memory"
			) {}
		};
	}
}
