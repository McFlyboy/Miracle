#pragma once

#include <Miracle/View/Implementations/Window.hpp>
#include <Miracle/Input/Devices/Implementations/Keyboard.hpp>
#include <Miracle/Io/ResourceLoader.hpp>
#include <Miracle/Graphics/Implementations/Vulkan/GraphicsEngine.hpp>

namespace Miracle {
	class EngineDependencies {
	private:
		View::Implementations::Window m_window;
		Input::Devices::Implementations::Keyboard m_keyboard;
		Io::ResourceLoader m_resourceLoader;
		Graphics::Implementations::Vulkan::GraphicsEngine m_graphicsEngine;

	public:
		EngineDependencies(const View::WindowProps& windowProps);
		EngineDependencies(const EngineDependencies&) = delete;

		inline View::IWindow& getWindow() { return m_window; }

		inline Input::Devices::IKeyboard& getKeyboard() { return m_keyboard; }

		inline Graphics::IGraphicsEngine& getGraphicsEngine() { return m_graphicsEngine; }
	};
}
