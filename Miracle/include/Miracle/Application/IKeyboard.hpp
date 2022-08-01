#pragma once

#include <Miracle/Common/Models/KeyboardKey.hpp>
#include <Miracle/Application/Models/Events/KeyInputEvent.hpp>

namespace Miracle::Application {
	class IKeyboard {
	public:
		virtual ~IKeyboard() = default;

		virtual bool isKeyPressed(KeyboardKey key) const = 0;

		virtual bool isKeyPressedOrRepeated(KeyboardKey key) const = 0;

		virtual bool isKeyHeld(KeyboardKey key) const = 0;

		virtual void setAllKeyStatesAsDated() = 0;
	};

	class KeyState {
	private:
		KeyInputAction m_action;
		bool m_updated;

	public:
		inline KeyInputAction getAction() const { return m_action; }

		inline void setAction(KeyInputAction action) {
			m_action = action;
			m_updated = true;
		}

		inline bool isUpdated() const { return m_updated; }

		inline void setAsDated() {
			m_updated = false;
		}
	};
}
