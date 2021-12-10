#pragma once

#include <string>

namespace Miracle::View {
	struct MIRACLE_API WindowProps {
		std::string title = "Miracle Application";
		int width = 640;
		int height = 480;
	};

	class MIRACLE_API IWindow {
	public:
		IWindow() = default;
		IWindow(const IWindow&) = delete;

		virtual void update() const = 0;
		virtual bool shouldClose() const = 0;
	};
}
