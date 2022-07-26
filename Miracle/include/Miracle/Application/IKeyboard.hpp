#pragma once

#include <Miracle/Common/Models/KeyboardKey.hpp>

namespace Miracle::Application {
	class IKeyboard {
	public:
		virtual ~IKeyboard() = default;

		virtual bool isKeyPressed(KeyboardKey key) = 0;

		virtual bool isKeyPressedOrRepeated(KeyboardKey key) = 0;

		virtual bool isKeyHeld(KeyboardKey key) const = 0;
	};
}
