#pragma once

#include <utility>

#include <Miracle/App.hpp>
#include <Miracle/Application/PerformanceCountingService.hpp>

namespace Miracle {
	using CountersUpdatedCallback = Application::CountersUpdatedCallback;

	class PerformanceCounters {
	public:
		PerformanceCounters() = delete;

		static int getFps() {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			return App::s_currentApp->m_dependencies->getPerformanceCountingService().getFps();
		}

		static int getUps() {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			return App::s_currentApp->m_dependencies->getPerformanceCountingService().getUps();
		}

		static void setCountersUpdatedCallback(CountersUpdatedCallback&& countersUpdatedCallback) {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			App::s_currentApp->m_dependencies->getPerformanceCountingService()
				.setCountersUpdatedCallback(std::move(countersUpdatedCallback));
		}

		static void unsetCountersUpdatedCallback() {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			App::s_currentApp->m_dependencies->getPerformanceCountingService()
				.unsetCountersUpdatedCallback();
		}
	};
}
