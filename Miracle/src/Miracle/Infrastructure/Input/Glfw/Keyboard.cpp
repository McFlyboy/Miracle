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
		EventSubscriber(eventDispatcher, [this](auto& event) { handleKeyInputEvent(event); }),
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

	void Keyboard::handleKeyInputEvent(const Application::KeyInputEvent& event) {
		if (event.key == KeyboardKey::keyUnknown) [[unlikely]] return;

		auto& keyState = m_keyStates[static_cast<size_t>(event.key)];

		keyState.setAction(event.action);

		if (event.action == Application::KeyInputAction::keyPressed) {
			m_keyPressedCallback(event.key);
		}

		if (
			event.key == KeyboardKey::keyBackspace
				&& event.action != Application::KeyInputAction::keyReleased
		) [[unlikely]] {
			m_window.getEventDispatcher().postEvent(
				Application::TextInputEvent{ .text = std::u8string(1, u8'\b') }
			);

			return;
		}

		constexpr KeyboardModifierKeys pasteModifierKey = Environment::getCurrentPlatform() == Platform::platformMacos
			? KeyboardModifierKeys::modSuper
			: KeyboardModifierKeys::modControl;

		if (
			event.modifiers == pasteModifierKey
				&& event.key == KeyboardKey::keyV
				&& event.action != Application::KeyInputAction::keyReleased
		) [[unlikely]] {
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
