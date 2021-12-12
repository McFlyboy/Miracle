#pragma once

#include <functional>

#include <Miracle/components/Miracle/View/IWindow.hpp>

namespace Miracle {
	class MIRACLE_API MiracleApp {
	private:
		const View::WindowProps m_windowProps;
		std::function<void()> m_startScript;
		std::function<void()> m_updateScript;

	public:
		MiracleApp(
			const View::WindowProps& windowProps,
			const std::function<void()>& startScript,
			const std::function<void()>& updateScript
		);

		int run() const;
	};
}
