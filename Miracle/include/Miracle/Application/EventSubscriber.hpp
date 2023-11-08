#pragma once

#include <functional>

#include "EventDispatcher.hpp"
#include "Events/Event.hpp"
#include "Events/TextInputEvent.hpp"

namespace Miracle::Application {
	template<Event... TEvents>
	class EventSubscriber {
	private:
		EventDispatcher& m_dispatcher;

		const EventSubscriberId m_subscriberId;

	public:
		EventSubscriber(EventDispatcher& dispatcher, std::function<void(const TEvents&)>... callbacks) :
			m_dispatcher(dispatcher),
			m_subscriberId(
				m_dispatcher.subscribe(
					std::vector{
						std::pair(
							std::type_index(typeid(TEvents)),
							EventCallback([=](const EventBase& event) { callbacks(reinterpret_cast<const TEvents&>(event)); })
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
