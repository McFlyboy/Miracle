#pragma once

#include <Miracle/App.hpp>
#include <Miracle/Common/Math/Color3f.hpp>
#include <Miracle/Common/Models/EntityConfig.hpp>

namespace Miracle {
	class Scene {
	public:
		Scene() = delete;

		static inline const Color3f& getBackgroundColor() {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			return App::s_currentApp->m_dependencies->getSceneManager()
				.getCurrentScene()
				.getBackgroundColor();
		}

		static inline void setBackgroundColor(const Color3f& color) {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			App::s_currentApp->m_dependencies->getSceneManager()
				.getCurrentScene()
				.setBackgroundColor(color);
		}

		static inline void addEntity(const EntityConfig& config) {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			App::s_currentApp->m_dependencies->getSceneManager()
				.getCurrentScene()
				.addEntity(config);
		}
	};
}
