#pragma once

#include <string_view>
#include <optional>
#include <chrono>

#include <Miracle/Common/MiracleError.hpp>

namespace Miracle::Application {
	class IMultimediaFramework {
	public:
		virtual ~IMultimediaFramework() = default;

		virtual void processEvents() = 0;

		virtual std::optional<std::u8string_view> getClipboardContent() const = 0;

		virtual void setClipboardContent(const std::u8string_view& content) = 0;

		virtual std::chrono::duration<double> getDurationSinceInitialization() const = 0;
	};

	namespace MultimediaFrameworkErrors {
		class MultimediaFrameworkInitError : public MultimediaFrameworkError {
		public:
			MultimediaFrameworkInitError() : MultimediaFrameworkError(
				MultimediaFrameworkError::ErrorValue::initError,
				"Failed to initialize underlying multimedia framework"
			) {}
		};
	}
}
