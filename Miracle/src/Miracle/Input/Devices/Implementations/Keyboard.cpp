#include "Keyboard.hpp"

#include <string>

#include <fmt/format.h>
#include <GLFW/glfw3.h>

#include <Miracle/components/Miracle/Diagnostics/Logger.hpp>

namespace Miracle::Input::Devices::Implementations {
	Keyboard::Keyboard(const Window& window) {
		glfwSetKeyCallback(
			window.getGlfwWindow(),
			[](GLFWwindow* window, int key, int scancode, int action, int mods) {
				if (action == GLFW_PRESS) {
					Diagnostics::Logger::info(fmt::format("Key {} pressed", key));
				}
				else if (action == GLFW_REPEAT) {
					Diagnostics::Logger::info(fmt::format("Key {} repeated", key));
				}
				else {
					Diagnostics::Logger::info(fmt::format("Key {} released", key));
				}
			}
		);
	}
}
