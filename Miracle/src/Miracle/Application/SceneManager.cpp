#include <Miracle/Application/SceneManager.hpp>

namespace Miracle::Application {
	SceneManager::SceneManager(
		IEcs& ecs,
		const SceneInitProps& firstSceneInitProps
	) :
		m_currentScene(ecs, firstSceneInitProps)
	{}
}
