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

		m_container->setEntityCreatedCallback(std::function(initProps.entityCreatedCallback));
		m_container->setEntityDestroyedCallback(std::function(initProps.entityDestroyedCallback));
	}

	void Scene::setBackgroundColor(const ColorRgb& color) {
		m_backgroundColor = color;
	}

	void Scene::createEntity(const EntityConfig& config) {
		m_container->createEntity(config);
	}

	EntityContext Scene::createAndGetEntity(const EntityConfig& config) {
		return EntityContext(m_container->createEntity(config), *m_container.get());
	}

	void Scene::scheduleEntityDestruction(EntityId entity) {
		m_container->scheduleEntityDestruction(entity);
	}

	void Scene::destroyScheduledEntities() {
		m_container->destroyScheduledEntities();
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

		destroyScheduledEntities();
	}
}
