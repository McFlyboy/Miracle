#pragma once

#include <memory>

#include "IEcsContainer.hpp"

namespace Miracle::Application {
	class IEcs {
	public:
		virtual ~IEcs() = default;

		virtual std::unique_ptr<IEcsContainer> createContainer() const = 0;
	};
}
