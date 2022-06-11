#pragma once

#include <functional>
#include <utility>

#include <Miracle/View/IWindow.hpp>
#include "App.hpp"

namespace Miracle {
	class AppBuilder {
	private:
		AppProps m_props;

	public:
		AppBuilder& setStartScript(std::function<void()>&& startScript) {
			m_props.startScript = std::move(startScript);
			return *this;
		}

		AppBuilder& setUpdateScript(std::function<void()>&& updateScript) {
			m_props.updateScript = std::move(updateScript);
			return *this;
		}

		AppBuilder& configureWindow(const std::function<void(View::WindowProps*)>& configurer) {
			configurer(&m_props.windowProps);
			return *this;
		}

		App build() const {
			return App(m_props);
		}
	};
}
