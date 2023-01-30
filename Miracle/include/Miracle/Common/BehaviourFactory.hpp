#pragma once

#include <functional>
#include <memory>

#include "Behaviour.hpp"
#include "Models/EntityProps.hpp"

namespace Miracle {
	using BehaviourFactory = std::function<std::unique_ptr<Behaviour>(const EntityProps&)>;
}
