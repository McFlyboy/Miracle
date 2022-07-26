#include "MultimediaFramework.hpp"

#include <fmt/format.h>

namespace Miracle::Infrastructure::Framework::Glfw {
	MultimediaFramework::MultimediaFramework(Application::ILogger& logger) :
		m_logger(logger)
	{
		m_logger.info(fmt::format("Initializing GLFW version: {}", glfwGetVersionString()));

		bool initialized = glfwInit();

		if (!initialized) {
			const char* glfwErrorDescription = nullptr;
			int glfwErrorCode = glfwGetError(&glfwErrorDescription);

			m_logger.error(
				fmt::format(
					"Failed to initialize GLFW\nGLFW error code {0}: {1}",
					glfwErrorCode,
					glfwErrorDescription
				)
			);

			throw Application::MultimediaFrameworkErrors::MultimediaFrameworkInitError();
		}

		m_logger.info("GLFW initialized");
	}

	MultimediaFramework::~MultimediaFramework() {
		glfwTerminate();

		m_logger.info("Terminated GLFW");
	}

	void MultimediaFramework::processEvents() {
		glfwPollEvents();
	}

	const GLFWvidmode* MultimediaFramework::getGlfwCurrentVideoModeForPrimaryMonitor() const {
		return glfwGetVideoMode(glfwGetPrimaryMonitor());
	}
}
