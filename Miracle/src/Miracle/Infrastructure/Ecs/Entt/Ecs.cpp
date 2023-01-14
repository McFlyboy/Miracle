#include "Ecs.hpp"

#include "EcsContainer.hpp"

namespace Miracle::Infrastructure::Ecs::Entt {
	std::unique_ptr<Application::IEcsContainer> Ecs::createContainer() const {
		return std::make_unique<EcsContainer>();
	}
}
