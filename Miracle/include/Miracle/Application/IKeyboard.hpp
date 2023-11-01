#pragma once

#include <functional>

#include <Miracle/Common/Models/KeyboardKey.hpp>
#include <Miracle/Application/Events/KeyInputEvent.hpp>

namespace Miracle::Application {
	class IKeyboard {
	public:
		virtual ~IKeyboard() = default;

		virtual bool isKeyPressed(KeyboardKey key) const = 0;

		virtual bool isKeyPressedOrRepeated(KeyboardKey key) const = 0;

		virtual bool isKeyHeld(KeyboardKey key) const = 0;

		virtual void setAllKeyStatesAsDated() = 0;

		virtual void setKeyPressedCallback(std::function<void(KeyboardKey)>&& keyPressedCallback) = 0;

		virtual void unsetKeyPressedCallback() = 0;
	};

	class KeyState {
	private:
		KeyInputAction m_action;
		bool m_updated;

	public:
		KeyInputAction getAction() const { return m_action; }

		void setAction(KeyInputAction action) {
			m_action = action;
			m_updated = true;
		}

		bool isUpdated() const { return m_updated; }

		void setAsDated() {
			m_updated = false;
		}
	};
}
