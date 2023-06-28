#include <Miracle/Miracle.hpp>

using namespace Miracle;

static void updateTitle() {
	Window::setTitle(
		UnicodeConverter::toUtf8(
			CurrentApp::getName()
				+ " - FPS: " + std::to_string(PerformanceCounters::getFps())
				+ " - UPS: " + std::to_string(PerformanceCounters::getUps())
				+ " - Entity count: " + std::to_string(CurrentScene::getEntityCount())
		)
	);
}

class ProjectileBehaviour : public Behaviour {
private:
	Vector3 m_velocity;

public:
	ProjectileBehaviour(
		const EntityView& entity,
		float movementSpeed
	) : Behaviour(entity),
		m_velocity(Vector3::up * movementSpeed)
	{}

	virtual void act() override {
		m_entity.getTransform().translate(m_velocity * DeltaTime::get());

		if (m_entity.getTransform().getTranslation().getLength() > 10.0f) {
			CurrentScene::destroyEntity(m_entity.getEntityId());

			updateTitle();
		}
	}
};

class PlayerBehaviour : public Behaviour {
private:
	float m_movementSpeed;
	Degrees m_turnSpeed;

public:
	PlayerBehaviour(
		const EntityView& entity,
		float movementSpeed,
		Degrees turnSpeed
	) : Behaviour(entity),
		m_movementSpeed(movementSpeed),
		m_turnSpeed(turnSpeed)
	{}
	
	virtual void act() override {
		auto velocity = Vector3{
			.x = static_cast<float>(Keyboard::isKeyHeld(KeyboardKey::keyD) - Keyboard::isKeyHeld(KeyboardKey::keyA)),
			.y = static_cast<float>(Keyboard::isKeyHeld(KeyboardKey::keyW) - Keyboard::isKeyHeld(KeyboardKey::keyS)),
		};

		float rotation = Keyboard::isKeyHeld(KeyboardKey::keyQ) - Keyboard::isKeyHeld(KeyboardKey::keyE);

		m_entity.getTransform()
			.rotate(Quaternion::createRotation(Vector3::forward, rotation * m_turnSpeed * DeltaTime::get()));

		m_entity.getTransform()
			.translate(velocity.toNormalized() * m_movementSpeed * DeltaTime::get());

		if (Keyboard::isKeyPressed(KeyboardKey::keySpace)) {
			CurrentScene::createEntity(
				EntityConfig{
					.transformConfig = TransformConfig{
						.translation = m_entity.getTransform().getTranslation(),
						.rotation    = m_entity.getTransform().getRotation(),
						.scale       = Vector3{ .x = 0.25f, .y = 0.5f, .z = 1.0f }
					},
					.behaviourFactory = BehaviourFactory::createFactoryFor<ProjectileBehaviour>(15.0f)
				}
			);
			
			updateTitle();
		}

		if (Keyboard::isKeyPressed(KeyboardKey::keyDelete)) {
			CurrentScene::destroyEntity(m_entity.getEntityId());

			updateTitle();
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
				.resizable = true
			},
			.rendererConfig = RendererConfig{
				.mesh = Mesh{
					.vertices = std::vector{
						Vertex{ .position = Vector3{ .x = -0.5f, .y = -0.5f, .z = 0.0f } },
						Vertex{ .position = Vector3{ .x =  0.5f, .y = -0.5f, .z = 0.0f } },
						Vertex{ .position = Vector3{ .x =  0.5f, .y =  0.5f, .z = 0.0f } },
						Vertex{ .position = Vector3{ .x = -0.5f, .y =  0.5f, .z = 0.0f } }
					},
					.faces = std::vector{
						Face{ .indices = { 0, 1, 2 } },
						Face{ .indices = { 0, 2, 3 } }
					}
				}
			},
			.sceneConfig = SceneConfig{
				.entityConfigs = std::vector<EntityConfig>{
					{
						.behaviourFactory = BehaviourFactory::createFactoryFor<PlayerBehaviour>(10.0f, 270.0_deg)
					}
				}
			},
			.startScript = []() {
				PerformanceCounters::setCountersUpdatedCallback(updateTitle);
				updateTitle();
			},
			.updateScript = []() {
				if (Keyboard::isKeyPressed(KeyboardKey::keyEscape)) {
					CurrentApp::close();
				}
			}
		}
	);

	int exitCode = app.run();

	return exitCode;
}
