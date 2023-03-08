#pragma once

#include <memory>
#include <vector>
#include <functional>

#include <Miracle/Common/Math/ColorRgb.hpp>
#include <Miracle/Common/Math/Vector3.hpp>
#include <Miracle/Common/Models/EntityConfig.hpp>
#include <Miracle/Application/IEcs.hpp>
#include <Miracle/Application/IEcsContainer.hpp>

namespace Miracle::Application {
	struct SceneInitProps {
		ColorRgb backgroundColor = {};
		std::vector<EntityConfig> entityConfigs = {};
	};

	class Scene {
	private:
		std::unique_ptr<IEcsContainer> m_container;
		ColorRgb m_backgroundColor;

	public:
		Scene(
			IEcs& ecs,
			const SceneInitProps& initProps
		);

		inline const ColorRgb& getBackgroundColor() const { return m_backgroundColor; }

		void setBackgroundColor(const ColorRgb& color);

		void addEntity(const EntityConfig& config);

		void forEachEntityPosition(const std::function<void(const Vector3&)>& forEach) const;

		void update();
	};
}
