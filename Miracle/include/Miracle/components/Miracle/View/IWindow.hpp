#pragma once

#include <string>

namespace Miracle::View {
	struct MIRACLE_API WindowProps {
		std::u8string title = u8"Miracle Application";
		int width = 640;
		int height = 480;
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

		virtual void update() = 0;
		virtual bool shouldClose() const = 0;
		virtual void close() = 0;
	};
}
