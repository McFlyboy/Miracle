#pragma once

#include "IEcsContainer.hpp"
#include "Models/EntityId.hpp"
#include "Components/Transform.hpp"
#include "Components/Appearance.hpp"

namespace Miracle {
	class EntityContext {
	private:
		IEcsContainer& m_ecsContainer;
		const EntityId m_entityId;

	public:
		EntityContext(EntityId entityId, IEcsContainer& ecsContainer) :
			m_ecsContainer(ecsContainer),
			m_entityId(entityId)
		{}

		inline EntityId getEntityId() const { return m_entityId; }

		inline void destroyEntity() { m_ecsContainer.scheduleEntityDestruction(m_entityId); }

		inline Transform& getTransform() { return m_ecsContainer.getTransform(m_entityId); }

		inline const Transform& getTransform() const { return m_ecsContainer.getTransform(m_entityId); }

		inline Appearance& getAppearance() { return m_ecsContainer.getAppearance(m_entityId); }

		inline const Appearance& getAppearance() const { return m_ecsContainer.getAppearance(m_entityId); }
	};
}
