#include "Keyboard.hpp"

#include <GLFW/glfw3.h>

namespace Miracle::Infrastructure::Input::Glfw {
	Keyboard::Keyboard(
		Application::EventDispatcher& eventDispatcher,
		View::Glfw::Window& window
	) :
		EventSubscriber(eventDispatcher),
		m_window(window)
	{
		glfwSetKeyCallback(
			*m_window,
			[](GLFWwindow* window, int key, int scanCode, int action, int mods) {
				auto& parentWindow = *reinterpret_cast<View::Glfw::Window*>(glfwGetWindowUserPointer(window));
				parentWindow.getEventDispatcher().postEvent(
					Application::KeyInputEvent(
						static_cast<KeyboardKey>(key),
						static_cast<Application::KeyInputAction>(action)
					)
				);
			}
		);
	}

	Keyboard::~Keyboard() {
		glfwSetKeyCallback(*m_window, nullptr);
	}

	void Keyboard::onEvent(const Application::Event& event) {
		auto& keyInputEvent = *reinterpret_cast<const Application::KeyInputEvent*>(&event);

		if (keyInputEvent.getKey() == KeyboardKey::keyUnknown) {
			return;
		}

		auto& keyState = m_keyStates[static_cast<size_t>(keyInputEvent.getKey())];

		keyState.setAction(keyInputEvent.getAction());
	}

	bool Keyboard::isKeyPressed(KeyboardKey key) {
		if (key == KeyboardKey::keyUnknown) {
			return false;
		}

		auto& keyState = m_keyStates[static_cast<size_t>(key)];

		return keyState.isUpdated()
			&& keyState.getActionAndSetStateAsDated() == Application::KeyInputAction::keyPressed;
	}

	bool Keyboard::isKeyPressedOrRepeated(KeyboardKey key) {
		if (key == KeyboardKey::keyUnknown) {
			return false;
		}

		auto& keyState = m_keyStates[static_cast<size_t>(key)];

		return keyState.isUpdated()
			&& keyState.getActionAndSetStateAsDated() != Application::KeyInputAction::keyReleased;
	}

	bool Keyboard::isKeyHeld(KeyboardKey key) const {
		if (key == KeyboardKey::keyUnknown) {
			return false;
		}

		auto& keyState = m_keyStates[static_cast<size_t>(key)];

		return keyState.getAction() != Application::KeyInputAction::keyReleased;
	}
}
