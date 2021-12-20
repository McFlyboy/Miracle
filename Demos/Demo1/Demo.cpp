#include <Miracle/Miracle.hpp>

using namespace Miracle;

Input::Devices::IKeyboard* keyboard;

void start() {
	keyboard = MiracleApp::getCurrentApp()->getKeyboard();
}

void update() {
	if (keyboard->keyIsDown(Input::Devices::IKeyboard::Key::Space)) {
		Diagnostics::Logger::info("Holding space");
	}

	if (keyboard->keyPressed(Input::Devices::IKeyboard::Key::W)) {
		Diagnostics::Logger::info("Pressed W");
	}

	if (keyboard->keyPressedOrRepeated(Input::Devices::IKeyboard::Key::E)) {
		Diagnostics::Logger::info("Pressed or repeated E");
	}
}

int main() {
	auto app = MiracleAppBuilder()
		.configureWindow(
			[](View::WindowProps& props) {
				props.title = u8"Demo 1";
			}
		).setStartScript(start)
		.setUpdateScript(update)
		.build();

	int exitCode = app.run();

	return exitCode;
}
