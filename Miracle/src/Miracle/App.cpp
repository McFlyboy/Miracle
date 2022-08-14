#include <Miracle/App.hpp>

#include <utility>
#include <exception>

#include <fmt/format.h>

#include "Infrastructure/Diagnostics/Spdlog/Logger.hpp"
#include "Infrastructure/View/TinyFileDialogs/MessageBox.hpp"

using namespace Miracle::Infrastructure::View::TinyFileDialogs;

namespace Miracle {
	using LoggerBackend = Miracle::Infrastructure::Diagnostics::Spdlog::Logger;

	App::App(
		std::string&& name,
		AppInitProps&& props,
		UserData&& userData
	) :
		m_name(std::move(name)),
		m_windowConfig(std::move(props.windowConfig)),
		m_startScript(std::move(props.startScript)),
		m_updateScript(std::move(props.updateScript)),
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
				m_windowConfig,
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
		m_logger->info(fmt::format("Running app: {}", getName()));

		auto& framework = m_dependencies->getMultimediaFramework();
		auto& window = m_dependencies->getWindow();
		auto& keyboard = m_dependencies->getKeyboard();
		auto& renderer = m_dependencies->getRenderer();
		auto& deltaTimeService = m_dependencies->getDeltaTimeService();
		auto& performanceCountingService = m_dependencies->getPerformanceCountingService();

		m_running = true;

		m_startScript();

		while (m_running) {
			keyboard.setAllKeyStatesAsDated();
			framework.processEvents();

			if (window.shouldClose()) {
				m_running = false;
				continue;
			}

			deltaTimeService.updateDeltaTime();

			m_updateScript();
			performanceCountingService.incrementUpdateCounter();

			renderer.render();
			performanceCountingService.incrementFrameCounter();

			performanceCountingService.updateCounters();
		}
	}

	void App::showError(const MiracleError& error) const {
		m_logger->info("Showing error message");

		MessageBox(
			"Error",
			fmt::format("{0:#010x}: {1}", error.getErrorCode(), error.what()),
			MessageBoxIcon::error
		).show();
	}
}
