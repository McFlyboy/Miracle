#include <Miracle/Miracle.hpp>

using namespace Miracle;

class ProjectileBehaviour : public Behaviour {
private:
	float m_horizontalVelocity;

public:
	ProjectileBehaviour(
		const BehaviourDependencies& dependencies,
		float horizontalVelocity
	) : Behaviour(dependencies),
		m_horizontalVelocity(horizontalVelocity)
	{}

	virtual void act() override {
		m_entityTransform *= Matrix4::createTranslation(
			Vector3{ .x = m_horizontalVelocity * DeltaTime::get() }
		);
	}
};

class PlayerBehaviour : public Behaviour {
private:
	float m_movementSpeed;
	float m_horizontalDirection = 1.0f;

public:
	PlayerBehaviour(
		const BehaviourDependencies& dependencies,
		float movementSpeed = 0.5f
	) : Behaviour(dependencies),
		m_movementSpeed(movementSpeed)
	{}
	
	virtual void act() override {
		auto velocity = Vector3{};

		if (Keyboard::isKeyHeld(KeyboardKey::keyW)) {
			velocity.y += 1.0f;
		}

		if (Keyboard::isKeyHeld(KeyboardKey::keyS)) {
			velocity.y -= 1.0f;
		}

		if (Keyboard::isKeyHeld(KeyboardKey::keyD)) {
			velocity.x += 1.0f;
			m_horizontalDirection = 1.0f;
		}

		if (Keyboard::isKeyHeld(KeyboardKey::keyA)) {
			velocity.x -= 1.0f;
			m_horizontalDirection = -1.0f;
		}

		m_entityTransform *= Matrix4::createTranslation(velocity.toNormalized() * m_movementSpeed * DeltaTime::get());

		if (Keyboard::isKeyPressed(KeyboardKey::keySpace)) {
			CurrentScene::addEntity(
				EntityConfig{
					.position = Vector3{
						.x = m_entityTransform.m41,
						.y = m_entityTransform.m42,
						.z = m_entityTransform.m43
					},
					.behaviourFactory = BehaviourFactory::createFactoryFor<ProjectileBehaviour>(
						m_horizontalDirection * 5.0f
					)
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
				.resizable = true
			},
			.rendererConfig = RendererConfig{
				.mesh = Mesh{
					.vertices = {
						Vertex{ .position = Vector3{ .x = -0.5f, .y = -0.5f, .z = 0.0f } },
						Vertex{ .position = Vector3{ .x =  0.5f, .y = -0.5f, .z = 0.0f } },
						Vertex{ .position = Vector3{ .x =  0.5f, .y =  0.5f, .z = 0.0f } },
						Vertex{ .position = Vector3{ .x = -0.5f, .y =  0.5f, .z = 0.0f } }
					},
					.faces = {
						Face{ .indices = { 0, 1, 2 } },
						Face{ .indices = { 0, 2, 3 } }
					}
				}
			},
			.sceneConfig = SceneConfig{
				.entityConfigs = std::vector<EntityConfig>{
					{
						.behaviourFactory = BehaviourFactory::createFactoryFor<PlayerBehaviour>(3.0f)
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
			}
		}
	);

	int exitCode = app.run();
	
	return exitCode;
}
