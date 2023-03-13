#pragma once

#include <Miracle/Common/Models/EntityId.hpp>
#include <Miracle/Common/Math/Matrix4.hpp>

namespace Miracle {
	class IEcsContainer {
	public:
		virtual ~IEcsContainer() = default;

		virtual Matrix4& getTransform(EntityId owner) = 0;
	};
}
