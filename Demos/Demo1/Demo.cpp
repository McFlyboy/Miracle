#include <Miracle/Miracle.hpp>

using namespace Miracle;

MiracleApp* currentApp = nullptr;
View::IWindow* window = nullptr;
Input::Devices::IKeyboard* keyboard = nullptr;

int main() {
	auto app = MiracleAppBuilder()
		.configureWindow(
			[](View::WindowProps& props) {
				props.title = u8"Demo 1";
				props.width = 800;
				props.height = 600;
				props.resizable = true;
			}
		).setStartScript(
			[]() {
				currentApp = MiracleApp::getCurrentApp();
				window = currentApp->getWindow();
				keyboard = currentApp->getKeyboard();
			}
		).setUpdateScript(
			[]() {
				if (keyboard->keyPressed(Input::Devices::IKeyboard::Key::Escape)) {
					currentApp->close();
				}

				if (keyboard->keyPressed(Input::Devices::IKeyboard::Key::R)) {
					window->setResizable(!window->isResizable());
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
