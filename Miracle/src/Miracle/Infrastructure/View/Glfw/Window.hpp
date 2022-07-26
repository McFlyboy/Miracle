#pragma once

#include <string_view>

#include <GLFW/glfw3.h>

#include <Miracle/Application/IWindow.hpp>
#include <Miracle/Application/ILogger.hpp>
#include <Miracle/Application/EventDispatcher.hpp>
#include <Miracle/Infrastructure/Framework/Glfw/MultimediaFramework.hpp>

namespace Miracle::Infrastructure::View::Glfw {
	class Window : public Application::IWindow {
	private:
		Application::ILogger& m_logger;
		Application::EventDispatcher& m_eventDispatcher;

		GLFWwindow* m_window = nullptr;

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

		virtual void setTitle(const std::string_view& title) override;
	};
}
