#pragma once

#include <functional>

#include <Miracle/Common/IEcsContainer.hpp>
#include <Miracle/Common/Models/EntityConfig.hpp>
#include <Miracle/Common/Behaviour.hpp>
#include <Miracle/Common/Math/Vector3.hpp>

namespace Miracle::Application {
	class IEcsContainer : public Miracle::IEcsContainer {
	public:
		virtual ~IEcsContainer() = default;

		virtual void createEntity(const EntityConfig& config) = 0;

		virtual void forEachPosition(const std::function<void(const Vector3&)>& forEach) const = 0;

		virtual void forEachBehaviour(const std::function<void(Behaviour&)>& forEach) = 0;
	};
}
