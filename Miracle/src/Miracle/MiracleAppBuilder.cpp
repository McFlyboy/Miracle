#include <Miracle/components/Miracle/MiracleAppBuilder.hpp>

namespace Miracle {
	MiracleAppBuilder& MiracleAppBuilder::configureWindow(std::function<void(View::WindowProps&)> configurer) {
		configurer(m_windowProps);

		return *this;
	}

	MiracleApp MiracleAppBuilder::build() const {
		return MiracleApp(m_windowProps);
	}
}
