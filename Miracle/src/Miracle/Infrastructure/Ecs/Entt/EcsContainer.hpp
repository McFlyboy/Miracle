#pragma once

#include <entt/entity/registry.hpp>

#include <Miracle/Application/IEcsContainer.hpp>

namespace Miracle::Infrastructure::Ecs::Entt {
	class EcsContainer : public Application::IEcsContainer {
	private:
		entt::registry m_registry;

	public:
		virtual void createEntity(const EntityConfig& config) override;

		virtual Matrix4& getTransform(EntityId owner) override;

		virtual void forEachTransform(const std::function<void(const Matrix4&)>& forEach) const override;

		virtual void forEachBehaviour(const std::function<void(Behaviour&)>& forEach) override;
	};
}
