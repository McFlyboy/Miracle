#pragma once

#include <Miracle/Common/EntityContext.hpp>
#include <Miracle/Common/Models/EntityId.hpp>
#include "Transform.hpp"

namespace Miracle {
	class Behavior {
	protected:
		EntityContext m_context;

	public:
		constexpr Behavior(const EntityContext& context) :
			m_context(context)
		{}

		virtual ~Behavior() = default;

		virtual void act() = 0;
	};
}
