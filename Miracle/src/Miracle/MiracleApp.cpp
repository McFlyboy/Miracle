#include <Miracle/components/Miracle/MiracleApp.hpp>

#include <Miracle/components/Miracle/Diagnostics/Logger.hpp>
#include <Miracle/View/Implementations/Window.hpp>

using namespace Miracle::Diagnostics;
using namespace Miracle::View;
using namespace Miracle::View::Implementations;

namespace Miracle {
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

	int MiracleApp::run() const {
		Logger::info("Starting Miracle");

		int exitCode = 0;

		try {
			Window window = Window(m_windowProps);

			m_startScript();

			while (!window.shouldClose()) {
				m_updateScript();

				window.update();
			}

			Logger::info("Closing Miracle");
		}
		catch (const WindowError& error) {
			switch (error) {
			case WindowError::initializationError:
				exitCode = 1;
				break;

			case WindowError::windowCreationError:
				exitCode = 2;
				break;
			default:
				exitCode = -1;
			}
		}

		Logger::info("Shutting down...");

		return exitCode;
	}
}
