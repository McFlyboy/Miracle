#include "Window.hpp"

#include <string>

#include <Miracle/components/Miracle/Diagnostics/Logger.hpp>

using namespace Miracle::Diagnostics;

namespace Miracle::View::Implementations {
	Window::Window(const WindowProps& props) {
		auto error = initializeGlfw();

		if (error.has_value()) {
			throw error.value();
		}

		glfwSetErrorCallback(
			[](int errorCode, const char* description) {
				Logger::error(std::string(description));
			}
		);

		glfwWindowHint(GLFW_RESIZABLE, props.resizable);
		glfwWindowHint(GLFW_VISIBLE, false);

		m_window = glfwCreateWindow(
			props.width,
			props.height,
			reinterpret_cast<const char*>(props.title.c_str()),
			nullptr,
			nullptr
		);

		if (m_window == nullptr) {
			Logger::error("Failed to create application window!");

			throw WindowError::windowCreationError;
		}

		Logger::info("Application window created");

		glfwSetWindowRefreshCallback(
			m_window,
			[](GLFWwindow* window) {
				glfwSwapBuffers(window);
			}
		);

		glfwShowWindow(m_window);

		Logger::info("Showing application window");
	}

	Window::~Window() {
		glfwDestroyWindow(m_window);
		glfwTerminate();

		Logger::info("Terminated GLFW");
	}

	void Window::update() const {
		glfwPollEvents();
		glfwSwapBuffers(m_window);
	}

	bool Window::shouldClose() const {
		return glfwWindowShouldClose(m_window) == GLFW_TRUE;
	}

	std::optional<WindowError> Window::initializeGlfw() const {
		Logger::info(std::string("Initializing GLFW version: ") + glfwGetVersionString());

		bool initialized = glfwInit();

		if (!initialized) {
			Logger::error("Failed to initialize GLFW!");

			return WindowError::initializationError;
		}

		Logger::info("GLFW initialized");

		return std::nullopt;
	}
}
