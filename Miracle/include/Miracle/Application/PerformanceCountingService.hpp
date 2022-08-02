#pragma once

#include <chrono>
#include <functional>

#include "IMultimediaFramework.hpp"

namespace Miracle::Application {
	using CountersUpdatedCallback = std::function<void()>;

	class PerformanceCountingService {
	private:
		IMultimediaFramework& m_multimediaFramework;

		std::chrono::seconds m_previousCounterUpdate = std::chrono::seconds(0);
		int m_ups = 0;
		int m_updateCounter = 0;
		CountersUpdatedCallback m_callback = []() {};

	public:
		PerformanceCountingService(IMultimediaFramework& multimediaFramework);

		inline int getUps() const { return m_ups; }

		void incrementUpdateCounter();

		void updateCounters();

		void setCountersUpdatedCallback(CountersUpdatedCallback&& countersUpdatedCallback);

		void unsetCountersUpdatedCallback();
	};
}
