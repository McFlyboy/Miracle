#include <Miracle/Application/TextInputService.hpp>

#include <utility>

namespace Miracle::Application {
	TextInputService::TextInputService(EventDispatcher& dispatcher) :
		EventSubscriber(dispatcher)
	{}

	void TextInputService::setTextInputReceiver(
		std::u32string& textInputReceiver,
		TextInputReceivedCallback&& textInputReceivedCallback
	) {
		m_receiver = &textInputReceiver;
		m_callback = std::move(textInputReceivedCallback);
	}

	void TextInputService::unsetTextInputReceiver() {
		m_receiver = nullptr;
		m_callback = []() {};
	}

	void TextInputService::onEvent(const EventBase& event) {
		if (m_receiver == nullptr) return;

		auto& textInputEvent = reinterpret_cast<const TextInputEvent&>(event);

		if (textInputEvent.text == U"\b") {
			if (!m_receiver->empty()) {
				m_receiver->pop_back();
			}
		}
		else {
			*m_receiver += textInputEvent.text;
		}

		m_callback();
	}
}
