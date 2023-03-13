#include "EcsContainer.hpp"

#include <memory>

#include <Miracle/Common/Models/EntityId.hpp>

namespace Miracle::Infrastructure::Ecs::Entt {
	void EcsContainer::createEntity(const EntityConfig& config) {
		auto entity = m_registry.create();

		m_registry.emplace<Matrix4>(entity, Matrix4::createTranslation(config.position));

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

	Matrix4& EcsContainer::getTransform(EntityId owner) {
		return m_registry.get<Matrix4>(static_cast<entt::entity>(owner));
	}

	void EcsContainer::forEachTransform(const std::function<void(const Matrix4&)>& forEach) const {
		for (auto [entity, position] : m_registry.view<Matrix4>().each()) {
			forEach(position);
		}
	}

	void EcsContainer::forEachBehaviour(const std::function<void(Behaviour&)>& forEach) {
		for (auto [entity, behaviour] : m_registry.view<std::unique_ptr<Behaviour>>().each()) {
			forEach(*behaviour.get());
		}
	}
}
