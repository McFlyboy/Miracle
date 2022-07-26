#pragma once

#include <Miracle/App.hpp>
#include <Miracle/Common/Models/KeyboardKey.hpp>

namespace Miracle {
	class Keyboard {
	public:
		Keyboard() = delete;

		static inline bool isKeyPressed(KeyboardKey key) {
			if (App::s_currentApp == nullptr) [[unlikely]] return false;

			return App::s_currentApp->m_dependencies->getKeyboard().isKeyPressed(key);
		}

		static inline bool isKeyPressedOrRepeated(KeyboardKey key) {
			if (App::s_currentApp == nullptr) [[unlikely]] return false;

			return App::s_currentApp->m_dependencies->getKeyboard().isKeyPressedOrRepeated(key);
		}

		static inline bool isKeyHeld(KeyboardKey key) {
			if (App::s_currentApp == nullptr) [[unlikely]] return false;

			return App::s_currentApp->m_dependencies->getKeyboard().isKeyHeld(key);
		}
	};
}
