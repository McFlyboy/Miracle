#include <Miracle/Miracle.hpp>

using namespace Miracle;

class PlayerBehaviour : public Behaviour {
	virtual void update() override {
		auto position = Scene::getEntityPosition();

		if (Keyboard::isKeyHeld(KeyboardKey::keyW)) {
			position.y += 0.5f * DeltaTime::get();
		}

		if (Keyboard::isKeyHeld(KeyboardKey::keyS)) {
			position.y -= 0.5f * DeltaTime::get();
		}

		if (Keyboard::isKeyHeld(KeyboardKey::keyD)) {
			position.x += 0.5f * DeltaTime::get();
		}

		if (Keyboard::isKeyHeld(KeyboardKey::keyA)) {
			position.x -= 0.5f * DeltaTime::get();
		}

		Scene::setEntityPosition(position);
	}
};

int main() {
	auto app = App(
		"Demo 1",
		AppConfig{
			.windowConfig = WindowConfig{
				.size = WindowSize{
					.width  = 800,
					.height = 600
				},
			},
			.rendererConfig = RendererConfig{
				.mesh = Mesh{
					.vertices = {
						Vertex{ .position = Vector2f{ .x = -0.5f, .y = -0.5f } },
						Vertex{ .position = Vector2f{ .x =  0.5f, .y = -0.5f } },
						Vertex{ .position = Vector2f{ .x =  0.5f, .y =  0.5f } },
						Vertex{ .position = Vector2f{ .x = -0.5f, .y =  0.5f } }
					},
					.faces = {
						Face{ .indices = { 0, 1, 2 } },
						Face{ .indices = { 0, 2, 3 } }
					}
				}
			},
			.sceneConfig = SceneConfig{
				.backgroundColor = Color3f{
					.red   = 0.5f,
					.green = 0.1f,
					.blue  = 0.1f
				},
				.entityBehaviourFactory = []() { return std::make_unique<PlayerBehaviour>(); }
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
					auto& currentColor = Scene::getBackgroundColor();
					Scene::setBackgroundColor(
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
