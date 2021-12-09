#pragma once

namespace Miracle::View {
	struct MIRACLE_API WindowProps {
		const int width = 640;
		const int height = 480;
		const char* title = "Miracle Application";
	};

	class MIRACLE_API IWindow {
	public:
		IWindow() = default;
		IWindow(const IWindow&) = delete;

		virtual void update() const = 0;
		virtual bool shouldClose() const = 0;
	};
}
