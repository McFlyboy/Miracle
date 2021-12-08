#include <Miracle/MiracleApp.hpp>

#include <Miracle/Diagnostics/Logger.hpp>

#include <Miracle/Window/Implementations/Window.hpp>

using namespace Miracle::Diagnostics;
using namespace Miracle::Window;

namespace Miracle {
	int MiracleApp::run() const {
		Logger::initialize();
		Logger::info("Starting Miracle");

		IWindow window = Implementations::Window();

		Logger::info("Shutting down...");

		return 0;
	}
}
