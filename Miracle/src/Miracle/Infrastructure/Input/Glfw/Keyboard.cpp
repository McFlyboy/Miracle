#include "Keyboard.hpp"

#include <GLFW/glfw3.h>

#include <Miracle/Environment.hpp>
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
					Application::KeyInputEvent{
						.key       = static_cast<KeyboardKey>(key),
						.action    = static_cast<Application::KeyInputAction>(action),
						.modifiers = static_cast<KeyboardModifierKeys>(mods)
					}
				);
			}
		);

		glfwSetCharCallback(
			*m_window,
			[](GLFWwindow* window, unsigned int codePoint) {
				auto& parentWindow = *reinterpret_cast<View::Glfw::Window*>(glfwGetWindowUserPointer(window));
				parentWindow.getEventDispatcher().postEvent(
					Application::TextInputEvent{ .text = UnicodeConverter::toUtf8(static_cast<uint32_t>(codePoint)) }
				);
			}
		);
	}

	Keyboard::~Keyboard() {
		glfwSetKeyCallback(*m_window, nullptr);
		glfwSetCharCallback(*m_window, nullptr);
	}

	void Keyboard::onEvent(const Application::EventBase& event) {
		auto& keyInputEvent = reinterpret_cast<const Application::KeyInputEvent&>(event);

		if (keyInputEvent.key == KeyboardKey::keyUnknown) {
			return;
		}

		auto& keyState = m_keyStates[static_cast<size_t>(keyInputEvent.key)];

		keyState.setAction(keyInputEvent.action);

		if (
			keyInputEvent.key == KeyboardKey::keyBackspace
				&& keyInputEvent.action != Application::KeyInputAction::keyReleased
		) {
			m_window.getEventDispatcher().postEvent(
				Application::TextInputEvent{ .text = std::u8string(1, u8'\b') }
			);
		}
		else {
			constexpr KeyboardModifierKeys pasteModifierKey = Environment::getCurrentPlatform() == Platform::macos
				? KeyboardModifierKeys::modSuper
				: KeyboardModifierKeys::modControl;

			if (
				keyInputEvent.modifiers == pasteModifierKey
					&& keyInputEvent.key == KeyboardKey::keyV
					&& keyInputEvent.action != Application::KeyInputAction::keyReleased
			) {
				auto clipboardContent = m_multimediaFramework.getClipboardContent();

				if (clipboardContent.has_value()) {
					auto& content = clipboardContent.value();

					m_window.getEventDispatcher().postEvent(
						Application::TextInputEvent{ .text = std::u8string(content.data(), content.size()) }
					);
				}
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
