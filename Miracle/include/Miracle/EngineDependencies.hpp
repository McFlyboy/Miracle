#pragma once

#include <string>
#include <memory>

#include "Common/Models/WindowConfig.hpp"
#include "Common/Models/RendererConfig.hpp"
#include "Common/Models/SceneConfig.hpp"
#include "Application/ILogger.hpp"
#include "Application/EventDispatcher.hpp"
#include "Application/IFileAccess.hpp"
#include "Application/IMultimediaFramework.hpp"
#include "Application/IWindow.hpp"
#include "Application/IKeyboard.hpp"
#include "Application/Graphics/IGraphicsApi.hpp"
#include "Application/Graphics/IGraphicsContext.hpp"
#include "Application/IEcs.hpp"
#include "Application/Graphics/Renderer.hpp"
#include "Application/SceneManager.hpp"
#include "Application/TextInputService.hpp"
#include "Application/DeltaTimeService.hpp"
#include "Application/PerformanceCountingService.hpp"

namespace Miracle {
	class EngineDependencies {
	private:
		std::unique_ptr<Application::IFileAccess> m_fileAccess;
		std::unique_ptr<Application::IMultimediaFramework> m_multimediaFramework;
		std::unique_ptr<Application::IWindow> m_window;
		std::unique_ptr<Application::IKeyboard> m_keyboard;
		std::unique_ptr<Application::IGraphicsApi> m_graphicsApi;
		std::unique_ptr<Application::IGraphicsContext> m_graphicsContext;
		std::unique_ptr<Application::IEcs> m_ecs;
		Application::Renderer m_renderer;
		Application::SceneManager m_sceneManager;
		Application::TextInputService m_textInputService;
		Application::DeltaTimeService m_deltaTimeService;
		Application::PerformanceCountingService m_performanceCountingService;

	public:
		EngineDependencies(
			const std::string_view& appName,
			const WindowConfig& windowConfig,
			const RendererConfig& rendererConfig,
			const SceneConfig& sceneConfig,
			Application::ILogger& logger,
			Application::EventDispatcher& eventDispatcher
		);

		inline Application::IMultimediaFramework& getMultimediaFramework() {
			return *m_multimediaFramework.get();
		}

		inline Application::IWindow& getWindow() {
			return *m_window.get();
		}

		inline Application::IKeyboard& getKeyboard() {
			return *m_keyboard.get();
		}

		inline Application::Renderer& getRenderer() {
			return m_renderer;
		}

		inline Application::SceneManager& getSceneManager() {
			return m_sceneManager;
		}

		inline Application::TextInputService& getTextInputService() {
			return m_textInputService;
		}

		inline Application::DeltaTimeService& getDeltaTimeService() {
			return m_deltaTimeService;
		}

		inline Application::PerformanceCountingService& getPerformanceCountingService() {
			return m_performanceCountingService;
		}
	};
}
