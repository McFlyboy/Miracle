#include <Miracle/Miracle.hpp>

using namespace Miracle;

int main() {
	Input::Devices::IKeyboard* keyboard = nullptr;

	auto app = MiracleAppBuilder()
		.configureWindow(
			[](View::WindowProps& props) {
				props.title = u8"Demo 1";
			}
		).setStartScript(
			[&keyboard]() {
				keyboard = MiracleApp::getCurrentApp()->getKeyboard();
			}
		).setUpdateScript(
			[&keyboard]() {
				if (keyboard->keyPressed(Input::Devices::IKeyboard::Key::Escape)) {
					MiracleApp::getCurrentApp()->close();
				}

				if (keyboard->keyIsDown(Input::Devices::IKeyboard::Key::Space)) {
					Diagnostics::Logger::info("Holding space");
				}

				if (keyboard->keyPressed(Input::Devices::IKeyboard::Key::W)) {
					Diagnostics::Logger::info("Pressed W");
				}

				if (keyboard->keyPressedContinuously(Input::Devices::IKeyboard::Key::E)) {
					Diagnostics::Logger::info("Pressed E continuously");
				}
			}
		).build();

	int exitCode = app.run();

	return exitCode;
}
