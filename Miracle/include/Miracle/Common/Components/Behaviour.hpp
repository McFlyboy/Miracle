#pragma once

#include <Miracle/Common/Models/EntityId.hpp>
#include "Transform.hpp"

namespace Miracle {
	struct BehaviourDependencies {
		EntityId entityId;
	};

	class Behaviour {
	protected:
		const EntityId m_entityId;

	public:
		Behaviour(const BehaviourDependencies& dependencies) :
			m_entityId(dependencies.entityId)
		{}

		virtual ~Behaviour() = default;

		virtual void act() = 0;
	};
}
