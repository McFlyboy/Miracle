#pragma once

#include "Math/Vector2f.hpp"
#include "Models/EntityId.hpp"
#include "IEcsContainer.hpp"

namespace Miracle {
	struct BehaviourDependencies {
		IEcsContainer& ecsContainer;
		EntityId entityId;
	};

	class Behaviour {
	protected:
		Vector2f& m_entityPosition;

	public:
		Behaviour(const BehaviourDependencies& dependencies) :
			m_entityPosition(dependencies.ecsContainer.getPosition(dependencies.entityId))
		{}

		virtual ~Behaviour() = default;

		virtual void act() = 0;
	};
}
