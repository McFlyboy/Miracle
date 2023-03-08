#pragma once

#include <Miracle/Common/Models/EntityId.hpp>
#include <Miracle/Common/Math/Vector3.hpp>

namespace Miracle {
	class IEcsContainer {
	public:
		virtual ~IEcsContainer() = default;

		virtual Vector3& getPosition(EntityId owner) = 0;
	};
}
