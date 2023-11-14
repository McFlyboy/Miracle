#include "MultimediaFramework.hpp"

#include <format>

namespace Miracle::Infrastructure::Framework::Glfw {
	MultimediaFramework::MultimediaFramework(Application::ILogger& logger) :
		m_logger(logger)
	{
		m_logger.info(std::format("Initializing GLFW version: {}", glfwGetVersionString()));

		bool initialized = glfwInit();

		if (!initialized) {
			const char* glfwErrorDescription = nullptr;
			int glfwErrorCode = glfwGetError(&glfwErrorDescription);

			m_logger.error(
				std::format(
					"Failed to initialize GLFW\nGLFW error code {0}: {1}",
					glfwErrorCode,
					glfwErrorDescription
				)
			);

			throw Application::MultimediaFrameworkErrors::InitError();
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

	std::chrono::duration<double> MultimediaFramework::getDurationSinceInitialization() const {
		return std::chrono::duration<double>(glfwGetTime());
	}

	const GLFWvidmode* MultimediaFramework::getGlfwCurrentVideoModeForPrimaryMonitor() const {
		return glfwGetVideoMode(glfwGetPrimaryMonitor());
	}
}
