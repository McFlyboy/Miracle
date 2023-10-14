#pragma once

#include <string>
#include <utility>

#include "Event.hpp"

namespace Miracle::Application {
	class TextInputEvent : public Event {
	private:
		std::u32string m_text;

	public:
		TextInputEvent(std::u32string&& text) :
			Event(EventTypes::textInput),
			m_text(std::move(text))
		{}

		TextInputEvent(char32_t character) :
			Event(EventTypes::textInput),
			m_text(1, character)
		{}

		std::u32string getText() const { return m_text; }
	};
}
