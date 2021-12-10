#pragma once

#include <functional>

#include "MiracleApp.hpp"
#include <Miracle/components/Miracle/View/IWindow.hpp>

namespace Miracle {
	class MIRACLE_API MiracleAppBuilder {
	private:
		View::WindowProps m_windowProps;

	public:
		MiracleAppBuilder& configureWindow(std::function<void(View::WindowProps&)> configurer);

		MiracleApp build() const;
	};
}
