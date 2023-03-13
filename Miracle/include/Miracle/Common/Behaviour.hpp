#pragma once

#include "Math/Matrix4.hpp"
#include "Models/EntityId.hpp"
#include "IEcsContainer.hpp"

namespace Miracle {
	struct BehaviourDependencies {
		IEcsContainer& ecsContainer;
		EntityId entityId;
	};

	class Behaviour {
	protected:
		Matrix4& m_entityTransform;

	public:
		Behaviour(const BehaviourDependencies& dependencies) :
			m_entityTransform(dependencies.ecsContainer.getTransform(dependencies.entityId))
		{}

		virtual ~Behaviour() = default;

		virtual void act() = 0;
	};
}
