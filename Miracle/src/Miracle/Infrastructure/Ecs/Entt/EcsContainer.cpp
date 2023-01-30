#include "EcsContainer.hpp"

#include <memory>

#include <Miracle/Common/Models/EntityProps.hpp>

namespace Miracle::Infrastructure::Ecs::Entt {
	void EcsContainer::createEntity(const EntityConfig& config) {
		auto entity = m_registry.create();

		m_registry.emplace<Vector2f>(entity, config.position);

		if (config.behaviourFactory.has_value()) {
			m_registry.emplace<std::unique_ptr<Behaviour>>(
				entity,
				config.behaviourFactory.value()(
					EntityProps{
						.position = m_registry.get<Vector2f>(entity)
					}
				)
			);
		}
	}

	void EcsContainer::forEachPosition(const std::function<void(const Vector2f&)>& forEach) const {
		for (auto [entity, position] : m_registry.view<Vector2f>().each()) {
			forEach(position);
		}
	}

	void EcsContainer::forEachBehaviour(const std::function<void(Behaviour&)>& forEach) {
		for (auto [entity, behaviour] : m_registry.view<std::unique_ptr<Behaviour>>().each()) {
			forEach(*behaviour.get());
		}
	}
}
