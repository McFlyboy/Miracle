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
					.width = 800,
					.height = 600
				},
				.resizable = true
			},
			.rendererConfig = RendererConfig{
				.useDepthTesting = true,
				.meshes          = std::vector{
					Mesh{
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
				}
			},
			.sceneConfig = SceneConfig{
				.entityConfigs = std::vector{
					EntityConfig{
						.transformConfig = TransformConfig{
							.translation = Vector3{ 0.0f, 0.0f, -1.0f }
						},
						.cameraConfig    = PerspectiveCameraConfig{},
						.behaviorFactory = BehaviorFactory::createFactoryFor<CameraBehavior>(2.0f, 120.0_deg)
					},
					EntityConfig{
						.appearanceConfig = AppearanceConfig{
							.meshIndex = 0,
							.color     = ColorRgb::createFromNonlinearSrgbColorCode(0xFF7700)
						}
					},
					EntityConfig{
						.transformConfig = TransformConfig{
							.translation = Vector3{
								.x = -1.5f,
								.z =  1.5f
							}
						},
						.appearanceConfig = AppearanceConfig{
							.meshIndex = 0,
							.color = ColorRgbs::magenta
						}
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

				if (Keyboard::isKeyPressed(KeyboardKey::keyF3)) {
					Renderer::setDepthTesting(!Renderer::isUsingDepthTesting());
				}
			}
		}
	);

	int exitCode = app.run();
	
	return exitCode;
}
