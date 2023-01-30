#pragma once

#include <memory>
#include <vector>
#include <functional>

#include <Miracle/Common/Math/Color3f.hpp>
#include <Miracle/Common/Math/Vector2f.hpp>
#include <Miracle/Common/Models/EntityConfig.hpp>
#include <Miracle/Application/IEcs.hpp>
#include <Miracle/Application/IEcsContainer.hpp>

namespace Miracle::Application {
	struct SceneInitProps {
		Color3f backgroundColor = {};
		std::vector<EntityConfig> entityConfigs = {};
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

		void update();

		void forEachPosition(const std::function<void(const Vector2f&)>& forEach) const;

		void addEntity(const EntityConfig& config);
	};
}
