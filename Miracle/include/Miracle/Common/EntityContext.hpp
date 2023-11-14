#pragma once

#include "IEcsContainer.hpp"
#include "Models/EntityId.hpp"
#include "Components/Transform.hpp"
#include "Components/Camera.hpp"
#include "Components/Appearance.hpp"

namespace Miracle {
	class EntityContext {
	private:
		IEcsContainer& m_ecsContainer;
		const EntityId m_entityId;

	public:
		constexpr EntityContext(EntityId entityId, IEcsContainer& ecsContainer) :
			m_ecsContainer(ecsContainer),
			m_entityId(entityId)
		{}

		constexpr EntityId getEntityId() const { return m_entityId; }

		void destroyEntity() { m_ecsContainer.scheduleEntityDestruction(m_entityId); }

		Transform& getTransform() { return m_ecsContainer.getTransform(m_entityId); }

		const Transform& getTransform() const { return m_ecsContainer.getTransform(m_entityId); }

		Camera& getCamera() { return m_ecsContainer.getCamera(m_entityId); }

		const Camera& getCamera() const { return m_ecsContainer.getCamera(m_entityId); }

		Appearance& getAppearance() { return m_ecsContainer.getAppearance(m_entityId); }

		const Appearance& getAppearance() const { return m_ecsContainer.getAppearance(m_entityId); }
	};
}
