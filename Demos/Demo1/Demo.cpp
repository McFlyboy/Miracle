#include <Miracle/Miracle.hpp>

using namespace Miracle;

int main() {
	auto app = App(
		"Demo 1",
		AppInitProps{
			.windowConfig = WindowConfig{
				.width  = 800,
				.height = 600
			},
			.updateScript = []() {
				if (Keyboard::isKeyPressed(KeyboardKey::keyE)) {
					Logger::info("E");
				}

				if (Keyboard::isKeyPressedOrRepeated(KeyboardKey::keyR)) {
					Logger::info("R");
				}

				if (Keyboard::isKeyHeld(KeyboardKey::keySpace)) {
					Logger::info("Space");
				}

				if (Keyboard::isKeyPressed(KeyboardKey::keyEscape)) {
					CurrentApp::close();
				}
			}
		}
	);

	int exitCode = app.run();

	return exitCode;
}
