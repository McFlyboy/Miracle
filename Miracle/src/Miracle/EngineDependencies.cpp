#include <Miracle/EngineDependencies.hpp>

#include <Miracle/Common/Mappings.hpp>
#include <Miracle/Common/UnicodeConverter.hpp>
#include "Infrastructure/Framework/Glfw/MultimediaFramework.hpp"
#include "Infrastructure/View/Glfw/Window.hpp"
#include "Infrastructure/Input/Glfw/Keyboard.hpp"
#include "Infrastructure/Graphics/Vulkan/GraphicsApi.hpp"

namespace Miracle {
	using GlfwMultimediaFramework = Infrastructure::Framework::Glfw::MultimediaFramework;
	using GlfwWindow = Infrastructure::View::Glfw::Window;
	using GlfwKeyboard = Infrastructure::Input::Glfw::Keyboard;
	using VulkanGraphicsApi = Infrastructure::Graphics::Vulkan::GraphicsApi;

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
				Mappings::toWindowInitProps(windowConfig, UnicodeConverter::toUtf8(appName))
				)
		),
		m_keyboard(
			std::make_unique<GlfwKeyboard>(
				eventDispatcher,
				*m_multimediaFramework.get(),
				*reinterpret_cast<GlfwWindow*>(m_window.get())
				)
		),
		m_graphicsApi(
			std::make_unique<VulkanGraphicsApi>()
		),
		m_renderer(
			appName,
			logger,
			*m_graphicsApi.get(),
			*reinterpret_cast<GlfwWindow*>(m_window.get()),
			Application::RendererInitProps{
				.swapchainInitProps = Application::SwapchainInitProps{
					.useSrgb  = true,
					.useVsync = true
				}
			}
		),
		m_textInputService(eventDispatcher),
		m_deltaTimeService(*m_multimediaFramework.get()),
		m_performanceCountingService(*m_multimediaFramework.get())
	{}
}
