#include "EcsContainer.hpp"

#include <memory>
#include <utility>

#include <Miracle/Common/EntityContext.hpp>
#include <Miracle/Common/Components/Transform.hpp>
#include <Miracle/Common/Components/Camera.hpp>
#include <Miracle/Common/Components/Appearance.hpp>
#include <Miracle/Common/Components/Behavior.hpp>

namespace Miracle::Infrastructure::Ecs::Entt {
	EntityId EcsContainer::createEntity(const EntityConfig& config) {
		auto entity = m_registry.create();

		m_registry.emplace<Transform>(
			entity,
			config.transformConfig.translation,
			config.transformConfig.rotation,
			config.transformConfig.scale
		);

		if (config.cameraConfig.has_value()) {
			if (std::holds_alternative<OrthographicCameraConfig>(config.cameraConfig.value())) {
				auto& cameraConfig = std::get<OrthographicCameraConfig>(config.cameraConfig.value());

				m_registry.emplace<Camera>(
					entity,
					cameraConfig.zoomFactor,
					cameraConfig.nearClipPlaneDistance,
					cameraConfig.farClipPlaneDistance
				);
			}
			else {
				auto& cameraConfig = std::get<PerspectiveCameraConfig>(config.cameraConfig.value());

				if (std::holds_alternative<Degrees>(cameraConfig.fieldOfView)) {
					m_registry.emplace<Camera>(
						entity,
						std::get<Degrees>(cameraConfig.fieldOfView),
						cameraConfig.nearClipPlaneDistance,
						cameraConfig.farClipPlaneDistance
					);
				}
				else {
					m_registry.emplace<Camera>(
						entity,
						std::get<Radians>(cameraConfig.fieldOfView),
						cameraConfig.nearClipPlaneDistance,
						cameraConfig.farClipPlaneDistance
					);
				}
			}
		}

		if (config.appearanceConfig.has_value()) {
			auto& appearanceConfig = config.appearanceConfig.value();

			m_registry.emplace<Appearance>(
				entity,
				appearanceConfig.visible,
				appearanceConfig.meshIndex,
				appearanceConfig.color
			);
		}
		
		if (config.behaviorFactory.has_value()) {
			m_registry.emplace<std::unique_ptr<BehaviorBase>>(
				entity,
				config.behaviorFactory.value().produce(
					EntityContext(entity, *this)
				)
			);
		}

		m_entityCreatedCallback(entity);
		return entity;
	}

	void EcsContainer::scheduleEntityDestruction(EntityId entity) {
		m_entitiesScheduledForDestruction.emplace(entity);
	}

	void EcsContainer::destroyScheduledEntities() {
		for (auto entity : m_entitiesScheduledForDestruction) {
			m_registry.destroy(entity);
			m_entityDestroyedCallback(entity);
		}

		m_entitiesScheduledForDestruction.clear();
	}

	void EcsContainer::setEntityCreatedCallback(std::function<void(EntityId)>&& entityCreatedCallback) {
		m_entityCreatedCallback = std::move(entityCreatedCallback);
	}

	void EcsContainer::unsetEntityCreatedCallback() {
		m_entityCreatedCallback = [](EntityId) {};
	}

	void EcsContainer::setEntityDestroyedCallback(std::function<void(EntityId)>&& entityDestroyedCallback) {
		m_entityDestroyedCallback = std::move(entityDestroyedCallback);
	}

	void EcsContainer::unsetEntityDestroyedCallback() {
		m_entityDestroyedCallback = [](EntityId) {};
	}

	void EcsContainer::forEachCamera(
		const std::function<void(const Transform&, const Camera&)>& forEach
	) const {
		for (auto [entity, transform, camera] : m_registry.view<Transform, Camera>().each()) {
			forEach(transform, camera);
		}
	}

	void EcsContainer::forEachAppearance(
		const std::function<void(const Transform&, const Appearance&)>& forEach
	) const {
		for (auto [entity, transform, appearance] : m_registry.view<Transform, Appearance>().each()) {
			forEach(transform, appearance);
		}
	}

	void EcsContainer::forEachBehavior(const std::function<void(BehaviorBase&)>& forEach) {
		for (auto [entity, behavior] : m_registry.view<std::unique_ptr<BehaviorBase>>().each()) {
			forEach(*behavior.get());
		}
	}
}
