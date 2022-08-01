#pragma once

#include <chrono>

#include "IMultimediaFramework.hpp"

namespace Miracle::Application {
	class DeltaTimeService {
	private:
		IMultimediaFramework& m_multimediaFramework;

		std::chrono::duration<float> m_deltaTime;
		std::chrono::duration<float> m_previousTime;

	public:
		DeltaTimeService(IMultimediaFramework& multimediaFramework);

		inline std::chrono::duration<float> getDeltaTime() const { return m_deltaTime; }

		void updateDeltaTime();
	};
}
