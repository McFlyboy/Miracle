#pragma once

#include <Miracle/components/Miracle/Input/Devices/IKeyboard.hpp>

#include <Miracle/View/Implementations/Window.hpp>

using namespace Miracle::View::Implementations;

namespace Miracle::Input::Devices::Implementations {
	class Keyboard : public IKeyboard {
	public:
		Keyboard(const Window& window);
	};
}
