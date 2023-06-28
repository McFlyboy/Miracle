#pragma once

#include <cstddef>

#include <Miracle/App.hpp>
#include <Miracle/Common/Math/ColorRgb.hpp>
#include <Miracle/Common/Models/EntityConfig.hpp>
#include <Miracle/Common/Models/EntityId.hpp>
#include <Miracle/Common/Components/Transform.hpp>

namespace Miracle {
	class CurrentScene {
	public:
		CurrentScene() = delete;

		static inline const ColorRgb& getBackgroundColor() {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			return App::s_currentApp->m_dependencies->getSceneManager()
				.getCurrentScene()
				.getBackgroundColor();
		}

		static inline void setBackgroundColor(const ColorRgb& color) {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			App::s_currentApp->m_dependencies->getSceneManager()
				.getCurrentScene()
				.setBackgroundColor(color);
		}

		static inline size_t getEntityCount() {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			return App::s_currentApp->m_dependencies->getSceneManager()
				.getCurrentScene()
				.getEntityCount();
		}

		static inline EntityId createEntity(const EntityConfig& config) {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			return App::s_currentApp->m_dependencies->getSceneManager()
				.getCurrentScene()
				.createEntity(config);
		}

		static inline void destroyEntity(EntityId id) {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			App::s_currentApp->m_dependencies->getSceneManager()
				.getCurrentScene()
				.destroyEntity(id);
		}

		static inline Transform& getEntityTransform(EntityId id) {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			return App::s_currentApp->m_dependencies->getSceneManager()
				.getCurrentScene()
				.getEntityTransform(id);
		}
	};
}
