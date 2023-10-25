#pragma once

#include <concepts>

namespace Miracle::Application {
	struct EventBase {};

	template<typename T>
	concept Event = std::derived_from<T, EventBase>;
}
