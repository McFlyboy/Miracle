#pragma once

#include <string_view>

#include <Miracle/Common/MiracleError.hpp>

namespace Miracle::Application {
	struct WindowInitProps {
		std::u8string_view title;
		int width;
		int height;
	};

	class IWindow {
	public:
		virtual ~IWindow() = default;

		virtual void show() = 0;

		virtual bool shouldClose() const = 0;

		virtual std::u8string_view getTitle() const = 0;

		virtual void setTitle(const std::u8string_view& title) = 0;
	};

	namespace WindowErrors {
		class WindowCreationError : public WindowError {
		public:
			WindowCreationError() : WindowError(
				WindowError::ErrorValue::creationError,
				"Failed to create window"
			) {}
		};
	}
}
