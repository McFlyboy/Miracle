#pragma once

#include <Miracle/Common/Math/Vector2f.hpp>

namespace Miracle::Application {
	class IEcsContainer {
	public:
		virtual ~IEcsContainer() = default;

		inline virtual const Vector2f& getEntityPosition() const = 0;

		virtual void setEntityPosition(const Vector2f& position) = 0;
	};
}
