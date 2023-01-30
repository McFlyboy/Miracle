#include <Miracle/Application/Models/Scene.hpp>

namespace Miracle::Application {
	Scene::Scene(
		IEcs& ecs,
		const SceneInitProps& initProps
	) :
		m_container(ecs.createContainer()),
		m_backgroundColor(initProps.backgroundColor)
	{
		for (auto& entityConfig : initProps.entityConfigs) {
			m_container->createEntity(entityConfig);
		}
	}

	void Scene::setBackgroundColor(const Color3f& color) {
		m_backgroundColor = color;
	}

	void Scene::update() {
		m_container->forEachBehaviour(
			[](Behaviour& behaviour) {
				behaviour.act();
			}
		);
	}

	void Scene::forEachPosition(const std::function<void(const Vector2f&)>& forEach) const {
		m_container->forEachPosition(forEach);
	}

	void Scene::addEntity(const EntityConfig& config) {
		m_container->createEntity(config);
	}
}