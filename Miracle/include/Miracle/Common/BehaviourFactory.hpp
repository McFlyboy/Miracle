#pragma once

#include <functional>
#include <memory>

#include "Behaviour.hpp"

namespace Miracle {
	using BehaviourFactory = std::function<std::unique_ptr<Behaviour>()>;
}
