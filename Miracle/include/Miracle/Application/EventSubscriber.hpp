#pragma once

#include "EventDispatcher.hpp"
#include "Events/Event.hpp"

namespace Miracle::Application {
	template<EventTypes t_subscribedTypes>
	class EventSubscriber {
	private:
		EventDispatcher& m_dispatcher;

		const EventSubscriptionId m_subscriptionId;

	public:
		EventSubscriber(EventDispatcher& dispatcher) :
			m_dispatcher(dispatcher),
			m_subscriptionId(
				m_dispatcher.subscribe(
					t_subscribedTypes,
					std::bind(&EventSubscriber::onEvent, this, std::placeholders::_1)
				)
			)
		{}

		virtual ~EventSubscriber() {
			m_dispatcher.unsubscribe(m_subscriptionId);
		}

		virtual void onEvent(const Event& event) = 0;
	};
}
