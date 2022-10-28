#include <Miracle/Application/DeltaTimeService.hpp>

#include <utility>

namespace Miracle::Application {
	DeltaTimeService::DeltaTimeService(IMultimediaFramework& multimediaFramework) :
		m_multimediaFramework(multimediaFramework)
	{}

	void DeltaTimeService::updateDeltaTime() {
		auto currentTime = std::chrono::duration_cast<std::chrono::duration<float>>(
			m_multimediaFramework.getDurationSinceInitialization()
		);

		m_deltaTime = currentTime - std::exchange(m_previousTime, currentTime);
	}
}
