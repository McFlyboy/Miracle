#pragma once

#include <optional>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Miracle/components/Miracle/View/IWindow.hpp>
#include <Miracle/MiracleError.hpp>

namespace Miracle::View::Implementations {
	class Window : public IWindow {
	private:
		GLFWwindow* m_window;

	public:
		Window(const WindowProps& props);
		~Window() override;

		virtual void update() override;
		virtual bool shouldClose() const override;
		virtual void close() override;

		inline GLFWwindow* getGlfwWindow() const { return m_window; }

	private:
		std::optional<MiracleError> initializeGlfw() const;
	};
}
