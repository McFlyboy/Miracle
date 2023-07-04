#include "Keyboard.hpp"

#include <GLFW/glfw3.h>

#include <Miracle/Definitions.hpp>
#include <Miracle/Application/Events/KeyInputEvent.hpp>
#include <Miracle/Application/Events/TextInputEvent.hpp>
#include <Miracle/Common/UnicodeConverter.hpp>

namespace Miracle::Infrastructure::Input::Glfw {
	Keyboard::Keyboard(
		Application::EventDispatcher& eventDispatcher,
		Application::IMultimediaFramework& multimediaFramework,
		View::Glfw::Window& window
	) :
		EventSubscriber(eventDispatcher),
		m_multimediaFramework(multimediaFramework),
		m_window(window)
	{
		glfwSetKeyCallback(
			*m_window,
			[](GLFWwindow* window, int key, int scanCode, int action, int mods) {
				auto& parentWindow = *reinterpret_cast<View::Glfw::Window*>(glfwGetWindowUserPointer(window));
				parentWindow.getEventDispatcher().postEvent(
					Application::KeyInputEvent(
						static_cast<KeyboardKey>(key),
						static_cast<Application::KeyInputAction>(action),
						static_cast<KeyboardModifierKeys>(mods)
					)
				);
			}
		);

		glfwSetCharCallback(
			*m_window,
			[](GLFWwindow* window, unsigned int codePoint) {
				auto& parentWindow = *reinterpret_cast<View::Glfw::Window*>(glfwGetWindowUserPointer(window));
				parentWindow.getEventDispatcher().postEvent(
					Application::TextInputEvent(codePoint)
				);
			}
		);
	}

	Keyboard::~Keyboard() {
		glfwSetKeyCallback(*m_window, nullptr);
		glfwSetCharCallback(*m_window, nullptr);
	}

	void Keyboard::onEvent(const Application::Event& event) {
		auto& keyInputEvent = reinterpret_cast<const Application::KeyInputEvent&>(event);

		if (keyInputEvent.getKey() == KeyboardKey::keyUnknown) {
			return;
		}

		auto& keyState = m_keyStates[static_cast<size_t>(keyInputEvent.getKey())];

		keyState.setAction(keyInputEvent.getAction());

		if (
			keyInputEvent.getKey() == KeyboardKey::keyBackspace
				&& keyInputEvent.getAction() != Application::KeyInputAction::keyReleased
		) {
			m_window.getEventDispatcher().postEvent(
				Application::TextInputEvent(U'\b')
			);
		}
		else if (
#ifdef MIRACLE_PLATFORM_MACOS
			keyInputEvent.getModifiers() == KeyboardModifierKeys::modSuper
#else
			keyInputEvent.getModifiers() == KeyboardModifierKeys::modControl
#endif
				&& keyInputEvent.getKey() == KeyboardKey::keyV
				&& keyInputEvent.getAction() != Application::KeyInputAction::keyReleased
		) {
			auto clipboardContent = m_multimediaFramework.getClipboardContent();

			if (clipboardContent.has_value()) {
				m_window.getEventDispatcher().postEvent(
					Application::TextInputEvent(UnicodeConverter::toUtf32(clipboardContent.value()))
				);
			}
		}
	}

	bool Keyboard::isKeyPressed(KeyboardKey key) const {
		if (key == KeyboardKey::keyUnknown) {
			return false;
		}

		auto& keyState = m_keyStates[static_cast<size_t>(key)];

		return keyState.isUpdated()
			&& keyState.getAction() == Application::KeyInputAction::keyPressed;
	}

	bool Keyboard::isKeyPressedOrRepeated(KeyboardKey key) const {
		if (key == KeyboardKey::keyUnknown) {
			return false;
		}

		auto& keyState = m_keyStates[static_cast<size_t>(key)];

		return keyState.isUpdated()
			&& keyState.getAction() != Application::KeyInputAction::keyReleased;
	}

	bool Keyboard::isKeyHeld(KeyboardKey key) const {
		if (key == KeyboardKey::keyUnknown) {
			return false;
		}

		auto& keyState = m_keyStates[static_cast<size_t>(key)];

		return keyState.getAction() != Application::KeyInputAction::keyReleased;
	}

	void Keyboard::setAllKeyStatesAsDated() {
		for (auto& keyState : m_keyStates) {
			keyState.setAsDated();
		}
	}
}
