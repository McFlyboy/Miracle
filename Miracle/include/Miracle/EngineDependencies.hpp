#pragma once

#include <string>
#include <memory>

#include "Common/Models/WindowConfig.hpp"
#include "Application/ILogger.hpp"
#include "Application/EventDispatcher.hpp"
#include "Application/IMultimediaFramework.hpp"
#include "Application/IWindow.hpp"
#include "Application/IKeyboard.hpp"
#include "Application/TextInputService.hpp"
#include "Application/DeltaTimeService.hpp"
#include "Application/PerformanceCountingService.hpp"

namespace Miracle {
	class EngineDependencies {
	private:
		std::unique_ptr<Application::IMultimediaFramework> m_multimediaFramework;
		std::unique_ptr<Application::IWindow> m_window;
		std::unique_ptr<Application::IKeyboard> m_keyboard;
		Application::TextInputService m_textInputService;
		Application::DeltaTimeService m_deltaTimeService;
		Application::PerformanceCountingService m_performanceCountingService;

	public:
		EngineDependencies(
			const std::string_view& appName,
			const WindowConfig& windowConfig,
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
