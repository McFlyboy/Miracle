#pragma once

#include <Miracle/Common/EntityContext.hpp>
#include <Miracle/Common/Models/EntityId.hpp>
#include "Transform.hpp"

namespace Miracle {
	class Behaviour {
	protected:
		EntityContext m_context;

	public:
		Behaviour(const EntityContext& context) :
			m_context(context)
		{}

		virtual ~Behaviour() = default;

		virtual void act() = 0;
	};
}
