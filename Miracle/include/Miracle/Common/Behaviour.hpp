#pragma once

#include "Models/EntityProps.hpp"

namespace Miracle {
	class Behaviour {
	protected:
		EntityProps m_entityProps;

	public:
		Behaviour(const EntityProps& entityProps) :
			m_entityProps(entityProps)
		{}

		virtual ~Behaviour() = default;

		virtual void act() = 0;
	};
}
