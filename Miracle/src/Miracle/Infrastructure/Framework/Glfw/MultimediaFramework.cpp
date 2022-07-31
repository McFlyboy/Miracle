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

	std::optional<std::u8string_view> MultimediaFramework::getClipboardContent() const {
		auto content = reinterpret_cast<const char8_t*>(glfwGetClipboardString(nullptr));

		return content != nullptr ? std::optional(content) : std::nullopt;
	}

	void MultimediaFramework::setClipboardContent(const std::u8string_view& content) {
		glfwSetClipboardString(nullptr, reinterpret_cast<const char*>(content.data()));
	}

	double MultimediaFramework::getTime() const {
		return glfwGetTime();
	}

	const GLFWvidmode* MultimediaFramework::getGlfwCurrentVideoModeForPrimaryMonitor() const {
		return glfwGetVideoMode(glfwGetPrimaryMonitor());
	}
}
