#include <Miracle/Application/EventDispatcher.hpp>

#include <utility>

namespace Miracle::Application {
	EventSubscriptionId EventDispatcher::subscribe(
		std::vector<std::type_index>&& subscribedEvents,
		EventCallback&& callback
	) {
		m_subscriptions.push_back(
			EventSubscription{
				.id              = m_nextId,
				.subscribedEvents = std::move(subscribedEvents),
				.callback        = std::move(callback)
			}
		);

		return m_nextId++;
	}

	void EventDispatcher::unsubscribe(EventSubscriptionId subscriptionId) {
		std::erase_if(m_subscriptions, [=](auto& subscription) {
			return subscription.id == subscriptionId;
		});
	}
}
