#pragma once

#include <set>
#include <functional>

#include <entt/entity/registry.hpp>

#include <Miracle/Application/IEcsContainer.hpp>
#include <Miracle/Common/Models/EntityId.hpp>

namespace Miracle::Infrastructure::Ecs::Entt {
	class EcsContainer : public Application::IEcsContainer {
	private:
		entt::basic_registry<EntityId> m_registry;
		std::set<EntityId> m_entitiesScheduledForDestruction;
		std::function<void(EntityId)> m_entityCreatedCallback = [](EntityId) {};
		std::function<void(EntityId)> m_entityDestroyedCallback = [](EntityId) {};

	public:
		inline virtual size_t getEntityCount() const override {
			return m_registry.storage<EntityId>()->in_use();
		}

		virtual EntityId createEntity(const EntityConfig& config) override;

		virtual void scheduleEntityDestruction(EntityId entity) override;

		virtual void destroyScheduledEntities() override;

		virtual void setEntityCreatedCallback(std::function<void(EntityId)>&& entityCreatedCallback) override;

		virtual void unsetEntityCreatedCallback() override;

		virtual void setEntityDestroyedCallback(std::function<void(EntityId)>&& entityDestroyedCallback) override;

		virtual void unsetEntityDestroyedCallback() override;

		inline virtual Transform& getTransform(EntityId entity) override {
			return m_registry.get<Transform>(entity);
		}

		inline virtual const Transform& getTransform(EntityId entity) const override {
			return m_registry.get<Transform>(entity);
		}

		inline virtual Camera& getCamera(EntityId entity) override {
			return m_registry.get<Camera>(entity);
		}

		inline virtual const Camera& getCamera(EntityId entity) const override {
			return m_registry.get<Camera>(entity);
		}

		inline virtual Appearance& getAppearance(EntityId entity) override {
			return m_registry.get<Appearance>(entity);
		}

		inline virtual const Appearance& getAppearance(EntityId entity) const override {
			return m_registry.get<Appearance>(entity);
		}

		virtual void forEachCamera(
			const std::function<void(const Transform&, const Camera&)>& forEach
		) const override;

		virtual void forEachAppearance(
			const std::function<void(const Transform&, const Appearance&)>& forEach
		) const override;

		virtual void forEachBehavior(const std::function<void(Behavior&)>& forEach) override;
	};
}
