#include <Miracle/Window/Implementations/Window.hpp>

#include <string>

#include <GLFW/glfw3.h>

#include <Miracle/Diagnostics/Logger.hpp>

using namespace Miracle::Diagnostics;

namespace Miracle::Window::Implementations {
	Window::Window() {
		bool initialized = initializeGlfw();

		if (initialized == false) {
			Logger::error("Failed to initialize GLFW!");

			return;
		}
	}

	bool Window::initializeGlfw() const {
		Logger::info(
			std::string("Initializing GLFW version: ") + glfwGetVersionString()
		);

		int initResult = glfwInit();

		if (initResult != GLFW_TRUE) {
			Logger::error("Failed to initialize GLFW!");

			return false;
		}

		Logger::info("GLFW initialized");

		return true;
	}
}
