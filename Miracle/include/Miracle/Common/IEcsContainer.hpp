#pragma once

#include <Miracle/Common/Models/EntityId.hpp>
#include <Miracle/Common/Math/Vector2f.hpp>

namespace Miracle {
	class IEcsContainer {
	public:
		virtual ~IEcsContainer() = default;

		virtual Vector2f& getPosition(EntityId owner) = 0;
	};
}
