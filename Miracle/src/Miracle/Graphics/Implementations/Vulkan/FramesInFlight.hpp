#pragma once

#include "FrameInFlight.hpp"

namespace Miracle::Graphics::Implementations::Vulkan {
	class FramesInFlight {
	private:
		std::array<FrameInFlight, 2> m_framesInFlight;
		int m_targetFrameInFlightIndex;

	public:
		FramesInFlight(const Device& device) :
			m_framesInFlight({
				FrameInFlight(device),
				FrameInFlight(device)
			})
		{}

		inline const FrameInFlight& getTargetFrameInFlight() {
			return m_framesInFlight[m_targetFrameInFlightIndex];
		}

		inline int getTargetFrameInFlightIndex() {
			return m_targetFrameInFlightIndex;
		}

		inline void nextTarget() {
			++m_targetFrameInFlightIndex %= m_framesInFlight.size();
		}
	};
}
