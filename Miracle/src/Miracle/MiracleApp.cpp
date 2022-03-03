#include <Miracle/components/Miracle/MiracleApp.hpp>

#include <memory>

#include "EngineDependencies.hpp"
#include <Miracle/components/Miracle/Diagnostics/Logger.hpp>
#include "MiracleError.hpp"

using namespace Miracle::Diagnostics;
using namespace Miracle::View;

namespace Miracle {
	MiracleApp* MiracleApp::s_currentApp = nullptr;

	MiracleApp::MiracleApp(
		const WindowProps& windowProps,
		const std::function<void()>& startScript,
		const std::function<void()>& updateScript
	) :
		m_windowProps(windowProps),
		m_startScript(startScript),
		m_updateScript(updateScript)
	{
		Logger::initialize();
	}

	int MiracleApp::run() {
		Logger::info("Starting Miracle");

		s_currentApp = this;
		m_exitCode = 0;

		runEngine();

		Logger::info("Shutting down...");

		s_currentApp = nullptr;

		return m_exitCode;
	}

	void MiracleApp::close(int exitCode) {
		m_exitCode = exitCode;
		m_window->close();
	}

	void MiracleApp::runEngine() {
		std::unique_ptr<EngineDependencies> dependencies = nullptr;

		try {
			dependencies = std::make_unique<EngineDependencies>(m_windowProps);
		}
		catch (const MiracleError& error) {
			m_exitCode = static_cast<int>(error);
			return;
		}

		m_window = &dependencies->getWindow();
		m_keyboard = &dependencies->getKeyboard();

		m_window->show();

		m_startScript();

		while (!m_window->shouldClose()) {
			m_updateScript();

			m_window->update();
		}

		Logger::info("Closing Miracle");

		m_window = nullptr;
		m_keyboard = nullptr;
	}
}
