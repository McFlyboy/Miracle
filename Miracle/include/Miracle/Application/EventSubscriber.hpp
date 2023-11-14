#pragma once

#include <concepts>

#include "EventDispatcher.hpp"
#include "Events/Event.hpp"

namespace Miracle::Application {
	template<Event... TEvents>
	class EventSubscriber {
	private:
		EventDispatcher& m_dispatcher;

		const EventSubscriberId m_subscriberId;

	public:
		EventSubscriber(EventDispatcher& dispatcher, const std::invocable<const TEvents&> auto&... callbacks) :
			m_dispatcher(dispatcher),
			m_subscriberId(
				m_dispatcher.subscribe(
					std::vector{
						std::pair<std::type_index, EventCallback>(
							typeid(TEvents),
							[=](const EventBase& event) { callbacks(reinterpret_cast<const TEvents&>(event)); }
						)...
					}
				)
			)
		{}

		virtual ~EventSubscriber() {
			m_dispatcher.unsubscribe(m_subscriberId);
		}
	};
}
