#pragma once

#include <functional>

#include <Miracle/components/Miracle/View/IWindow.hpp>
#include <Miracle/components/Miracle/Input/Devices/IKeyboard.hpp>

namespace Miracle {
	class MIRACLE_API MiracleApp {
	private:
		static MiracleApp* s_currentApp;

		const View::WindowProps m_windowProps;
		const std::function<void()> m_startScript;
		const std::function<void()> m_updateScript;
		View::IWindow* m_window = nullptr;
		Input::Devices::IKeyboard* m_keyboard = nullptr;

	public:
		MiracleApp(
			const View::WindowProps& windowProps,
			const std::function<void()>& startScript,
			const std::function<void()>& updateScript
		);

		inline View::IWindow* getWindow() { return m_window; }
		inline Input::Devices::IKeyboard* getKeyboard() { return m_keyboard; }

		int run();

		static MiracleApp* getCurrentApp() { return s_currentApp; }
	};
}
