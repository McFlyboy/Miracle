#pragma once

#include <functional>
#include <vector>

#include "Events/Event.hpp"

namespace Miracle::Application {
	using EventSubscriptionId = int;
	using EventCallback = std::function<void(const Event&)>;

	struct EventSubscription {
		EventSubscriptionId id;
		EventTypes subscribedTypes;
		EventCallback callback;
	};


	class EventDispatcher {
	private:
		std::vector<EventSubscription> m_subscriptions;
		EventSubscriptionId m_nextId = 0;

	public:
		void postEvent(const Event& event) const;

		EventSubscriptionId subscribe(
			const EventTypes& subscribedTypes,
			EventCallback&& callback
		);

		void unsubscribe(EventSubscriptionId subscriptionId);
	};
}
