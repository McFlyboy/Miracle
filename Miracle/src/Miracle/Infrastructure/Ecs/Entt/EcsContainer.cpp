#include "EcsContainer.hpp"

#include <memory>

#include <Miracle/Common/Models/EntityId.hpp>

namespace Miracle::Infrastructure::Ecs::Entt {
	void EcsContainer::createEntity(const EntityConfig& config) {
		auto entity = m_registry.create();

		m_registry.emplace<Vector3>(entity, config.position);

		if (config.behaviourFactory.has_value()) {
			m_registry.emplace<std::unique_ptr<Behaviour>>(
				entity,
				config.behaviourFactory.value()(
					BehaviourDependencies{
						.ecsContainer = *this,
						.entityId = static_cast<EntityId>(entity)
					}
				)
			);
		}
	}

	Vector3& EcsContainer::getPosition(EntityId owner) {
		return m_registry.get<Vector3>(static_cast<entt::entity>(owner));
	}

	void EcsContainer::forEachPosition(const std::function<void(const Vector3&)>& forEach) const {
		for (auto [entity, position] : m_registry.view<Vector3>().each()) {
			forEach(position);
		}
	}

	void EcsContainer::forEachBehaviour(const std::function<void(Behaviour&)>& forEach) {
		for (auto [entity, behaviour] : m_registry.view<std::unique_ptr<Behaviour>>().each()) {
			forEach(*behaviour.get());
		}
	}
}
