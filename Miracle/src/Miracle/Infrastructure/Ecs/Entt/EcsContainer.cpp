#include "EcsContainer.hpp"

#include <utility>

namespace Miracle::Infrastructure::Ecs::Entt {
	EcsContainer::EcsContainer() {
		auto entity = m_registry.create();
		m_registry.emplace<Vector2f>(entity);
	}

	void EcsContainer::setEntityPosition(const Vector2f& position) {
		auto& currentPosition = m_registry.get<Vector2f>(m_registry.data()[0]);
		currentPosition = position;
	}

	void EcsContainer::addEntityBehaviour(std::unique_ptr<Behaviour>&& behaviour) {
		m_registry.emplace<std::unique_ptr<Behaviour>>(
			m_registry.data()[0],
			std::move(behaviour)
		);
	}

	void EcsContainer::forEachEntityBehaviour(const std::function<void(Behaviour&)>& forEach) {
		for (auto [entity, behaviour] : m_registry.view<std::unique_ptr<Behaviour>>().each()) {
			forEach(*behaviour.get());
		}
	}
}
