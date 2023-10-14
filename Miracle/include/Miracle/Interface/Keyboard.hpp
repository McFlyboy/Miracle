#pragma once

#include <Miracle/App.hpp>
#include <Miracle/Common/Models/KeyboardKey.hpp>

namespace Miracle {
	class Keyboard {
	public:
		Keyboard() = delete;

		static bool isKeyPressed(KeyboardKey key) {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			return App::s_currentApp->m_dependencies->getKeyboard().isKeyPressed(key);
		}

		static bool isKeyPressedOrRepeated(KeyboardKey key) {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			return App::s_currentApp->m_dependencies->getKeyboard().isKeyPressedOrRepeated(key);
		}

		static bool isKeyHeld(KeyboardKey key) {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			return App::s_currentApp->m_dependencies->getKeyboard().isKeyHeld(key);
		}
	};
}
