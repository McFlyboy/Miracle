#pragma once

#include "IEcs.hpp"
#include "Models/Scene.hpp"

namespace Miracle::Application {
	class SceneManager {
	private:
		Scene m_currentScene;

	public:
		SceneManager(
			IEcs& ecs,
			const SceneInitProps& firstSceneInitProps
		);

		inline Scene& getCurrentScene() { return m_currentScene; }
	};
}
