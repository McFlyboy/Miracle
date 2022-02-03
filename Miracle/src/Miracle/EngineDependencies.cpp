#include "EngineDependencies.hpp"

using namespace Miracle::View::Implementations;
using namespace Miracle::Input::Devices::Implementations;

namespace Miracle {
	EngineDependencies::EngineDependencies(const View::WindowProps& windowProps) :
		m_window(Window(windowProps)),
		m_keyboard(Keyboard(m_window))
	{}
}
