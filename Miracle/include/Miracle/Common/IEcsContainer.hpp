#pragma once

#include <Miracle/Common/Models/EntityId.hpp>
#include <Miracle/Common/Components/Transform.hpp>
#include <Miracle/Common/Components/Camera.hpp>
#include <Miracle/Common/Components/Appearance.hpp>

namespace Miracle {
	class IEcsContainer {
	public:
		virtual ~IEcsContainer() = default;

		virtual void scheduleEntityDestruction(EntityId entity) = 0;

		virtual Transform& getTransform(EntityId entity) = 0;

		virtual const Transform& getTransform(EntityId entity) const = 0;

		virtual Camera& getCamera(EntityId entity) = 0;

		virtual const Camera& getCamera(EntityId entity) const = 0;

		virtual Appearance& getAppearance(EntityId entity) = 0;

		virtual const Appearance& getAppearance(EntityId entity) const = 0;
	};
}
