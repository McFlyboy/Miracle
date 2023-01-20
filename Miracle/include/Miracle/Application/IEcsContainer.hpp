#pragma once

#include <memory>
#include <functional>

#include <Miracle/Common/Math/Vector2f.hpp>
#include <Miracle/Common/Behaviour.hpp>

namespace Miracle::Application {
	class IEcsContainer {
	public:
		virtual ~IEcsContainer() = default;

		inline virtual const Vector2f& getEntityPosition() const = 0;

		virtual void setEntityPosition(const Vector2f& position) = 0;

		virtual void addEntityBehaviour(std::unique_ptr<Behaviour>&& behaviour) = 0;

		virtual void forEachEntityBehaviour(const std::function<void(Behaviour&)>& forEach) = 0;
	};
}
