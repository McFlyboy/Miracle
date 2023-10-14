#pragma once

#include <cstdint>

#include <Miracle/Common/MiracleError.hpp>

namespace Miracle::Application {
	class IIndexBuffer {
	public:
		virtual ~IIndexBuffer() = default;

		virtual uint32_t getIndexCount() const = 0;

		virtual void bind() = 0;
	};

	namespace IndexBufferErrors {
		class NoIndicesProvidedError : public IndexBufferError {
		public:
			NoIndicesProvidedError() : IndexBufferError(
				IndexBufferError::ErrorValue::noIndicesProvidedError,
				"No indices were provided for index buffer creation"
			) {}
		};

		class CreationError : public IndexBufferError {
		public:
			CreationError() : IndexBufferError(
				IndexBufferError::ErrorValue::creationError,
				"Failed to create index buffer"
			) {}
		};
	}
}
