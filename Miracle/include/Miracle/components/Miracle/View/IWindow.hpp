#pragma once

#include <string>

namespace Miracle::View {
	struct MIRACLE_API WindowProps {
		std::u8string title = u8"Miracle Application";
		int width = 640;
		int height = 480;
		bool resizable = false;
		bool fullscreen = false;
	};

	enum class MIRACLE_API WindowError {
		initializationError,
		windowCreationError 
	};

	class MIRACLE_API IWindow {
	public:
		IWindow() = default;
		IWindow(const IWindow&) = delete;

		virtual void update() const = 0;
		virtual bool shouldClose() const = 0;
	};
}
