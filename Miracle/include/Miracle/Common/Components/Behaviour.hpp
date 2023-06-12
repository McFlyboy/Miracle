#pragma once

#include <Miracle/Common/Models/EntityId.hpp>
#include <Miracle/Common/IEcsContainer.hpp>
#include "Transform.hpp"

namespace Miracle {
	struct BehaviourDependencies {
		IEcsContainer& ecsContainer;
		EntityId entityId;
	};

	class Behaviour {
	protected:
		Transform& m_entityTransform;

	public:
		Behaviour(const BehaviourDependencies& dependencies) :
			m_entityTransform(dependencies.ecsContainer.getTransform(dependencies.entityId))
		{}

		virtual ~Behaviour() = default;

		virtual void act() = 0;
	};
}
