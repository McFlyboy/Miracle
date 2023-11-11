#pragma once

#include <functional>
#include <unordered_map>
#include <typeindex>
#include <vector>
#include <utility>

#include "Events/Event.hpp"

namespace Miracle::Application {
	using EventSubscriberId = int;
	using EventCallback = std::function<void(const EventBase&)>;

	struct EventSubscription {
		EventSubscriberId subscriberId;
		EventCallback callback;
	};

	class EventDispatcher {
	private:
		std::unordered_map<std::type_index, std::vector<EventSubscription>> m_subscriptions;
		EventSubscriberId m_nextId = 0;

	public:
		void postEvent(const Event auto& event) const {
			if (!m_subscriptions.contains(typeid(event))) return;

			for (auto& subscription : m_subscriptions.at(typeid(event))) {
				subscription.callback(event);
			}
		}

		EventSubscriberId subscribe(
			std::vector<std::pair<std::type_index, EventCallback>>&& subscribedEvents
		);

		void unsubscribe(EventSubscriberId subscriberId);
	};
}
