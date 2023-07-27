#pragma once

#include <functional>
#include <memory>
#include <utility>

#include "Components/Behaviour.hpp"
#include "EntityContext.hpp"

namespace Miracle {
	class BehaviourFactory {
	private:
		std::function<std::unique_ptr<Behaviour>(const EntityContext&)> m_factory;

	public:
		BehaviourFactory(std::function<std::unique_ptr<Behaviour>(const EntityContext&)> factory) :
			m_factory(std::move(factory))
		{}

		std::unique_ptr<Behaviour> operator()(const EntityContext& context) const {
			return m_factory(context);
		}

		template<typename TBehaviour, typename... TBehaviourArgs>
		static BehaviourFactory createFactoryFor(TBehaviourArgs&&... behaviourArgs) {
			return BehaviourFactory(
				[=](const EntityContext& context) {
					return std::make_unique<TBehaviour>(context, behaviourArgs...);
				}
			);
		}
	};
}
