#pragma once

#include <Miracle/App.hpp>

namespace Miracle {
	class DeltaTime {
	public:
		DeltaTime() = delete;

		static float get() {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			return App::s_currentApp->m_dependencies->getDeltaTimeService()
				.getDeltaTime()
				.count();
		}
	};
}
