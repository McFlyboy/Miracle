#include <Miracle/Miracle.hpp>

using namespace Miracle;

class ProjectileBehaviour : public Behaviour {
private:
	Vector3 m_velocity;

public:
	ProjectileBehaviour(
		const BehaviourDependencies& dependencies,
		const Vector3& velocity
	) : Behaviour(dependencies),
		m_velocity(velocity)
	{}

	virtual void act() override {
		*reinterpret_cast<Vector3*>(&m_entityTransform.getTransformation()[3]) += m_velocity * DeltaTime::get();
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

		*reinterpret_cast<Vector3*>(&m_entityTransform.getTransformation()[3]) += velocity.toNormalized() * m_movementSpeed * DeltaTime::get();

		auto translation = *reinterpret_cast<Vector3*>(&m_entityTransform.getTransformation()[3]);

		m_entityTransform.getTransformation() *= Matrix4::createTranslation(-translation)
			* Matrix4::createRotation(Vector3::forward, 90.0_deg * DeltaTime::get())
			* Matrix4::createTranslation(translation);

		if (Keyboard::isKeyPressed(KeyboardKey::keySpace)) {
			CurrentScene::addEntity(
				EntityConfig{
					.transformConfig = TransformConfig{
						.translation = *reinterpret_cast<Vector3*>(&m_entityTransform.getTransformation()[3]),
						.scale = Vector3{ .x = 0.25f, .y = 0.125f, .z = 1.0f }
					},
					.behaviourFactory = BehaviourFactory::createFactoryFor<ProjectileBehaviour>(
						Vector3{ .x = m_horizontalDirection * 5.0f }
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
