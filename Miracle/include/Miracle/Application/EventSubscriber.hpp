#pragma once

#include "EventDispatcher.hpp"
#include "Events/Event.hpp"

namespace Miracle::Application {
	template<Event... t_subscribedEvents>
	class EventSubscriber {
	private:
		EventDispatcher& m_dispatcher;

		const EventSubscriptionId m_subscriptionId;

	public:
		EventSubscriber(EventDispatcher& dispatcher) :
			m_dispatcher(dispatcher),
			m_subscriptionId(
				m_dispatcher.subscribe(
					std::vector<std::type_index>{ typeid(t_subscribedEvents)... },
					std::bind(&EventSubscriber::onEvent, this, std::placeholders::_1)
				)
			)
		{}

		virtual ~EventSubscriber() {
			m_dispatcher.unsubscribe(m_subscriptionId);
		}

		virtual void onEvent(const EventBase& event) = 0;
	};
}
