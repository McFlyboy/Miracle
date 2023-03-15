#pragma once

#include <Miracle/Common/Models/EntityId.hpp>
#include <Miracle/Common/Transform.hpp>

namespace Miracle {
	class IEcsContainer {
	public:
		virtual ~IEcsContainer() = default;

		virtual Transform& getTransform(EntityId owner) = 0;
	};
}
