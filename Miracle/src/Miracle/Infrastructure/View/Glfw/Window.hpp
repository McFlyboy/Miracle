#pragma once

#include <string>
#include <utility>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Miracle/Common/Models/WindowSize.hpp>
#include <Miracle/Application/IWindow.hpp>
#include <Miracle/Application/ILogger.hpp>
#include <Miracle/Application/EventDispatcher.hpp>
#include <Miracle/Infrastructure/Framework/Glfw/MultimediaFramework.hpp>
#include <Miracle/Infrastructure/Graphics/Vulkan/IContextTarget.hpp>

namespace Miracle::Infrastructure::View::Glfw {
	class Window :
		public Application::IWindow,
		public Graphics::Vulkan::IContextTarget
	{
	private:
		Application::ILogger& m_logger;
		Application::EventDispatcher& m_eventDispatcher;

		GLFWwindow* m_window = nullptr;
		std::u8string m_title;
		WindowSize m_sizeInPixels = {};
		bool m_sizeChanged = false;
		bool m_iconified = false;

	public:
		Window(
			Application::ILogger& logger,
			Application::EventDispatcher& eventDispatcher,
			Framework::Glfw::MultimediaFramework& multimediaFramework,
			const Application::WindowInitProps& initProps
		);

		~Window();

		GLFWwindow* operator*() const { return m_window; }

		Application::EventDispatcher& getEventDispatcher() const { return m_eventDispatcher; }

		virtual bool isSizeChanged() override { return std::exchange(m_sizeChanged, false); }

		virtual bool isCurrentlyPresentable() const override;

		virtual void show() override;

		virtual bool shouldClose() const override;

		virtual std::u8string_view getTitle() const override;

		virtual void setTitle(const std::u8string_view& title) override;

		virtual bool isResizable() const override;

		virtual void setResizable(bool resizable) override;

		virtual WindowSize getSize() const override;

		virtual void setSize(WindowSize size) override;

		virtual std::span<const char*> getRequiredVulkanExtensionNames() const override;

		virtual vk::raii::SurfaceKHR createVulkanSurface(
			vk::raii::Instance& instance
		) const override;

		virtual vk::Extent2D getCurrentVulkanExtent() const override;
	};
}
