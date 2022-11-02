#pragma once

#include <string_view>
#include <utility>

#include <Miracle/Application/IWindow.hpp>
#include <Miracle/Application/Graphics/Renderer.hpp>
#include "Models/WindowConfig.hpp"
#include "Models/RendererConfig.hpp"

namespace Miracle {
	class Mappings {
	public:
		Mappings() = delete;

		static inline Application::WindowInitProps toWindowInitProps(
			const WindowConfig& windowConfig,
			const std::u8string_view& defaultTitle
		) {
			return Application::WindowInitProps{
				.title     = windowConfig.title.has_value() ? windowConfig.title.value() : defaultTitle,
				.size      = windowConfig.size,
				.resizable = windowConfig.resizable
			};
		}

		static inline Application::RendererInitProps toRendererInitProps(
			const RendererConfig& rendererConfig
		) {
			return Application::RendererInitProps{
				.swapchainInitProps = Application::SwapchainInitProps{
					.useSrgb  = false,
					.useVsync = false
				},
				.clearColor         = rendererConfig.clearColor,
				.mesh               = std::move(rendererConfig.mesh)
			};
		}
	};
}
