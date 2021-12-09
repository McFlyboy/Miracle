#include <Miracle/components/Miracle/MiracleApp.hpp>

#include <Miracle/components/Miracle/Diagnostics/Logger.hpp>
#include <Miracle/View/Implementations/Window.hpp>

using namespace Miracle::Diagnostics;
using namespace Miracle::View;
using namespace Miracle::View::Implementations;

namespace Miracle {
	int MiracleApp::run() const {
		Logger::initialize();
		Logger::info("Starting Miracle");

		{
			const IWindow& window = Window(WindowProps());

			while (!window.shouldClose()) {
				window.update();
			}

			Logger::info("Closing Miracle");
		}

		Logger::info("Shutting down...");

		return 0;
	}
}
