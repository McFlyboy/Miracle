#pragma once

#include <memory>
#include <vector>
#include <functional>
#include <utility>
#include <optional>

#include <Miracle/Common/Math/ColorRgb.hpp>
#include <Miracle/Common/Models/EntityConfig.hpp>
#include <Miracle/Common/Models/EntityId.hpp>
#include <Miracle/Common/EntityContext.hpp>
#include <Miracle/Common/Components/Transform.hpp>
#include <Miracle/Common/Components/Camera.hpp>
#include <Miracle/Common/Components/Appearance.hpp>
#include <Miracle/Application/IEcs.hpp>
#include <Miracle/Application/IEcsContainer.hpp>

namespace Miracle::Application {
	struct SceneInitProps {
		ColorRgb backgroundColor = {};
		std::vector<EntityConfig> entityConfigs = {};
		std::function<void(EntityId)> entityCreatedCallback = [](EntityId) {};
		std::function<void(EntityId)> entityDestroyedCallback = [](EntityId) {};
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

		const ColorRgb& getBackgroundColor() const { return m_backgroundColor; }

		void setBackgroundColor(const ColorRgb& color);

		size_t getEntityCount() const { return m_container->getEntityCount(); }

		void createEntity(const EntityConfig& config);

		EntityContext createAndGetEntity(const EntityConfig& config);

		void scheduleEntityDestruction(EntityId entity);

		void destroyScheduledEntities();

		void setEntityCreatedCallback(std::function<void(EntityId)>&& entityCreatedCallback) {
			m_container->setEntityCreatedCallback(std::move(entityCreatedCallback));
		}

		void unsetEntityCreatedCallback() {
			m_container->unsetEntityCreatedCallback();
		}

		void setEntityDestroyedCallback(std::function<void(EntityId)>&& entityDestroyedCallback) {
			m_container->setEntityDestroyedCallback(std::move(entityDestroyedCallback));
		}

		void unsetEntityDestroyedCallback() {
			m_container->unsetEntityDestroyedCallback();
		}

		void forEachEntityCamera(
			const std::function<void(const Transform&, const Camera&)>& forEach
		) const;

		void forEachEntityAppearance(
			const std::function<void(const Transform&, const Appearance&)>& forEach
		) const;

		void update();
	};
}
