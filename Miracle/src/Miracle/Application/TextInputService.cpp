#include <Miracle/Application/TextInputService.hpp>

#include <utility>

namespace Miracle::Application {
	TextInputService::TextInputService(EventDispatcher& dispatcher) :
		EventSubscriber(dispatcher, [this](auto& event) { handleTextInputEvent(event); })
	{}

	void TextInputService::setTextInputReceiver(
		std::u8string& textInputReceiver,
		TextInputReceivedCallback&& textInputReceivedCallback
	) {
		m_receiver = &textInputReceiver;
		m_callback = std::move(textInputReceivedCallback);
	}

	void TextInputService::unsetTextInputReceiver() {
		m_receiver = nullptr;
		m_callback = []() {};
	}

	void TextInputService::handleTextInputEvent(const TextInputEvent& event) {
		if (m_receiver == nullptr) return;

		if (event.text == u8"\b") [[unlikely]] {
			if (m_receiver->empty()) {
				return;
			}

			auto reverseIterator = m_receiver->rbegin();
			if (*reverseIterator >> 7 == 0) [[likely]] {
				m_receiver->pop_back();
			}
			else if (*(++reverseIterator) >> 5 == 0b110) {
				m_receiver->pop_back();
				m_receiver->pop_back();
			}
			else if (*(++reverseIterator) >> 4 == 0b1110) {
				m_receiver->pop_back();
				m_receiver->pop_back();
				m_receiver->pop_back();
			}
			else if (*(++reverseIterator) >> 3 == 0b11110) {
				m_receiver->pop_back();
				m_receiver->pop_back();
				m_receiver->pop_back();
				m_receiver->pop_back();
			}
			else {
				return;
			}
		}
		else {
			*m_receiver += event.text;
		}

		m_callback();
	}
}
