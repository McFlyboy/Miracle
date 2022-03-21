#include "EngineDependencies.hpp"

namespace Miracle {
	EngineDependencies::EngineDependencies(const View::WindowProps& windowProps) :
		m_window(windowProps),
		m_keyboard(m_window),
		m_graphicsEngine(m_window, m_resourceLoader)
	{}
}
