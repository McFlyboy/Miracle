#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>

namespace Miracle {
	using ErrorCode = uint32_t;
	using ErrorCodeSection = uint16_t;

	using ErrorValue = ErrorCodeSection;

	enum class ErrorCategory : ErrorCodeSection {
		none,

		fileAccess,
		multimediaFramework,
		window,
		graphicsContext,
		swapchain,
		renderPass,
		graphicsPipeline,
		vertexBuffer,
		indexBuffer
	};

	class MiracleError : public std::runtime_error {
	private:
		ErrorCode m_errorCode;

	public:
		MiracleError(ErrorCategory errorCategory, ErrorValue errorValue, const std::string& message) :
			runtime_error(message),
			m_errorCode(createErrorCode(errorCategory, errorValue))
		{}

		constexpr ErrorCode getErrorCode() const { return m_errorCode; }

	private:
		constexpr ErrorCode createErrorCode(ErrorCategory category, ErrorValue value) const {
			return (static_cast<ErrorCode>(category) << (sizeof(category) * 8))
				| static_cast<ErrorCode>(value);
		}
	};

	class UncategorizedError : public MiracleError {
	public:
		enum class ErrorValue : Miracle::ErrorValue {
			success,

			noAppRunningError
		};

		UncategorizedError(ErrorValue errorValue, const std::string& message) : MiracleError(
			ErrorCategory::none,
			static_cast<Miracle::ErrorValue>(errorValue),
			message
		) {}
	};

	class FileAccessError : public MiracleError {
	public:
		enum class ErrorValue : Miracle::ErrorValue {
			fileDoesNotExistError,
			unableToOpenFileError
		};

		FileAccessError(ErrorValue errorValue, const std::string& message) : MiracleError(
			ErrorCategory::fileAccess,
			static_cast<Miracle::ErrorValue>(errorValue),
			message
		) {}
	};

	class MultimediaFrameworkError : public MiracleError {
	public:
		enum class ErrorValue : Miracle::ErrorValue {
			initError
		};

		MultimediaFrameworkError(ErrorValue errorValue, const std::string& message) : MiracleError(
			ErrorCategory::multimediaFramework,
			static_cast<Miracle::ErrorValue>(errorValue),
			message
		) {}
	};

	class WindowError : public MiracleError {
	public:
		enum class ErrorValue : Miracle::ErrorValue {
			creationError
		};

		WindowError(ErrorValue errorValue, const std::string& message) : MiracleError(
			ErrorCategory::window,
			static_cast<Miracle::ErrorValue>(errorValue),
			message
		) {}
	};

	class GraphicsContextError : public MiracleError {
	public:
		enum class ErrorValue : Miracle::ErrorValue {
			creationError,
			debugToolsUnavailableError,
			functionalityNotSupportedError,
			graphicsDeviceNotFoundError,
			noGraphicsDeviceSupportedError
		};

		GraphicsContextError(ErrorValue errorValue, const std::string& message) : MiracleError(
			ErrorCategory::graphicsContext,
			static_cast<Miracle::ErrorValue>(errorValue),
			message
		) {}
	};

	class SwapchainError : public MiracleError {
	public:
		enum class ErrorValue : Miracle::ErrorValue {
			creationError
		};

		SwapchainError(ErrorValue errorValue, const std::string& message) : MiracleError(
			ErrorCategory::swapchain,
			static_cast<Miracle::ErrorValue>(errorValue),
			message
		) {}
	};

	class RenderPassError : public MiracleError {
	public:
		enum class ErrorValue : Miracle::ErrorValue {
			creationError
		};

		RenderPassError(ErrorValue errorValue, const std::string& message) : MiracleError(
			ErrorCategory::renderPass,
			static_cast<Miracle::ErrorValue>(errorValue),
			message
		) {}
	};

	class GraphicsPipelineError : public MiracleError {
	public:
		enum class ErrorValue : Miracle::ErrorValue {
			creationError
		};

		GraphicsPipelineError(ErrorValue errorValue, const std::string& message) : MiracleError(
			ErrorCategory::graphicsPipeline,
			static_cast<Miracle::ErrorValue>(errorValue),
			message
		) {}
	};

	class VertexBufferError : public MiracleError {
	public:
		enum class ErrorValue : Miracle::ErrorValue {
			noVerticesProvidedError,
			creationError
		};

		VertexBufferError(ErrorValue errorValue, const std::string& message) : MiracleError(
			ErrorCategory::vertexBuffer,
			static_cast<Miracle::ErrorValue>(errorValue),
			message
		) {}
	};

	class IndexBufferError : public MiracleError {
	public:
		enum class ErrorValue : Miracle::ErrorValue {
			noIndicesProvidedError,
			creationError
		};

		IndexBufferError(ErrorValue errorValue, const std::string& message) : MiracleError(
			ErrorCategory::indexBuffer,
			static_cast<Miracle::ErrorValue>(errorValue),
			message
		) {}
	};
}
