#include "Window.hpp"

namespace Miracle::Infrastructure::View::Glfw {
	Window::Window(
		Application::ILogger& logger,
		Application::EventDispatcher& eventDispatcher,
		Framework::Glfw::MultimediaFramework& multimediaFramework,
		const Application::WindowInitProps& initProps
	) :
		m_logger(logger),
		m_eventDispatcher(eventDispatcher),
		m_title(initProps.title)
	{
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, false);
		glfwWindowHint(GLFW_VISIBLE, false);

		m_window = glfwCreateWindow(
			initProps.width,
			initProps.height,
			reinterpret_cast<const char*>(m_title.c_str()),
			nullptr,
			nullptr
		);

		if (m_window == nullptr) {
			m_logger.error("Failed to create window");

			throw Application::WindowErrors::WindowCreationError();
		}

		glfwSetWindowUserPointer(m_window, this);

		auto monitorVideoMode = multimediaFramework.getGlfwCurrentVideoModeForPrimaryMonitor();

		glfwSetWindowPos(
			m_window,
			(monitorVideoMode->width - initProps.width) / 2,
			(monitorVideoMode->height - initProps.height) / 2
		);

		m_logger.info("Application window created");
	}

	Window::~Window() {
		glfwDestroyWindow(m_window);

		m_logger.info("Application window destroyed");
	}

	void Window::show() {
		m_logger.info("Showing application window");

		glfwShowWindow(m_window);
	}

	bool Window::shouldClose() const {
		return glfwWindowShouldClose(m_window);
	}

	std::u8string_view Window::getTitle() const {
		return m_title;
	}

	void Window::setTitle(const std::u8string_view& title) {
		m_title = title;
		glfwSetWindowTitle(m_window, reinterpret_cast<const char*>(m_title.c_str()));
	}
}
