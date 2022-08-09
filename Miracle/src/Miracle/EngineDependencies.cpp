#include <Miracle/EngineDependencies.hpp>

#include <Miracle/Common/Mappings.hpp>
#include <Miracle/Common/UnicodeConverter.hpp>
#include "Infrastructure/Framework/Glfw/MultimediaFramework.hpp"
#include "Infrastructure/View/Glfw/Window.hpp"
#include "Infrastructure/Input/Glfw/Keyboard.hpp"
#include "Infrastructure/Graphics/Vulkan/GraphicsContext.hpp"
#include "Infrastructure/Graphics/Vulkan/Swapchain.hpp"

namespace Miracle {
	using GlfwMultimediaFramework = Infrastructure::Framework::Glfw::MultimediaFramework;
	using GlfwWindow = Infrastructure::View::Glfw::Window;
	using GlfwKeyboard = Infrastructure::Input::Glfw::Keyboard;
	using VulkanGraphicsContext = Infrastructure::Graphics::Vulkan::GraphicsContext;
	using VulkanSwapchain = Infrastructure::Graphics::Vulkan::Swapchain;

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
		m_graphicsContext(
			std::make_unique<VulkanGraphicsContext>(
				appName,
				logger,
				*reinterpret_cast<GlfwWindow*>(m_window.get())
				)
		),
		m_swapchain(
			std::make_unique<VulkanSwapchain>(
				logger,
				*reinterpret_cast<VulkanGraphicsContext*>(m_graphicsContext.get()),
				Application::SwapchainInitProps{
					.useSrgb  = true,
					.useVsync = true
				}
			)
		),
		m_textInputService(eventDispatcher),
		m_deltaTimeService(*m_multimediaFramework.get()),
		m_performanceCountingService(*m_multimediaFramework.get())
	{}
}
