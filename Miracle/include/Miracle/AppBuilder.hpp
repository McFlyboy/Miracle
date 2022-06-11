#pragma once

#include <functional>

#include <Miracle/View/IWindow.hpp>
#include "App.hpp"

namespace Miracle {
	class AppBuilder {
	private:
		AppProps m_props;

	public:
		AppBuilder& configureWindow(std::function<void (View::WindowProps&)> configurer) {
			configurer(m_props.windowProps);
			return *this;
		}

		AppBuilder& setStartScript(std::function<void ()> startScript) {
			m_props.startScript = startScript;
			return *this;
		}

		AppBuilder& setUpdateScript(std::function<void ()> updateScript) {
			m_props.updateScript = updateScript;
			return *this;
		}

		App build() const {
			return App(m_props);
		}
	};
}
