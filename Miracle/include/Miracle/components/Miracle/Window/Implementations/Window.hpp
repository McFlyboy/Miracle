#pragma once

#include <Miracle/components/Miracle/Window/IWindow.hpp>

namespace Miracle::Window::Implementations {
	class MIRACLE_API Window : public Miracle::Window::IWindow {
	public:
		Window();

	private:
		bool initializeGlfw() const;
	};
}
