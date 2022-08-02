#pragma once

#include <string>
#include <functional>

#include "EventDispatcher.hpp"
#include "EventSubscriber.hpp"

namespace Miracle::Application {
	using TextInputReceivedCallback = std::function<void()>;

	class TextInputService : public EventSubscriber<EventTypes::textInput> {
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

		virtual void onEvent(const Event& event) override;
	};
}
