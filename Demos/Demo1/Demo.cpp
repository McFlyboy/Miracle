#include <Miracle/Miracle.hpp>

using namespace Miracle;

class BullettBehaviour : public Behaviour {
private:
	float m_direction;

public:
	BullettBehaviour(
		const EntityProps& entityProps,
		float direction
	) : Behaviour(entityProps),
		m_direction(direction)
	{}

	virtual void act() override {
		m_entityProps.position.x += 5.0f * m_direction * DeltaTime::get();
	}
};

class Player1Behaviour : public Behaviour {
private:
	float m_direction;

public:
	Player1Behaviour(const EntityProps& entityProps) : Behaviour(entityProps),
		m_direction(1.0f)
	{}

	virtual void act() override {
		if (Keyboard::isKeyHeld(KeyboardKey::keyW)) {
			m_entityProps.position.y += 0.5f * DeltaTime::get();
		}

		if (Keyboard::isKeyHeld(KeyboardKey::keyS)) {
			m_entityProps.position.y -= 0.5f * DeltaTime::get();
		}

		if (Keyboard::isKeyHeld(KeyboardKey::keyD)) {
			m_entityProps.position.x += 0.5f * DeltaTime::get();
			m_direction = 1.0f;
		}

		if (Keyboard::isKeyHeld(KeyboardKey::keyA)) {
			m_entityProps.position.x -= 0.5f * DeltaTime::get();
			m_direction = -1.0f;
		}

		if (Keyboard::isKeyPressed(KeyboardKey::keySpace)) {
			Scene::addEntity(
				EntityConfig{
					.position = m_entityProps.position,
					.behaviourFactory = [&](const EntityProps& entityProps) {
						return std::make_unique<BullettBehaviour>(entityProps, m_direction);
					}
				}
			);
		}
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
				.entityConfigs = std::vector<EntityConfig>{
					{
						.position = Vector2f{
							.x = 0.5f,
							.y = 0.2f
						},
						.behaviourFactory = [](const EntityProps& entityProps) {
							return std::make_unique<Player1Behaviour>(entityProps);
						}
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
