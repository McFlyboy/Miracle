#pragma once

#include <Miracle/components/Miracle/View/IWindow.hpp>

namespace Miracle {
	class MIRACLE_API MiracleApp {
	private:
		const View::WindowProps m_windowProps;

	public:
		MiracleApp(const View::WindowProps& windowProps);

		int run() const;
	};
}
