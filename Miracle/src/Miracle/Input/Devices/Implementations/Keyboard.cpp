#include "Keyboard.hpp"

#include <GLFW/glfw3.h>

namespace Miracle::Input::Devices::Implementations {
	static std::array<Keyboard::KeyStates, static_cast<size_t>(Keyboard::Key::Last) + 1> keyStates;

	static inline Keyboard::KeyStates operator|(
		const Keyboard::KeyStates& states1,
		const Keyboard::KeyStates& states2
	) {
		return static_cast<Keyboard::KeyStates>(
			static_cast<int8_t>(states1) | static_cast<int8_t>(states2)
		);
	}

	static inline Keyboard::KeyStates operator&(
		const Keyboard::KeyStates& states1,
		const Keyboard::KeyStates& states2
	) {
		return static_cast<Keyboard::KeyStates>(
			static_cast<int8_t>(states1) & static_cast<int8_t>(states2)
		);
	}

	Keyboard::Keyboard(const Window& window) {
		glfwSetKeyCallback(
			window.getGlfwWindow(),
			[](GLFWwindow* window, int key, int scancode, int action, int mods) {
				if (key == GLFW_KEY_UNKNOWN) {
					return;
				}

				keyStates[key] = static_cast<Keyboard::KeyStates>(action) | Keyboard::KeyStates::Unchecked;
			}
		);
	}

	bool Keyboard::keyPressed(const Key& key) {
		bool isPressed = keyStates[static_cast<int>(key)] == Keyboard::KeyStates::PressedUnchecked;

		if (isPressed) {
			keyStates[static_cast<int>(key)] = Keyboard::KeyStates::PressedChecked;
		}

		return isPressed;
	}

	bool Keyboard::keyPressedContinuously(const Key& key) {
		bool isRepeated = keyStates[static_cast<int>(key)] == Keyboard::KeyStates::RepeatedUnchecked;

		if (isRepeated) {
			keyStates[static_cast<int>(key)] = Keyboard::KeyStates::RepeatedChecked;
			return true;
		}

		bool isPressed = keyStates[static_cast<int>(key)] == Keyboard::KeyStates::PressedUnchecked;

		if (isPressed) {
			keyStates[static_cast<int>(key)] = Keyboard::KeyStates::PressedChecked;
			return true;
		}

		return false;
	}

	bool Keyboard::keyIsDown(const Key& key) {
		const auto& state = keyStates[static_cast<int>(key)];

		return state != Keyboard::KeyStates::ReleasedChecked
			&& state != Keyboard::KeyStates::ReleasedUnchecked;
	}
}
