#include "Window.hpp"

#include <fmt/format.h>

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
				Logger::error(description);
			}
		);

		auto monitorVideoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, false);
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

			throw MiracleError::WindowCreationError;
		}

		Logger::info("Application window created");

		glfwSetWindowPos(
			m_window,
			(monitorVideoMode->width - props.width) / 2,
			(monitorVideoMode->height - props.height) / 2
		);
	}

	Window::~Window() {
		glfwDestroyWindow(m_window);
		glfwTerminate();

		Logger::info("Terminated GLFW");
	}

	void Window::show() {
		glfwShowWindow(m_window);

		Logger::info("Showing application window");
	}

	void Window::update() {
		glfwPollEvents();
	}

	bool Window::shouldClose() const {
		return glfwWindowShouldClose(m_window);
	}

	void Window::close() {
		glfwSetWindowShouldClose(m_window, true);
	}

	std::optional<MiracleError> Window::initializeGlfw() const {
		Logger::info(fmt::format("Initializing GLFW version: {}", glfwGetVersionString()));

		bool initialized = glfwInit();

		if (!initialized) {
			Logger::error("Failed to initialize GLFW!");

			return MiracleError::WindowInitializationError;
		}

		Logger::info("GLFW initialized");

		return std::nullopt;
	}
}
