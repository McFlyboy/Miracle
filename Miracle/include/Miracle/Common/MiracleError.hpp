#pragma once

#include <exception>
#include <cstdint>

namespace Miracle {
	using ErrorCode = uint32_t;
	using ErrorCodeSection = uint16_t;

	using ErrorValue = ErrorCodeSection;

	enum class ErrorCategory : ErrorCodeSection {
		none,

		multimediaFramework,
		window
	};

	class MiracleError : public std::exception {
	private:
		ErrorCode m_errorCode;

	public:
		MiracleError(ErrorCategory errorCategory, ErrorValue errorValue) :
			m_errorCode(createErrorCode(static_cast<ErrorCodeSection>(errorCategory), errorValue))
		{}

		MiracleError(ErrorCategory errorCategory, ErrorValue errorValue, const char* message) :
			exception(message),
			m_errorCode(createErrorCode(static_cast<ErrorCodeSection>(errorCategory), errorValue))
		{}

		inline ErrorCode getErrorCode() const { return m_errorCode; }

	private:
		inline ErrorCode createErrorCode(ErrorCodeSection section1, ErrorCodeSection section2) const {
			return (static_cast<ErrorCode>(section1) << (sizeof(ErrorCodeSection) * 8))
				| static_cast<ErrorCode>(section2);
		}
	};

	class UncategorizedError : public MiracleError {
	public:
		enum class ErrorValue : Miracle::ErrorValue {
			success
		};

		UncategorizedError(ErrorValue errorValue) : MiracleError(
			ErrorCategory::none,
			static_cast<Miracle::ErrorValue>(errorValue)
		) {}

		UncategorizedError(ErrorValue errorValue, const char* message) : MiracleError(
			ErrorCategory::none,
			static_cast<Miracle::ErrorValue>(errorValue),
			message
		) {}
	};

	class MultimediaFrameworkError : public MiracleError {
	public:
		enum class ErrorValue : Miracle::ErrorValue {
			initError
		};

		MultimediaFrameworkError(ErrorValue errorValue) : MiracleError(
			ErrorCategory::multimediaFramework,
			static_cast<Miracle::ErrorValue>(errorValue)
		) {}

		MultimediaFrameworkError(ErrorValue errorValue, const char* message) : MiracleError(
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

		WindowError(ErrorValue errorValue) : MiracleError(
			ErrorCategory::window,
			static_cast<Miracle::ErrorValue>(errorValue)
		) {}

		WindowError(ErrorValue errorValue, const char* message) : MiracleError(
			ErrorCategory::window,
			static_cast<Miracle::ErrorValue>(errorValue),
			message
		) {}
	};
}
