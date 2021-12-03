#include <Miracle/MiracleApp.hpp>

#include <spdlog/spdlog.h>

namespace Miracle {
	int MiracleApp::run() {
		spdlog::info("Hello Miracle!");

		return 0;
	}
}
