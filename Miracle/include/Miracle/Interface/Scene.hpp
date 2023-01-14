#pragma once

#include <Miracle/App.hpp>
#include <Miracle/Common/Math/Color3f.hpp>
#include <Miracle/Common/Math/Vector2f.hpp>

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

		static inline const Vector2f& getEntityPosition() {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			return App::s_currentApp->m_dependencies->getSceneManager()
				.getCurrentScene()
				.getEntityPosition();
		}

		static inline void setEntityPosition(const Vector2f& position) {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			App::s_currentApp->m_dependencies->getSceneManager()
				.getCurrentScene()
				.setEntityPosition(position);
		}
	};
}
