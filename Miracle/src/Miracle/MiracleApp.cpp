#include <Miracle/MiracleApp.hpp>

#include <Miracle/Diagnostics/Logger.hpp>

using namespace Miracle::Diagnostics;

namespace Miracle {
	int MiracleApp::run() {
		Logger::initPattern();
		Logger::info("Miracle started");

		Logger::info("Shutting down...");

		return 0;
	}
}
