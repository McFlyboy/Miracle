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
				Logger::info(std::to_string(DeltaTime::get()));

				if (Keyboard::isKeyPressed(KeyboardKey::keyEscape)) {
					CurrentApp::close();
				}
			}
		}
	);

	int exitCode = app.run();

	return exitCode;
}
