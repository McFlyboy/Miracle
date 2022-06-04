#pragma once

#include <string>

#include <Miracle/components/Definitions.hpp>

namespace Miracle::View {
	struct MIRACLE_API WindowProps {
		std::u8string title = u8"Miracle Application";
		int width = 640;
		int height = 480;
		bool resizable = false;
	};

	class MIRACLE_API IWindow {
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
