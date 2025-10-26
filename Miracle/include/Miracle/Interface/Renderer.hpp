#pragma once

#include <Miracle/App.hpp>

namespace Miracle {
	class Renderer {
	public:
		Renderer() = delete;

		static bool isUsingVsync() {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			return App::s_currentApp->m_dependencies->getRenderer().isUsingVsync();
		}

		static void setVsync(bool useVsync) {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			App::s_currentApp->m_dependencies->getRenderer().setVsync(useVsync);
		}

		static bool isUsingTripleBuffering() {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			return App::s_currentApp->m_dependencies->getRenderer().isUsingTripleBuffering();
		}

		static void setTripleBuffering(bool useTripleBuffering) {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			App::s_currentApp->m_dependencies->getRenderer().setTripleBuffering(useTripleBuffering);
		}

		static void setVsyncAndTripleBuffering(bool useVsync, bool useTripleBuffering) {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			App::s_currentApp->m_dependencies->getRenderer()
				.setVsyncAndTripleBuffering(useVsync, useTripleBuffering);
		}

		static bool isUsingDepthTesting() {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			return App::s_currentApp->m_dependencies->getRenderer().isUsingDepthTesting();
		}

		static void setDepthTesting(bool useDepthTesting) {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			App::s_currentApp->m_dependencies->getRenderer().setDepthTesting(useDepthTesting);
		}
	};
}
