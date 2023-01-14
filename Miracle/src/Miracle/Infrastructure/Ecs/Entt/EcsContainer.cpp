#include "EcsContainer.hpp"

namespace Miracle::Infrastructure::Ecs::Entt {
	EcsContainer::EcsContainer() {
		auto entity = m_registry.create();
		m_registry.emplace<Vector2f>(entity, Vector2f{});
	}

	void EcsContainer::setEntityPosition(const Vector2f& position) {
		auto& currentPosition = m_registry.get<Vector2f>(m_registry.data()[0]);
		currentPosition = position;
	}
}
