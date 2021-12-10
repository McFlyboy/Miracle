#include <Miracle/Miracle.hpp>

using namespace Miracle;

int main() {
	auto app = MiracleAppBuilder()
		.configureWindow(
			[] (View::WindowProps& props) {
				props.title = "Demo 1";
				props.width = 1366;
				props.height = 768;
			}
		).build();

	int exitCode = app.run();

	return exitCode;
}
