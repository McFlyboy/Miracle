#pragma once

#include <optional>

#include <GLFW/glfw3.h>

#include <Miracle/components/Miracle/View/IWindow.hpp>

namespace Miracle::View::Implementations {
	class Window : public IWindow {
	private:
		GLFWwindow* m_window;

	public:
		Window(const WindowProps& props);
		~Window();

		virtual void update() const override;
		virtual bool shouldClose() const override;

	private:
		std::optional<WindowError> initializeGlfw() const;
	};
}
