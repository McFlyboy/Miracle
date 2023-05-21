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
		m_entityTransform.getTranslation() += m_velocity * DeltaTime::get();
	}
};

class PlayerBehaviour : public Behaviour {
private:
	float m_movementSpeed;

public:
	PlayerBehaviour(
		const BehaviourDependencies& dependencies,
		float movementSpeed = 0.5f
	) : Behaviour(dependencies),
		m_movementSpeed(movementSpeed)
	{}
	
	virtual void act() override {
		auto velocity = Vector3{
			.x = static_cast<float>(Keyboard::isKeyHeld(KeyboardKey::keyD) - Keyboard::isKeyHeld(KeyboardKey::keyA)),
			.y = static_cast<float>(Keyboard::isKeyHeld(KeyboardKey::keyW) - Keyboard::isKeyHeld(KeyboardKey::keyS)),
		};

		float rotation = Keyboard::isKeyHeld(KeyboardKey::keyQ) - Keyboard::isKeyHeld(KeyboardKey::keyE);

		m_entityTransform.getRotation() *= Quaternion::createRotation(Vector3::forward, 180.0_deg * rotation * DeltaTime::get());
		m_entityTransform.getTranslation() += MathUtilities::rotateVector(velocity.toNormalized(), m_entityTransform.getRotation())
			* m_movementSpeed * DeltaTime::get();

		if (Keyboard::isKeyPressed(KeyboardKey::keySpace)) {
			CurrentScene::addEntity(
				EntityConfig{
					.transformConfig = TransformConfig{
						.translation = m_entityTransform.getTranslation(),
						.rotation = m_entityTransform.getRotation(),
						.scale = Vector3{ .x = 0.0625f, .y = 0.125f, .z = 1.0f }
					},
					.behaviourFactory = BehaviourFactory::createFactoryFor<ProjectileBehaviour>(
						MathUtilities::rotateVector(Vector3::up, m_entityTransform.getRotation()) * 3.0f
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
						.transformConfig = TransformConfig{
							.scale = Vector3{.x = 0.25f, .y = 0.25f, .z = 0.25f }
						},
						.behaviourFactory = BehaviourFactory::createFactoryFor<PlayerBehaviour>(2.0f)
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
