#pragma once

#include <Miracle/Common/Models/EntityId.hpp>
#include <Miracle/Common/Components/Transform.hpp>

namespace Miracle {
	class IEcsContainer {
	public:
		virtual ~IEcsContainer() = default;

		virtual void scheduleEntityDestruction(EntityId entity) = 0;

		inline virtual Transform& getTransform(EntityId entity) = 0;

		inline virtual const Transform& getTransform(EntityId entity) const = 0;
	};
}
