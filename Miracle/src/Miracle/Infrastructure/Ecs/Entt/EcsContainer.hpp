#pragma once

#include <entt/entity/registry.hpp>

#include <Miracle/Application/IEcsContainer.hpp>

namespace Miracle::Infrastructure::Ecs::Entt {
	class EcsContainer : public Application::IEcsContainer {
	private:
		entt::basic_registry<EntityId> m_registry;

	public:
		inline virtual size_t getEntityCount() const override { return m_registry.storage<EntityId>()->in_use(); }

		virtual EntityId createEntity(const EntityConfig& config) override;

		virtual void destroyEntity(EntityId id) override;

		inline virtual Transform& getTransform(EntityId id) override { return m_registry.get<Transform>(id); }

		inline virtual const Transform& getTransform(EntityId id) const override { return m_registry.get<Transform>(id); }

		virtual void forEachTransform(const std::function<void(const Transform&)>& forEach) const override;

		virtual void forEachBehaviour(const std::function<void(Behaviour&)>& forEach) override;
	};
}
