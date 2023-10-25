#pragma once

#include <string>
#include <functional>

#include "EventDispatcher.hpp"
#include "EventSubscriber.hpp"
#include "Events/TextInputEvent.hpp"

namespace Miracle::Application {
	using TextInputReceivedCallback = std::function<void()>;

	class TextInputService : public EventSubscriber<TextInputEvent> {
	private:
		std::u32string* m_receiver = nullptr;
		TextInputReceivedCallback m_callback = []() {};

	public:
		TextInputService(EventDispatcher& dispatcher);

		void setTextInputReceiver(
			std::u32string& textInputReceiver,
			TextInputReceivedCallback&& textInputReceivedCallback = []() {}
		);

		void unsetTextInputReceiver();

		virtual void onEvent(const EventBase& event) override;
	};
}
