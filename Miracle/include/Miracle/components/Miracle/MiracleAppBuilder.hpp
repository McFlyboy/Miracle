#pragma once

#include <functional>

#include "MiracleApp.hpp"
#include <Miracle/components/Miracle/View/IWindow.hpp>

namespace Miracle {
	class MIRACLE_API MiracleAppBuilder {
	private:
		View::WindowProps m_windowProps;
		std::function<void()> m_startScript = []() {};
		std::function<void()> m_updateScript = []() {};

	public:
		MiracleAppBuilder& configureWindow(std::function<void(View::WindowProps&)> configurer);
		MiracleAppBuilder& setStartScript(std::function<void()> startScript);
		MiracleAppBuilder& setUpdateScript(std::function<void()> updateScript);
		MiracleApp build() const;
	};
}
