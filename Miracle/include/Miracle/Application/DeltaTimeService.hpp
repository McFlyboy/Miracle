#pragma once

#include <chrono>

#include "IMultimediaFramework.hpp"

namespace Miracle::Application {
	class DeltaTimeService {
	private:
		IMultimediaFramework& m_multimediaFramework;

		std::chrono::duration<float> m_deltaTime = std::chrono::duration<float>(0.0f);
		std::chrono::duration<float> m_previousTime = std::chrono::duration<float>(0.0f);

	public:
		DeltaTimeService(IMultimediaFramework& multimediaFramework);

		std::chrono::duration<float> getDeltaTime() const { return m_deltaTime; }

		void updateDeltaTime();
	};
}
