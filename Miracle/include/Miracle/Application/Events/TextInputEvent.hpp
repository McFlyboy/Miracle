#pragma once

#include <string>

#include "Event.hpp"

namespace Miracle::Application {
	struct TextInputEvent : public EventBase {
		std::u8string text = {};
	};
}
