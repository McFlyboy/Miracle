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
		InitializationError,
		WindowCreationError 
	};

	class MIRACLE_API IWindow {
	protected:
		IWindow() = default;

	public:
		IWindow(const IWindow&) = delete;
		virtual ~IWindow() = default;

		virtual void update() const = 0;
		virtual bool shouldClose() const = 0;
	};
}
