#pragma once

#include <concepts>

#include <Miracle/Common/EntityContext.hpp>
#include <Miracle/Common/Models/EntityId.hpp>
#include "Transform.hpp"

namespace Miracle {
	class BehaviorBase {
	protected:
		EntityContext m_context;

	public:
		constexpr BehaviorBase(const EntityContext& context) :
			m_context(context)
		{}

		virtual ~BehaviorBase() = default;

		virtual void act() = 0;
	};

	template<typename T>
	concept Behavior = std::derived_from<T, BehaviorBase>;
}
