#pragma once

#include <Miracle/Common/EntityView.hpp>
#include <Miracle/Common/Models/EntityId.hpp>
#include "Transform.hpp"

namespace Miracle {
	class Behaviour {
	protected:
		EntityView m_entity;

	public:
		Behaviour(const EntityView& entity) :
			m_entity(entity)
		{}

		virtual ~Behaviour() = default;

		virtual void act() = 0;
	};
}
