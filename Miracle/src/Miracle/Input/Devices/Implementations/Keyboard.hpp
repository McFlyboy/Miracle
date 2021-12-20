#pragma once

#include <array>

#include <Miracle/components/Miracle/Input/Devices/IKeyboard.hpp>

#include <Miracle/View/Implementations/Window.hpp>

using namespace Miracle::View::Implementations;

namespace Miracle::Input::Devices::Implementations {
	class Keyboard : public IKeyboard {
	private:
		std::array<KeyState, 350> m_keyStates;

	public:
		Keyboard(const Window& window);
	};
}
