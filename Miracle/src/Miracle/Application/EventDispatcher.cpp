#include <Miracle/Application/EventDispatcher.hpp>

namespace Miracle::Application {
	EventSubscriberId EventDispatcher::subscribe(
		std::vector<std::pair<std::type_index, EventCallback>>&& subscribedEvents
	) {
		for (auto& [eventId, callback] : subscribedEvents) {
			m_subscriptions[eventId].emplace_back(m_nextId, std::move(callback));
		}

		return m_nextId++;
	}

	void EventDispatcher::unsubscribe(EventSubscriberId subscriberId) {
		for (auto& [_, eventSubscriptions] : m_subscriptions) {
			std::erase_if(eventSubscriptions, [=](auto& subscription) {
				return subscription.subscriberId == subscriberId;
			});
		}
	}
}
