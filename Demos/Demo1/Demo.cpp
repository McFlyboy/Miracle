#include <Miracle/Miracle.hpp>

using namespace Miracle;

int main() {
	auto app = App(
		"Demo 1",
		AppInitProps{
			.windowConfig = WindowConfig{
				.size = WindowSize{
					.width  = 800,
					.height = 600
				},
			},
			.startScript = []() {
				PerformanceCounters::setCountersUpdatedCallback(
					[]() {
						Logger::info(
							std::string("FPS: ") + std::to_string(PerformanceCounters::getFps())
								+ " - UPS: " + std::to_string(PerformanceCounters::getUps())
						);
					}
				);
			},
			.updateScript = []() {
				if (Keyboard::isKeyPressed(KeyboardKey::keyEscape)) {
					CurrentApp::close();
				}

				if (Keyboard::isKeyPressed(KeyboardKey::keyR)) {
					Window::setResizable(!Window::isResizable());
				}
			}
		}
	);

	int exitCode = app.run();

	return exitCode;
}
