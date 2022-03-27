#include <Miracle/components/Miracle/MiracleAppBuilder.hpp>

namespace Miracle {
	MiracleAppBuilder& MiracleAppBuilder::configureWindow(std::function<void (View::WindowProps&)> configurer) {
		configurer(m_windowProps);

		return *this;
	}

	MiracleAppBuilder& MiracleAppBuilder::setStartScript(std::function<void ()> startScript) {
		m_startScript = startScript;

		return *this;
	}

	MiracleAppBuilder& MiracleAppBuilder::setUpdateScript(std::function<void ()> updateScript) {
		m_updateScript = updateScript;

		return *this;
	}

	MiracleApp MiracleAppBuilder::build() const {
		return MiracleApp(
			m_windowProps,
			m_startScript,
			m_updateScript
		);
	}
}
