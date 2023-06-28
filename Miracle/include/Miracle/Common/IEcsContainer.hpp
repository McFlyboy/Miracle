#pragma once

#include <Miracle/Common/Models/EntityId.hpp>
#include <Miracle/Common/Components/Transform.hpp>

namespace Miracle {
	class IEcsContainer {
	public:
		virtual ~IEcsContainer() = default;

		inline virtual Transform& getTransform(EntityId id) = 0;

		inline virtual const Transform& getTransform(EntityId id) const = 0;
	};
}
