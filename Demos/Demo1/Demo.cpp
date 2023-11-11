#include <format>

#include <Miracle/Miracle.hpp>

using namespace Miracle;

static void updateTitle() {
	Window::setTitle(
		UnicodeConverter::toUtf8(
			std::format(
				"{} - FPS: {} - UPS: {} - Entity count: {}",
				CurrentApp::getName(),
				PerformanceCounters::getFps(),
				PerformanceCounters::getUps(),
				CurrentScene::getEntityCount()
			)
		)
	);
}

class ProjectileBehavior : public BehaviorBase {
private:
	Vector3 m_velocity;

public:
	ProjectileBehavior(
		const EntityContext& context,
		float movementSpeed
	) : BehaviorBase(context),
		m_velocity(Vector3s::up * movementSpeed)
	{}

	virtual void act() override {
		auto& transform = m_context.getTransform();

		transform.translate(m_velocity * DeltaTime::get());

		if (transform.getTranslation().getLength() > 10.0f) {
			m_context.destroyEntity();
		}
	}
};

class PlayerBehavior : public BehaviorBase {
private:
	float m_movementSpeed;
	Degrees m_turnSpeed;

public:
	PlayerBehavior(
		const EntityContext& context,
		float movementSpeed,
		Degrees turnSpeed
	) : BehaviorBase(context),
		m_movementSpeed(movementSpeed),
		m_turnSpeed(turnSpeed)
	{}
	
	virtual void act() override {
		auto velocity = Vector3{
			.x = static_cast<float>(Keyboard::isKeyHeld(KeyboardKey::keyD) - Keyboard::isKeyHeld(KeyboardKey::keyA)),
			.y = static_cast<float>(Keyboard::isKeyHeld(KeyboardKey::keyW) - Keyboard::isKeyHeld(KeyboardKey::keyS))
		};

		float rotation = Keyboard::isKeyHeld(KeyboardKey::keyQ) - Keyboard::isKeyHeld(KeyboardKey::keyE);

		auto& transform = m_context.getTransform();

		transform.rotate(Quaternion::createRotation(Vector3s::forward, rotation * m_turnSpeed * DeltaTime::get()));
		transform.translate(velocity.toNormalized() * m_movementSpeed * DeltaTime::get());

		if (Keyboard::isKeyPressed(KeyboardKey::keySpace)) {
			CurrentScene::createEntity(
				EntityConfig{
					.transformConfig = TransformConfig{
						.translation = transform.getTranslation(),
						.rotation    = transform.getRotation(),
						.scale       = Vector3{ .x = 0.5f, .y = 0.5f, .z = 1.0f }
					},
					.appearanceConfig = AppearanceConfig{
						.meshIndex = 1,
						.color     = ColorRgbs::magenta
					},
					.behaviorFactory = BehaviorFactory::createFactoryFor<ProjectileBehavior>(3.0f)
				}
			);
		}

		if (Keyboard::isKeyPressed(KeyboardKey::keyDelete)) {
			m_context.destroyEntity();
		}
	}
};

class CameraBehavior : public BehaviorBase {
private:
	float m_movementSpeed;
	Degrees m_turnSpeed;

public:
	CameraBehavior(
		const EntityContext& context,
		float movementSpeed,
		Degrees turnSpeed
	) : BehaviorBase(context),
		m_movementSpeed(movementSpeed),
		m_turnSpeed(turnSpeed)
	{}

	virtual void act() {
		auto velocity = Vector3{
			.x = static_cast<float>(Keyboard::isKeyHeld(KeyboardKey::keyRight) - Keyboard::isKeyHeld(KeyboardKey::keyLeft)),
			.y = static_cast<float>(Keyboard::isKeyHeld(KeyboardKey::keyPageUp) - Keyboard::isKeyHeld(KeyboardKey::keyPageDown)),
			.z = static_cast<float>(Keyboard::isKeyHeld(KeyboardKey::keyUp) - Keyboard::isKeyHeld(KeyboardKey::keyDown))
		};

		float rotation = Keyboard::isKeyHeld(KeyboardKey::keyPeriod) - Keyboard::isKeyHeld(KeyboardKey::keyComma);

		auto& transform = m_context.getTransform();

		transform.rotate(Quaternion::createRotation(Vector3s::up, rotation * m_turnSpeed * DeltaTime::get()));
		transform.translate(velocity.toNormalized() * m_movementSpeed * DeltaTime::get());
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
				.meshes = std::vector<Mesh>{
					{
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
					},
					{
						.vertices = std::vector{
							Vertex{ .position = Vector3{ .x = -0.5f, .y = -0.5f, .z = 0.0f } },
							Vertex{ .position = Vector3{ .x =  0.5f, .y = -0.5f, .z = 0.0f } },
							Vertex{ .position = Vector3{ .x =  0.0f, .y =  0.5f, .z = 0.0f } }
						},
						.faces = std::vector{
							Face{ .indices = { 0, 1, 2 } }
						}
					}
				}
			},
			.sceneConfig = SceneConfig{
				.entityConfigs = std::vector<EntityConfig>{
					{
						.transformConfig = TransformConfig{
							.translation = Vector3{ 0.0f, 0.0f, -1.0f }
						},
						.cameraConfig = PerspectiveCameraConfig{},
						.behaviorFactory = BehaviorFactory::createFactoryFor<CameraBehavior>(1.0f, 120.0_deg)
					},
					{
						.appearanceConfig = AppearanceConfig{
							.meshIndex = 0,
							.color     = ColorRgbs::green
						},
						.behaviorFactory = BehaviorFactory::createFactoryFor<PlayerBehavior>(10.0f, 270.0_deg)
					}
				},
				.entityCreatedCallback   = [](EntityId) { updateTitle(); },
				.entityDestroyedCallback = [](EntityId) { updateTitle(); }
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
