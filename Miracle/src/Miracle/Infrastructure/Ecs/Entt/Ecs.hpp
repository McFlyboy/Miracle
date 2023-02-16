#pragma once

#include <Miracle/Application/IEcs.hpp>

namespace Miracle::Infrastructure::Ecs::Entt {
	class Ecs : public Application::IEcs {
		virtual std::unique_ptr<Application::IEcsContainer> createContainer() const override;
	};
}
