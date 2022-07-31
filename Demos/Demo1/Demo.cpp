#include <Miracle/Miracle.hpp>

using namespace Miracle;

int main() {
	auto text = std::u32string();

	auto app = App(
		"Demo 1",
		AppInitProps{
			.windowConfig = WindowConfig{
				.title  = std::optional<std::u8string>(u8"Demo App™"),
				.width  = 800,
				.height = 600
			},
			.startScript = [&]() {
				text = UnicodeConverter::toUtf32(Window::getTitle());

				TextInput::setTextInputReceiver(
					text,
					[&]() {
						Window::setTitle(UnicodeConverter::toUtf8(text));
					}
				);
			},
			.updateScript = []() {
				Logger::info(std::to_string(CurrentApp::getRuntime()));

				if (Keyboard::isKeyPressed(KeyboardKey::keyEscape)) {
					CurrentApp::close();
				}
			}
		}
	);

	int exitCode = app.run();

	return exitCode;
}
