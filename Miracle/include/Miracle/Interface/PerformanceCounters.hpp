#pragma once

#include <utility>

#include <Miracle/App.hpp>
#include <Miracle/Application/PerformanceCountingService.hpp>

namespace Miracle {
	using CountersUpdatedCallback = Application::CountersUpdatedCallback;

	class PerformanceCounters {
	public:
		PerformanceCounters() = delete;

		static inline int getUps() {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			return App::s_currentApp->m_dependencies->getPerformanceCountingService().getUps();
		}

		static inline void setCountersUpdatedCallback(CountersUpdatedCallback&& countersUpdatedCallback) {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			App::s_currentApp->m_dependencies->getPerformanceCountingService()
				.setCountersUpdatedCallback(std::move(countersUpdatedCallback));
		}

		static inline void unsetCountersUpdatedCallback() {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			App::s_currentApp->m_dependencies->getPerformanceCountingService()
				.unsetCountersUpdatedCallback();
		}
	};
}
