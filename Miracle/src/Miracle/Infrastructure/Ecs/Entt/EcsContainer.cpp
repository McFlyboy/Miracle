#include "EcsContainer.hpp"

#include <memory>
#include <utility>

namespace Miracle::Infrastructure::Ecs::Entt {
	EntityId EcsContainer::createEntity(const EntityConfig& config) {
		auto entity = m_registry.create();

		m_registry.emplace<Transform>(
			entity,
			config.transformConfig.translation,
			config.transformConfig.rotation,
			config.transformConfig.scale
		);

		if (config.behaviourFactory.has_value()) {
			m_registry.emplace<std::unique_ptr<Behaviour>>(
				entity,
				config.behaviourFactory.value()(
					EntityContext(entity, *this)
				)
			);
		}

		m_entityCreatedCallback(entity);
		return entity;
	}

	void EcsContainer::scheduleEntityDestruction(EntityId entity) {
		m_entitiesScheduledForDestruction.emplace(entity);
	}

	void EcsContainer::destroyScheduledEntities() {
		for (auto entity : m_entitiesScheduledForDestruction) {
			m_registry.destroy(entity);
			m_entityDestroyedCallback(entity);
		}

		m_entitiesScheduledForDestruction.clear();
	}

	void EcsContainer::setEntityCreatedCallback(std::function<void(EntityId)>&& entityCreatedCallback) {
		m_entityCreatedCallback = std::move(entityCreatedCallback);
	}

	void EcsContainer::unsetEntityCreatedCallback() {
		m_entityCreatedCallback = [](EntityId) {};
	}

	void EcsContainer::setEntityDestroyedCallback(std::function<void(EntityId)>&& entityDestroyedCallback) {
		m_entityDestroyedCallback = std::move(entityDestroyedCallback);
	}

	void EcsContainer::unsetEntityDestroyedCallback() {
		m_entityDestroyedCallback = [](EntityId) {};
	}

	void EcsContainer::forEachTransform(const std::function<void(const Transform&)>& forEach) const {
		for (auto [entity, transform] : m_registry.view<Transform>().each()) {
			forEach(transform);
		}
	}

	void EcsContainer::forEachBehaviour(const std::function<void(Behaviour&)>& forEach) {
		for (auto [entity, behaviour] : m_registry.view<std::unique_ptr<Behaviour>>().each()) {
			forEach(*behaviour.get());
		}
	}
}
