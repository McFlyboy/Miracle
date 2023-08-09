#include "EcsContainer.hpp"

#include <memory>
#include <utility>

#include <Miracle/Common/EntityContext.hpp>
#include <Miracle/Common/Components/Transform.hpp>
#include <Miracle/Common/Components/Appearance.hpp>
#include <Miracle/Common/Components/Behaviour.hpp>

namespace Miracle::Infrastructure::Ecs::Entt {
	EntityId EcsContainer::createEntity(const EntityConfig& config) {
		auto entity = m_registry.create();

		m_registry.emplace<Transform>(
			entity,
			config.transformConfig.translation,
			config.transformConfig.rotation,
			config.transformConfig.scale
		);

		if (config.appearanceConfig.has_value()) {
			auto& appearanceConfig = config.appearanceConfig.value();

			m_registry.emplace<Appearance>(
				entity,
				appearanceConfig.visible,
				appearanceConfig.meshIndex,
				appearanceConfig.color
			);
		}
		
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

	void EcsContainer::forEachAppearance(
		const std::function<void(const Transform&, const Appearance&)>& forEach
	) const {
		for (auto [entity, transform, appearance] : m_registry.view<Transform, Appearance>().each()) {
			forEach(transform, appearance);
		}
	}

	void EcsContainer::forEachBehaviour(const std::function<void(Behaviour&)>& forEach) {
		for (auto [entity, behaviour] : m_registry.view<std::unique_ptr<Behaviour>>().each()) {
			forEach(*behaviour.get());
		}
	}
}
