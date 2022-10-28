#pragma once

#include <cstdint>

namespace Miracle {
	enum class KeyboardModifierKeys : uint8_t {
		modShift    = 0x01,
		modControl  = 0x02,
		modAlt      = 0x04,
		modSuper    = 0x08,
		modCapsLock = 0x10,
		modNumLock  = 0x20
	};
}
