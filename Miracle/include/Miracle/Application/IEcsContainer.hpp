#pragma once

#include <functional>

#include <Miracle/Common/IEcsContainer.hpp>
#include <Miracle/Common/Models/EntityConfig.hpp>
#include <Miracle/Common/Components/Transform.hpp>
#include <Miracle/Common/Components/Behaviour.hpp>

namespace Miracle::Application {
	class IEcsContainer : public Miracle::IEcsContainer {
	public:
		virtual ~IEcsContainer() = default;

		inline virtual size_t getEntityCount() const = 0;

		virtual void createEntity(const EntityConfig& config) = 0;

		virtual void forEachTransform(const std::function<void(const Transform&)>& forEach) const = 0;

		virtual void forEachBehaviour(const std::function<void(Behaviour&)>& forEach) = 0;
	};
}
