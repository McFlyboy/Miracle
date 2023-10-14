#pragma once

#include <string_view>
#include <utility>

#include <Miracle/Common/Models/WindowConfig.hpp>
#include <Miracle/Common/Models/RendererConfig.hpp>
#include <Miracle/Common/Models/SceneConfig.hpp>
#include "Graphics/Renderer.hpp"
#include "Models/Scene.hpp"
#include "IWindow.hpp"

namespace Miracle::Application {
	class Mappings {
	public:
		Mappings() = delete;

		static WindowInitProps toWindowInitProps(
			const WindowConfig& windowConfig,
			const std::u8string_view& defaultTitle
		) {
			return WindowInitProps{
				.title     = windowConfig.title.has_value() ? windowConfig.title.value() : defaultTitle,
				.size      = windowConfig.size,
				.resizable = windowConfig.resizable
			};
		}

		static RendererInitProps toRendererInitProps(
			const RendererConfig& rendererConfig
		) {
			return RendererInitProps{
				.swapchainInitProps = SwapchainInitProps{
					.useSrgb  = false,
					.useVsync = false
				},
				.meshes               = rendererConfig.meshes
			};
		}

		static SceneInitProps toSceneInitProps(
			const SceneConfig& sceneConfig
		) {
			return SceneInitProps{
				.backgroundColor         = sceneConfig.backgroundColor,
				.entityConfigs           = sceneConfig.entityConfigs,
				.entityCreatedCallback   = sceneConfig.entityCreatedCallback,
				.entityDestroyedCallback = sceneConfig.entityDestroyedCallback
			};
		}
	};
}
