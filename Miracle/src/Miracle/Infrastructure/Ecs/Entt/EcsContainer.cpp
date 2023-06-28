#include "EcsContainer.hpp"

#include <memory>

namespace Miracle::Infrastructure::Ecs::Entt {
	EntityId EcsContainer::createEntity(const EntityConfig& config) {
		auto entity = m_registry.create();

		auto& transform = m_registry.emplace<Transform>(
			entity,
			config.transformConfig.translation,
			config.transformConfig.rotation,
			config.transformConfig.scale
		);

		if (config.behaviourFactory.has_value()) {
			m_registry.emplace<std::unique_ptr<Behaviour>>(
				entity,
				config.behaviourFactory.value()(
					EntityView(*this, entity)
				)
			);
		}

		return entity;
	}

	void EcsContainer::destroyEntity(EntityId id) {
		m_registry.destroy(id);
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
