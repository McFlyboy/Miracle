#include <Miracle/Application/PerformanceCountingService.hpp>

#include <utility>

namespace Miracle::Application {
	PerformanceCountingService::PerformanceCountingService(IMultimediaFramework& multimediaFramework) :
		m_multimediaFramework(multimediaFramework)
	{}

	void PerformanceCountingService::incrementUpdateCounter() {
		m_updateCounter++;
	}

	void PerformanceCountingService::updateCounters() {
		auto currentTime = std::chrono::duration_cast<std::chrono::seconds>(
			m_multimediaFramework.getDurationSinceInitialization()
		);

		if (currentTime == m_previousCounterUpdate) {
			return;
		}

		m_previousCounterUpdate++;
		m_ups = std::exchange(m_updateCounter, 0);
		m_callback();
	}

	void PerformanceCountingService::setCountersUpdatedCallback(
		CountersUpdatedCallback&& countersUpdatedCallback
	) {
		m_callback = std::move(countersUpdatedCallback);
	}

	void PerformanceCountingService::unsetCountersUpdatedCallback() {
		m_callback = []() {};
	}
}
