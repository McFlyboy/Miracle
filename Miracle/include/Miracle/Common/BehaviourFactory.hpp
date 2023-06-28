#pragma once

#include <functional>
#include <memory>
#include <utility>

#include "Components/Behaviour.hpp"
#include "EntityView.hpp"

namespace Miracle {
	class BehaviourFactory {
	private:
		std::function<std::unique_ptr<Behaviour>(const EntityView&)> m_factory;

	public:
		BehaviourFactory(std::function<std::unique_ptr<Behaviour>(const EntityView&)> factory) :
			m_factory(std::move(factory))
		{}

		std::unique_ptr<Behaviour> operator()(const EntityView& entity) const {
			return m_factory(entity);
		}

		template<typename TBehaviour, typename... TBehaviourArgs>
		static BehaviourFactory createFactoryFor(TBehaviourArgs&&... behaviourArgs) {
			return BehaviourFactory(
				[=](const EntityView& entity) {
					return std::make_unique<TBehaviour>(entity, behaviourArgs...);
				}
			);
		}
	};
}
