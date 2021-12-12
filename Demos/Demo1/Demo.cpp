#include <Miracle/Miracle.hpp>

using namespace Miracle;

int main() {
	auto app = MiracleAppBuilder()
		.configureWindow(
			[](View::WindowProps& props) {
				props.title = u8"Demo 1";
			}
		).setStartScript(
			[]() {
				
			}
		).setUpdateScript(
			[]() {
				
			}
		).build();

	int exitCode = app.run();

	return exitCode;
}
