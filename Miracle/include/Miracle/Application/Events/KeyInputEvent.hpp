#pragma once

#include <Miracle/Common/Models/KeyboardKey.hpp>
#include <Miracle/Common/Models/KeyboardModifierKeys.hpp>
#include "Event.hpp"

namespace Miracle::Application {
	enum class KeyInputAction {
		keyReleased = 0,
		keyPressed  = 1,
		keyRepeated = 2
	};

	class KeyInputEvent : public Event {
	private:
		const KeyboardKey m_key;
		const KeyInputAction m_action;
		const KeyboardModifierKeys m_modifiers;

	public:
		KeyInputEvent(
			KeyboardKey key,
			KeyInputAction action,
			KeyboardModifierKeys modifiers
		) :
			Event(EventTypes::keyInput),
			m_key(key),
			m_action(action),
			m_modifiers(modifiers)
		{}

		KeyboardKey getKey() const { return m_key; }

		KeyInputAction getAction() const { return m_action; }

		KeyboardModifierKeys getModifiers() const { return m_modifiers; }
	};
}
