#pragma once

#include <functional>
#include <vector>
#include <typeindex>

#include "Events/Event.hpp"

namespace Miracle::Application {
	using EventSubscriptionId = int;
	using EventCallback = std::function<void(const EventBase&)>;

	struct EventSubscription {
		EventSubscriptionId id;
		std::vector<std::type_index> subscribedEvents;
		EventCallback callback;
	};

	class EventDispatcher {
	private:
		std::vector<EventSubscription> m_subscriptions;
		EventSubscriptionId m_nextId = 0;

	public:
		void postEvent(const Event auto& event) const {
			for (auto& subscription : m_subscriptions) {
				for (auto& subscribedEvent : subscription.subscribedEvents) {
					if (subscribedEvent == std::type_index(typeid(event))) {
						subscription.callback(event);
					}
				}
			}
		}

		EventSubscriptionId subscribe(
			std::vector<std::type_index>&& subscribedEvents,
			EventCallback&& callback
		);

		void unsubscribe(EventSubscriptionId subscriptionId);
	};
}
