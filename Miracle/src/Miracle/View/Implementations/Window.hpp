#pragma once

#include <optional>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Miracle/components/Miracle/View/IWindow.hpp>
#include <Miracle/MiracleError.hpp>
#include <Miracle/Graphics/Implementations/Vulkan/ISurfaceTarget.hpp>

namespace Miracle::View::Implementations {
	class Window : public IWindow, public Graphics::Implementations::Vulkan::ISurfaceTarget {
	private:
		GLFWwindow* m_window;

	public:
		Window(const WindowProps& props);
		~Window() override;

		virtual void show() override;

		virtual void update() override;

		virtual bool shouldClose() const override;

		virtual void close() override;

		virtual bool isResizable() const override;

		virtual void setResizable(bool resizable) override;

		inline GLFWwindow* getGlfwWindow() const { return m_window; }

		virtual std::vector<const char*> getRequiredInstanceExtensions() const override;

		virtual vk::Extent2D getCurrentExtent() const override;

		virtual std::variant<MiracleError, vk::raii::SurfaceKHR> createSurface(
			const vk::raii::Instance& instance
		) const override;

	private:
		std::optional<MiracleError> initializeGlfw() const;
	};
}
