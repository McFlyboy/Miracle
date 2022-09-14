#include "Window.hpp"

#include <fmt/format.h>

#include <Miracle/Application/Graphics/IGraphicsContext.hpp>

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
		glfwWindowHint(GLFW_RESIZABLE, initProps.resizable);
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

			throw Application::WindowErrors::CreationError();
		}

		glfwSetWindowUserPointer(m_window, this);

		glfwSetWindowSizeCallback(
			m_window,
			[](GLFWwindow* window, int width, int height) {
				reinterpret_cast<Window*>(glfwGetWindowUserPointer(window))
					->m_sizeChanged = true;
			}
		);

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

	bool Window::isResizable() const {
		return glfwGetWindowAttrib(m_window, GLFW_RESIZABLE);
	}

	void Window::setResizable(bool resizable) {
		glfwSetWindowAttrib(m_window, GLFW_RESIZABLE, resizable);
	}

	std::span<const char*> Window::getRequiredVulkanExtensionNames() const {
		uint32_t extensionCount = 0;
		auto extensionNames = glfwGetRequiredInstanceExtensions(&extensionCount);

		return std::span(extensionNames, extensionCount);
	}

	vk::raii::SurfaceKHR Window::createVulkanSurface(
		vk::raii::Instance& instance
	) const {
		VkSurfaceKHR surface = nullptr;
		auto result = vk::Result(glfwCreateWindowSurface(*instance, m_window, nullptr, &surface));

		if (result != vk::Result::eSuccess) {
			m_logger.error(
				fmt::format(
					"Failed to create Vulkan surface for graphics context target.\nResult: {}",
					vk::to_string(result)
				)
			);

			throw Application::GraphicsContextErrors::CreationError();
		}

		return vk::raii::SurfaceKHR(instance, surface);
	}

	vk::Extent2D Window::getCurrentVulkanExtent() const {
		int width = 0;
		int height = 0;

		glfwGetFramebufferSize(m_window, &width, &height);

		return vk::Extent2D{
			.width = static_cast<uint32_t>(width),
			.height = static_cast<uint32_t>(height)
		};
	}
}
