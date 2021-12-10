#include <Miracle/components/Miracle/MiracleApp.hpp>

#include <Miracle/components/Miracle/Diagnostics/Logger.hpp>
#include <Miracle/View/Implementations/Window.hpp>

using namespace Miracle::Diagnostics;
using namespace Miracle::View;
using namespace Miracle::View::Implementations;

namespace Miracle {
	MiracleApp::MiracleApp(const WindowProps& windowProps) : m_windowProps(windowProps) {
		Logger::initialize();
	}

	int MiracleApp::run() const {
		Logger::info("Starting Miracle");

		int exitCode = 0;

		try {
			const IWindow& window = Window(m_windowProps);

			while (!window.shouldClose()) {
				window.update();
			}

			Logger::info("Closing Miracle");
		}
		catch (int& errorCode) {
			exitCode = errorCode;
		}

		Logger::info("Shutting down...");

		return exitCode;
	}
}
