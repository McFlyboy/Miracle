#include <Miracle/MiracleApp.hpp>

#include <iostream>

#include <GLFW/glfw3.h>

namespace Miracle {
	int MiracleApp::run() {
		std::cout << glfwGetVersionString() << std::endl;

		return 0;
	}
}
