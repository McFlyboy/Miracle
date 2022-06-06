#include <Miracle/App.hpp>

#include <memory>

#include <Miracle/Diagnostics/Logger.hpp>
#include "EngineDependencies.hpp"
#include "MiracleError.hpp"

using namespace Miracle::Diagnostics;
using namespace Miracle::View;

namespace Miracle {
	App* App::s_currentApp = nullptr;

	App::App(const AppProps& props) :
		m_props(props)
	{
		Logger::initialize();
	}

	int App::run() {
		Logger::info("Starting Miracle");

		s_currentApp = this;
		m_exitCode = 0;

		runEngine();

		Logger::info("Shutting down...");

		s_currentApp = nullptr;

		return m_exitCode;
	}

	void App::close(int exitCode) {
		m_exitCode = exitCode;
		m_window->close();
	}

	void App::runEngine() {
		std::unique_ptr<EngineDependencies> dependencies = nullptr;

		try {
			dependencies = std::make_unique<EngineDependencies>(m_props.windowProps);
		}
		catch (const MiracleError& error) {
			m_exitCode = static_cast<int>(error);
			return;
		}

		m_window = &dependencies->getWindow();
		m_keyboard = &dependencies->getKeyboard();
		auto& graphicsEngine = dependencies->getGraphicsEngine();

		m_window->show();

		m_props.startScript();

		while (!m_window->shouldClose()) {
			m_window->update();

			m_props.updateScript();

			auto renderError = graphicsEngine.render();

			if (renderError.has_value()) {
				m_exitCode = static_cast<int>(renderError.value());
			}
		}

		Logger::info("Closing Miracle");

		auto waitError = graphicsEngine.waitForExecutionToFinish();

		if (waitError.has_value()) {
			m_exitCode = static_cast<int>(waitError.value());
		}

		m_window = nullptr;
		m_keyboard = nullptr;
	}
}
