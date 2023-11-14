#include <Miracle/App.hpp>

#include <utility>
#include <exception>
#include <format>

#include "Infrastructure/Diagnostics/Spdlog/Logger.hpp"
#include "Infrastructure/View/TinyFileDialogs/MessageBox.hpp"

using namespace Miracle::Infrastructure::View::TinyFileDialogs;

namespace Miracle {
	using LoggerBackend = Miracle::Infrastructure::Diagnostics::Spdlog::Logger;

	App::App(
		std::string&& name,
		AppConfig&& config,
		UserData&& userData
	) :
		m_name(std::move(name)),
		m_config(std::move(config)),
		m_userData(std::move(userData)),
		m_logger(std::make_unique<LoggerBackend>())
	{}

	void App::setUserData(const UserData& userData) {
		m_userData = userData;
	}

	int App::run() {
		m_logger->info("Starting Miracle");

		s_currentApp = this;

		runEngine();

		m_logger->info("Shutting down...");

		s_currentApp = nullptr;

		return std::exchange(m_exitCode, 0);
	}

	void App::runEngine() {
		std::unique_ptr<EngineDependencies> dependencies = nullptr;

		try {
			dependencies = std::make_unique<EngineDependencies>(
				m_name,
				m_config.windowConfig,
				m_config.rendererConfig,
				m_config.sceneConfig,
				*m_logger.get(),
				m_dispatcher
			);
		}
		catch (const MiracleError& e) {
			showError(e);
			m_exitCode = static_cast<int>(e.getErrorCode());
			return;
		}

		m_dependencies = dependencies.get();

		m_dependencies->getWindow().show();

		runApp();

		m_logger->info("Closing Miracle");

		m_dependencies = nullptr;
	}

	void App::runApp() {
		m_logger->info(std::format("Running app: {}", getName()));

		auto& framework = m_dependencies->getMultimediaFramework();
		auto& window = m_dependencies->getWindow();
		auto& keyboard = m_dependencies->getKeyboard();
		auto& renderer = m_dependencies->getRenderer();
		auto& sceneManager = m_dependencies->getSceneManager();
		auto& deltaTimeService = m_dependencies->getDeltaTimeService();
		auto& performanceCountingService = m_dependencies->getPerformanceCountingService();

		m_running = true;
		deltaTimeService.updateDeltaTime();

		try {
			m_config.startScript();

			while (m_running) {
				keyboard.setAllKeyStatesAsDated();
				framework.processEvents();

				if (window.shouldClose()) {
					m_running = false;
					continue;
				}

				deltaTimeService.updateDeltaTime();

				m_config.updateScript();
				auto& currentScene = sceneManager.getCurrentScene();
				currentScene.destroyScheduledEntities();
				currentScene.update();
				performanceCountingService.incrementUpdateCounter();

				bool frameRendered = renderer.render(sceneManager.getCurrentScene());

				if (frameRendered) {
					performanceCountingService.incrementFrameCounter();
				}

				performanceCountingService.updateCounters();
			}
		}
		catch (const MiracleError& e) {
			showError(e);
			m_exitCode = static_cast<int>(e.getErrorCode());
			m_running = false;
		}
	}

	void App::showError(const MiracleError& error) const {
		m_logger->info("Showing error message");

		MessageBox(
			"Error",
			std::format("{0:#010x}: {1}", error.getErrorCode(), error.what()),
			MessageBoxIcon::error
		).show();
	}
}
