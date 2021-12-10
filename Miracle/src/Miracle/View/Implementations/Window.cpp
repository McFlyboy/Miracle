#include "Window.hpp"

#include <string>

#include <Miracle/components/Miracle/Diagnostics/Logger.hpp>

using namespace Miracle::Diagnostics;

namespace Miracle::View::Implementations {
	Window::Window(const WindowProps& props) {
		bool initialized = initializeGlfw();

		if (initialized == false) {
			throw -1;
		}

		m_window = glfwCreateWindow(props.width, props.height, props.title.c_str(), nullptr, nullptr);

		if (m_window == nullptr) {
			Logger::error("Failed to create application window!");

			throw -1;
		}

		Logger::info("Application window created");
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

	bool Window::initializeGlfw() const {
		Logger::info(std::string("Initializing GLFW version: ") + glfwGetVersionString());

		int initResult = glfwInit();

		if (initResult != GLFW_TRUE) {
			Logger::error("Failed to initialize GLFW!");

			return false;
		}

		Logger::info("GLFW initialized");

		return true;
	}
}
