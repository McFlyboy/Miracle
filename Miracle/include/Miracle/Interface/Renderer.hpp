#pragma once

#include <Miracle/App.hpp>
#include <Miracle/Common/Math/Color3f.hpp>
#include <Miracle/Common/Math/Vector2f.hpp>

namespace Miracle {
	class Renderer {
	public:
		Renderer() = delete;

		static inline const Color3f& getClearColor() {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			return App::s_currentApp->m_dependencies->getRenderer().getClearColor();
		}

		static inline void setClearColor(const Color3f& clearColor) {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			App::s_currentApp->m_dependencies->getRenderer().setClearColor(clearColor);
		}

		static inline const Vector2f& getTranslation() {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			return App::s_currentApp->m_dependencies->getRenderer().getTranslation();
		}

		static inline void setTranslation(const Vector2f& translation) {
			if (App::s_currentApp == nullptr) [[unlikely]] throw NoAppRunningError();

			App::s_currentApp->m_dependencies->getRenderer().setTranslation(translation);
		}
	};
}
