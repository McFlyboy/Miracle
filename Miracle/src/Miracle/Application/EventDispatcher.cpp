#include <Miracle/Application/EventDispatcher.hpp>

#include <utility>

namespace Miracle::Application {
	void EventDispatcher::postEvent(const Event& event) const {
		for (auto& subscription : m_subscriptions) {
			if (EventTypesUtilities::hasAnyOfSameTypes(subscription.subscribedTypes, event.getTypes())) {
				subscription.callback(event);
			}
		}
	}

	EventSubscriptionId EventDispatcher::subscribe(
		const EventTypes& subscribedTypes,
		EventCallback&& callback
	) {
		m_subscriptions.emplace_back(m_nextId, subscribedTypes, std::move(callback));
		return m_nextId++;
	}

	void EventDispatcher::unsubscribe(EventSubscriptionId subscriptionId) {
		std::erase_if(m_subscriptions, [=](auto& subscription) {
			return subscription.id == subscriptionId;
		});
	}
}
