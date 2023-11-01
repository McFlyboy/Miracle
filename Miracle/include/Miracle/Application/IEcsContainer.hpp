#pragma once

#include <functional>
#include <optional>

#include <Miracle/Common/IEcsContainer.hpp>
#include <Miracle/Common/Models/EntityConfig.hpp>
#include <Miracle/Common/Models/EntityId.hpp>
#include <Miracle/Common/Components/Transform.hpp>
#include <Miracle/Common/Components/Camera.hpp>
#include <Miracle/Common/Components/Appearance.hpp>
#include <Miracle/Common/Components/Behavior.hpp>

namespace Miracle::Application {
	class IEcsContainer : public Miracle::IEcsContainer {
	public:
		virtual ~IEcsContainer() = default;

		virtual size_t getEntityCount() const = 0;

		virtual EntityId createEntity(const EntityConfig& config) = 0;

		virtual void destroyScheduledEntities() = 0;

		virtual void setEntityCreatedCallback(std::function<void(EntityId)>&& entityCreatedCallback) = 0;

		virtual void unsetEntityCreatedCallback() = 0;

		virtual void setEntityDestroyedCallback(std::function<void(EntityId)>&& entityDestroyedCallback) = 0;

		virtual void unsetEntityDestroyedCallback() = 0;

		virtual void forEachCamera(
			const std::function<void(const Transform&, const Camera&)>& forEach
		) const = 0;

		virtual void forEachAppearance(
			const std::function<void(const Transform&, const Appearance&)>& forEach
		) const = 0;

		virtual void forEachBehavior(const std::function<void(BehaviorBase&)>& forEach) = 0;
	};
}
