#pragma once

#include <string>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

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

	public:
		Window(
			Application::ILogger& logger,
			Application::EventDispatcher& eventDispatcher,
			Framework::Glfw::MultimediaFramework& multimediaFramework,
			const Application::WindowInitProps& initProps
		);

		~Window();

		inline GLFWwindow* operator*() const { return m_window; }

		inline Application::EventDispatcher& getEventDispatcher() const { return m_eventDispatcher; }

		virtual void show() override;

		virtual bool shouldClose() const override;

		virtual std::u8string_view getTitle() const override;

		virtual void setTitle(const std::u8string_view& title) override;

		virtual std::span<const char*> getRequiredVulkanExtensionNames() const override;

		virtual vk::raii::SurfaceKHR createVulkanSurface(
			vk::raii::Instance& instance
		) const override;
	};
}
