#include <Miracle/EngineDependencies.hpp>

#include <Miracle/Common/Mappings.hpp>
#include "Infrastructure/Framework/Glfw/MultimediaFramework.hpp"
#include "Infrastructure/View/Glfw/Window.hpp"
#include "Infrastructure/Input/Glfw/Keyboard.hpp"

namespace Miracle {
	using GlfwMultimediaFramework = Infrastructure::Framework::Glfw::MultimediaFramework;
	using GlfwWindow = Infrastructure::View::Glfw::Window;
	using GlfwKeyboard = Infrastructure::Input::Glfw::Keyboard;

	EngineDependencies::EngineDependencies(
		const std::string_view& appName,
		const WindowConfig& windowConfig,
		Application::ILogger& logger,
		Application::EventDispatcher& eventDispatcher
	) :
		m_multimediaFramework(
			std::make_unique<GlfwMultimediaFramework>(logger)
		),
		m_window(
			std::make_unique<GlfwWindow>(
				logger,
				eventDispatcher,
				*reinterpret_cast<GlfwMultimediaFramework*>(m_multimediaFramework.get()),
				Mappings::toWindowInitProps(windowConfig, Mappings::toU8StringView(appName))
			)
		),
		m_keyboard(
			std::make_unique<GlfwKeyboard>(
				eventDispatcher,
				*reinterpret_cast<GlfwWindow*>(m_window.get())
			)
		)
	{}
}
