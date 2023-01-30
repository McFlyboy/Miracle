#pragma once

#include <functional>

#include <Miracle/Common/Models/EntityConfig.hpp>
#include <Miracle/Common/Math/Vector2f.hpp>
#include <Miracle/Common/Behaviour.hpp>

namespace Miracle::Application {
	class IEcsContainer {
	public:
		virtual ~IEcsContainer() = default;

		virtual void createEntity(const EntityConfig& config) = 0;

		virtual void forEachPosition(const std::function<void(const Vector2f&)>& forEach) const = 0;

		virtual void forEachBehaviour(const std::function<void(Behaviour&)>& forEach) = 0;
	};
}
