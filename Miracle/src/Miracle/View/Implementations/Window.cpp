#include "Window.hpp"

#include <utility>

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
		glfwWindowHint(GLFW_RESIZABLE, props.resizable);
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

		glfwSetWindowUserPointer(m_window, this);

		glfwSetFramebufferSizeCallback(
			m_window,
			[](GLFWwindow* window, int width, int height) {
				auto thisWindow = (Window*)glfwGetWindowUserPointer(window);
				thisWindow->markExtentChanged();
			}
		);

		glfwSetWindowPos(
			m_window,
			(monitorVideoMode->width - props.width) / 2,
			(monitorVideoMode->height - props.height) / 2
		);

		Logger::info("Application window created");
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

	bool Window::isResizable() const {
		return glfwGetWindowAttrib(m_window, GLFW_RESIZABLE);
	}

	void Window::setResizable(bool resizable) {
		glfwSetWindowAttrib(m_window, GLFW_RESIZABLE, resizable);
	}

	std::vector<const char*> Window::getRequiredInstanceExtensions() const {
		uint32_t extensionCount;
		auto extensionNames = glfwGetRequiredInstanceExtensions(&extensionCount);

		auto extensions = std::vector<const char*>();
		extensions.reserve(extensionCount);

		for (uint32_t i = 0; i < extensionCount; i++) {
			extensions.push_back(extensionNames[i]);
		}

		return std::move(extensions);
	}

	vk::Extent2D Window::getCurrentExtent() const {
		int width;
		int height;

		glfwGetFramebufferSize(m_window, &width, &height);

		return vk::Extent2D{
			.width  = static_cast<uint32_t>(width),
			.height = static_cast<uint32_t>(height)
		};
	}

	std::variant<MiracleError, vk::raii::SurfaceKHR> Window::createSurface(
		const vk::raii::Instance& instance
	) const {
		VkSurfaceKHR surface;
		auto result = vk::Result(glfwCreateWindowSurface(*instance, m_window, nullptr, &surface));

		if (result != vk::Result::eSuccess) {
			Logger::error("Failed to create Vulkan Surface from GLFW window!");
			Logger::error(vk::to_string(result));
			return MiracleError::VulkanGraphicsEngineSurfaceCreationError;
		}

		return vk::raii::SurfaceKHR(instance, surface);
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
