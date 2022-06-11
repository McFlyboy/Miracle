#pragma once

#include <string>

namespace Miracle::View {
	struct WindowProps {
		std::u8string title = u8"Miracle Application";
		int width = 640;
		int height = 480;
		bool resizable = false;
	};

	class IWindow {
	protected:
		IWindow() = default;

	public:
		IWindow(const IWindow&) = delete;
		virtual ~IWindow() = default;

		virtual void show() = 0;

		virtual void update() = 0;

		virtual bool shouldClose() const = 0;

		virtual void close() = 0;

		virtual bool isResizable() const = 0;

		virtual void setResizable(bool resizable) = 0;
	};
}
