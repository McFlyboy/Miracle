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

class CameraBehavior : public BehaviorBase {
private:
	float m_movementSpeed;

public:
	CameraBehavior(
		const EntityContext& context,
		float movementSpeed
	) : BehaviorBase(context),
		m_movementSpeed(movementSpeed)
	{}

	virtual void act() {
		auto velocity = Vector3{
			.x = static_cast<float>(Keyboard::isKeyHeld(KeyboardKey::keyRight) - Keyboard::isKeyHeld(KeyboardKey::keyLeft)),
			.y = static_cast<float>(Keyboard::isKeyHeld(KeyboardKey::keyUp) - Keyboard::isKeyHeld(KeyboardKey::keyDown)),
		};

		m_context.getTransform().translate(velocity.toNormalized() * m_movementSpeed * DeltaTime::get());
	}
};

class BulletBehavior : public BehaviorBase {
private:
	Vector3 m_velocity;

public:
	BulletBehavior(
		const EntityContext& context,
		float movementSpeed
	) : BehaviorBase(context),
		m_velocity(Vector3s::up * movementSpeed)
	{}

	virtual void act() {
		auto& transform = m_context.getTransform();
		transform.translate(m_velocity * DeltaTime::get());

		if (transform.getTranslation().distanceTo(Vector3s::zero) > 10.0f)
			m_context.destroyEntity();
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

	virtual void act() {
		auto velocity = (Vector3s::up * (static_cast<float>(Keyboard::isKeyHeld(KeyboardKey::keyW) - Keyboard::isKeyHeld(KeyboardKey::keyS))))
			.toNormalized() * m_movementSpeed;

		auto deltaRotation = m_turnSpeed
			* static_cast<float>(Keyboard::isKeyHeld(KeyboardKey::keyA) - Keyboard::isKeyHeld(KeyboardKey::keyD));

		auto& transform = m_context.getTransform();

		transform.rotate(Quaternion::createRotation(Vector3s::forward, deltaRotation * DeltaTime::get()));
		transform.translate(velocity * DeltaTime::get());

		if (Keyboard::isKeyPressed(KeyboardKey::keySpace))
			CurrentScene::createEntity(
				EntityConfig{
					.transformConfig = TransformConfig{
						.translation = transform.getTranslation(),
						.rotation    = transform.getRotation(),
						.scale       = Vector3{ .x = 1.0f, .y = 1.0f } * 0.25f
					},
					.appearanceConfig = AppearanceConfig{
						.meshIndex = 0,
						.color     = ColorRgbs::cyan
					},
					.behaviorFactory = BehaviorFactory::createFactoryFor<BulletBehavior>(5.0f)
				}
			);
	}
};

int main() {
	auto app = App(
		"Demo 1",
		AppConfig{
			.windowConfig = WindowConfig{
				.size = WindowSize{
					.width = 800,
					.height = 600
				},
				.resizable = true
			},
			.rendererConfig = RendererConfig{
				.swapchainConfig = SwapchainConfig{
					.useVsync = true
				},
				.meshes = std::vector{
					Mesh{
						.vertices = std::vector{
							Vertex{ .position = Vector3{ .x = -0.5f, .y = -0.5f } },
							Vertex{ .position = Vector3{ .x =  0.5f, .y = -0.5f } },
							Vertex{ .position = Vector3{ .x =  0.0f, .y =  0.5f } }
						},
						.faces = std::vector{
							Face{ .indices = { 0, 1, 2 } }
						}
					},
					Mesh{
						.vertices = std::vector{
							Vertex{ .position = Vector3{ .x = -0.5f, .y = -0.5f } },
							Vertex{ .position = Vector3{ .x =  0.5f, .y = -0.5f } },
							Vertex{ .position = Vector3{ .x =  0.5f, .y =  0.5f } },
							Vertex{ .position = Vector3{ .x = -0.5f, .y =  0.5f } }
						},
						.faces = std::vector{
							Face{ .indices = { 0, 1, 2 } },
							Face{ .indices = { 0, 2, 3 } }
						}
					}
				}
			},
			.sceneConfig = SceneConfig{
				.entityConfigs = std::vector{
					EntityConfig{
						.transformConfig = TransformConfig{},
						.cameraConfig    = OrthographicCameraConfig{},
						.behaviorFactory = BehaviorFactory::createFactoryFor<CameraBehavior>(2.0f)
					},
					EntityConfig{
						.appearanceConfig = AppearanceConfig{
							.meshIndex = 1,
							.color     = ColorRgbs::magenta
						},
						.behaviorFactory = BehaviorFactory::createFactoryFor<PlayerBehavior>(8.0f, 225.0_deg)
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

				if (Keyboard::isKeyPressed(KeyboardKey::keyF1)) {
					Renderer::setVsync(!Renderer::isUsingVsync());
				}

				if (Keyboard::isKeyPressed(KeyboardKey::keyF2)) {
					Renderer::setTripleBuffering(!Renderer::isUsingTripleBuffering());
				}
			}
		}
	);

	int exitCode = app.run();
	
	return exitCode;
}
