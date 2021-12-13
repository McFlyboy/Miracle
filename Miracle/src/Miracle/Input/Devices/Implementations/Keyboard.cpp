#include "Keyboard.hpp"

#include <GLFW/glfw3.h>

#include <Miracle/components/Miracle/Diagnostics/Logger.hpp>

namespace Miracle::Input::Devices::Implementations {
	Keyboard::Keyboard(const Window& window) {
		glfwSetKeyCallback(
			window.getGlfwWindow(),
			[](GLFWwindow* window, int key, int scancode, int action, int mods) {
				if (action == GLFW_PRESS) {
					Diagnostics::Logger::info("Key pressed");
				}
				else if (action == GLFW_REPEAT) {
					Diagnostics::Logger::info("Key repeated");
				}
				else {
					Diagnostics::Logger::info("Key released");
				}
			}
		);
	}
}
