#pragma once

#include <string>
#include <string_view>
#include <functional>
#include <optional>
#include <any>
#include <memory>

#include <Miracle/EngineDependencies.hpp>
#include "Application/EventDispatcher.hpp"
#include "Application/ILogger.hpp"
#include "Common/MiracleError.hpp"
#include "Common/Models/WindowConfig.hpp"
#include "Common/Models/RendererConfig.hpp"
#include "Common/Models/SceneConfig.hpp"

namespace Miracle {
	using StartScript = std::function<void()>;
	using UpdateScript = std::function<void()>;
	using UserData = std::optional<std::any>;

	struct AppConfig {
		WindowConfig windowConfig = {};
		RendererConfig rendererConfig = {};
		SceneConfig sceneConfig = {};
		StartScript startScript = []() {};
		UpdateScript updateScript = []() {};
	};

	class App {
		friend class CurrentApp;
		friend class Logger;
		friend class Window;
		friend class Keyboard;
		friend class CurrentScene;
		friend class TextInput;
		friend class Clipboard;
		friend class DeltaTime;
		friend class PerformanceCounters;

	private:
		static inline App* s_currentApp = nullptr;

		const std::string m_name;
		const AppConfig m_config;
		UserData m_userData;
		Application::EventDispatcher m_dispatcher;
		const std::unique_ptr<Application::ILogger> m_logger;
		EngineDependencies* m_dependencies = nullptr;
		int m_exitCode = 0;
		bool m_running = false;

	public:
		App(
			std::string&& name,
			AppConfig&& config = {},
			UserData&& userData = {}
		);

		const std::string& getName() const { return m_name; }

		const UserData& getUserData() const { return m_userData; }

		void setUserData(const UserData& userData);

		int run();

	private:
		void runEngine();

		void runApp();

		void showError(const MiracleError& error) const;
	};

	class NoAppRunningError : public UncategorizedError {
	public:
		NoAppRunningError() : UncategorizedError(
			UncategorizedError::ErrorValue::noAppRunningError,
			"Failed on calling app functionality. No app is currently running..."
		) {}
	};
}
