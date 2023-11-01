#pragma once

#include <functional>
#include <memory>
#include <utility>

#include "Components/Behavior.hpp"
#include "EntityContext.hpp"

namespace Miracle {
	class BehaviorFactory {
	private:
		std::function<std::unique_ptr<BehaviorBase>(const EntityContext&)> m_factory;

	public:
		BehaviorFactory(std::function<std::unique_ptr<BehaviorBase>(const EntityContext&)> factory) :
			m_factory(std::move(factory))
		{}

		std::unique_ptr<BehaviorBase> produce(const EntityContext& context) const {
			return m_factory(context);
		}

		template<Behavior TBehavior, typename... TBehaviorArgs>
		static BehaviorFactory createFactoryFor(TBehaviorArgs&&... behaviorArgs) {
			return BehaviorFactory(
				[=](const EntityContext& context) {
					return std::make_unique<TBehavior>(context, behaviorArgs...);
				}
			);
		}
	};
}
