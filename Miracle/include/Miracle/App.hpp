#pragma once

#include <functional>

#include <Miracle/View/IWindow.hpp>
#include <Miracle/Input/Devices/IKeyboard.hpp>

namespace Miracle {
	using Script = std::function<void()>;

	struct AppProps {
		Script startScript = []() {};
		Script updateScript = []() {};
		View::WindowProps windowProps;
	};

	class App {
	private:
		static App* s_currentApp;

		const AppProps m_props;

		View::IWindow* m_window = nullptr;
		Input::Devices::IKeyboard* m_keyboard = nullptr;
		int m_exitCode = 0;

	public:
		App(const AppProps& props);

		App(AppProps&& props = {});

		inline View::IWindow* getWindow() { return m_window; }

		inline Input::Devices::IKeyboard* getKeyboard() { return m_keyboard; }

		int run();

		void close(int exitCode = 0);

		static inline App* getCurrentApp() { return s_currentApp; }

	private:
		void runEngine();
	};
}
