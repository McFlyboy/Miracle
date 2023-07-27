#pragma once

#include <cstddef>
#include <functional>
#include <utility>

#include <Miracle/App.hpp>
#include <Miracle/Common/Math/ColorRgb.hpp>
#include <Miracle/Common/Models/EntityConfig.hpp>
#include <Miracle/Common/Models/EntityId.hpp>
#include <Miracle/Common/EntityContext.hpp>
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

		static inline void createEntity(const EntityConfig& config) {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			App::s_currentApp->m_dependencies->getSceneManager()
				.getCurrentScene()
				.createEntity(config);
		}

		static inline EntityContext createAndGetEntity(const EntityConfig& config) {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			return App::s_currentApp->m_dependencies->getSceneManager()
				.getCurrentScene()
				.createAndGetEntity(config);
		}

		static inline void setEntityCreatedCallback(std::function<void(EntityId)>&& entityCreatedCallback) {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			App::s_currentApp->m_dependencies->getSceneManager()
				.getCurrentScene()
				.setEntityCreatedCallback(std::move(entityCreatedCallback));
		}

		static inline void unsetEntityCreatedCallback() {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			App::s_currentApp->m_dependencies->getSceneManager()
				.getCurrentScene()
				.unsetEntityCreatedCallback();
		}

		static inline void setEntityDestroyedCallback(std::function<void(EntityId)>&& entityDestroyedCallback) {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			App::s_currentApp->m_dependencies->getSceneManager()
				.getCurrentScene()
				.setEntityDestroyedCallback(std::move(entityDestroyedCallback));
		}

		static inline void unsetEntityDestroyedCallback() {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			App::s_currentApp->m_dependencies->getSceneManager()
				.getCurrentScene()
				.unsetEntityDestroyedCallback();
		}
	};
}
