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

	struct KeyInputEvent : public EventBase {
		KeyboardKey key = KeyboardKey::keyUnknown;
		KeyInputAction action = {};
		KeyboardModifierKeys modifiers = {};
	};
}
