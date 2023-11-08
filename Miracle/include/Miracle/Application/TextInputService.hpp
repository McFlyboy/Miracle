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
		std::u8string* m_receiver = nullptr;
		TextInputReceivedCallback m_callback = []() {};

	public:
		TextInputService(EventDispatcher& dispatcher);

		void setTextInputReceiver(
			std::u8string& textInputReceiver,
			TextInputReceivedCallback&& textInputReceivedCallback = []() {}
		);

		void unsetTextInputReceiver();

	private:
		void handleTextInputEvent(const TextInputEvent& event);
	};
}
