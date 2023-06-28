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

	void Scene::setBackgroundColor(const ColorRgb& color) {
		m_backgroundColor = color;
	}

	EntityId Scene::createEntity(const EntityConfig& config) {
		return m_container->createEntity(config);
	}

	void Scene::destroyEntity(EntityId id) {
		m_container->destroyEntity(id);
	}

	void Scene::forEachEntityTransform(const std::function<void(const Transform&)>& forEach) const {
		m_container->forEachTransform(forEach);
	}

	void Scene::update() {
		m_container->forEachBehaviour(
			[](Behaviour& behaviour) {
				behaviour.act();
			}
		);
	}
}
