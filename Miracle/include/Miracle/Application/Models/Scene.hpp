#pragma once

#include <memory>

#include <Miracle/Common/Math/Vector2f.hpp>
#include <Miracle/Common/Math/Color3f.hpp>
#include <Miracle/Application/IEcs.hpp>
#include <Miracle/Application/IEcsContainer.hpp>

namespace Miracle::Application {
	struct SceneInitProps {
		Color3f backgroundColor = {};
		Vector2f entityPosition = {};
	};

	class Scene {
	private:
		std::unique_ptr<IEcsContainer> m_container;
		Color3f m_backgroundColor;

	public:
		Scene(
			IEcs& ecs,
			const SceneInitProps& initProps
		);

		inline const Color3f& getBackgroundColor() const { return m_backgroundColor; }

		void setBackgroundColor(const Color3f& color);

		inline const Vector2f& getEntityPosition() const { return m_container->getEntityPosition(); }

		void setEntityPosition(const Vector2f& position);
	};
}
