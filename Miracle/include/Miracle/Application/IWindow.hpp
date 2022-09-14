#pragma once

#include <string_view>

#include <Miracle/Common/MiracleError.hpp>

namespace Miracle::Application {
	class IWindow {
	public:
		virtual ~IWindow() = default;

		virtual void show() = 0;

		virtual bool shouldClose() const = 0;

		virtual std::u8string_view getTitle() const = 0;

		virtual void setTitle(const std::u8string_view& title) = 0;

		virtual bool isResizable() const = 0;

		virtual void setResizable(bool resizable) = 0;
	};

	struct WindowInitProps {
		std::u8string_view title;
		int width;
		int height;
		bool resizable;
	};

	namespace WindowErrors {
		class CreationError : public WindowError {
		public:
			CreationError() : WindowError(
				WindowError::ErrorValue::creationError,
				"Failed to create window"
			) {}
		};
	}
}
