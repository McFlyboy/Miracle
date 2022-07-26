#pragma once

#include <Miracle/Common/Models/KeyboardKey.hpp>
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

	public:
		KeyInputEvent(KeyboardKey key, KeyInputAction action) :
			Event(EventTypes::keyInput),
			m_key(key),
			m_action(action)
		{}

		inline KeyboardKey getKey() const { return m_key; }

		inline KeyInputAction getAction() const { return m_action; }
	};
}
