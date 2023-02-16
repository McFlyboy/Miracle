#pragma once

#include <functional>
#include <memory>
#include <utility>

#include "Behaviour.hpp"

namespace Miracle {
	class BehaviourFactory {
	private:
		std::function<std::unique_ptr<Behaviour>(const BehaviourDependencies&)> m_factory;

	public:
		BehaviourFactory(std::function<std::unique_ptr<Behaviour>(const BehaviourDependencies&)> factory) :
			m_factory(std::move(factory))
		{}

		std::unique_ptr<Behaviour> operator()(const BehaviourDependencies& dependencies) const {
			return m_factory(dependencies);
		}

		template<typename TBehaviour, typename... TBehaviourArgs>
		static BehaviourFactory createFactoryFor(TBehaviourArgs&&... behaviourArgs) {
			return BehaviourFactory(
				[=](const BehaviourDependencies& dependencies) {
					return std::make_unique<TBehaviour>(dependencies, behaviourArgs...);
				}
			);
		}
	};
}
