#pragma once

#include <GLFW/glfw3.h>

#include <Miracle/components/Miracle/Window/IWindow.hpp>

namespace Miracle::Window::Implementations {
	class Window : public Miracle::Window::IWindow {
	private:
		GLFWwindow* m_window;

	public:
		Window();
		~Window();

		virtual void update() const override;
		virtual bool shouldClose() const override;

	private:
		bool initializeGlfw() const;
	};
}
