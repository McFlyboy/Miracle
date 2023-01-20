#pragma once

#include <entt/entity/registry.hpp>

#include <Miracle/Application/IEcsContainer.hpp>

namespace Miracle::Infrastructure::Ecs::Entt {
	class EcsContainer : public Application::IEcsContainer {
	private:
		entt::registry m_registry;

	public:
		EcsContainer();

		inline virtual const Vector2f& getEntityPosition() const override {
			return m_registry.get<Vector2f>(m_registry.data()[0]);
		}

		virtual void setEntityPosition(const Vector2f& position) override;

		virtual void addEntityBehaviour(std::unique_ptr<Behaviour>&& behaviour) override;

		virtual void forEachEntityBehaviour(const std::function<void(Behaviour&)>& forEach) override;
	};
}
