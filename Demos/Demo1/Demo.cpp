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
			.rendererConfig = RendererConfig{
				.clearColor = Color3f{
					.red   = 0.5f,
					.green = 0.1f,
					.blue  = 0.1f
				},
				.mesh = Mesh{
					.vertices = {
						Vertex{ .position = Vector2f{ .x = -0.5f, .y = -0.5f } },
						Vertex{ .position = Vector2f{ .x =  0.5f, .y = -0.5f } },
						Vertex{ .position = Vector2f{ .x =  0.5f, .y =  0.5f } },
						Vertex{ .position = Vector2f{ .x = -0.5f, .y =  0.5f } }
					},
					.faces = {
						Face{ .indices = { 0, 1, 2 } },
						Face{ .indices = { 0, 2, 3 } },
					}
				}
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

				if (Keyboard::isKeyPressedOrRepeated(KeyboardKey::keyC)) {
					auto& currentColor = Renderer::getClearColor();
					Renderer::setClearColor(
						Color3f{
							.red   = currentColor.green,
							.green = currentColor.blue,
							.blue  = currentColor.red
						}
					);
				}
			}
		}
	);

	int exitCode = app.run();

	return exitCode;
}
