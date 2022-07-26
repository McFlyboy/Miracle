#pragma once

#include <GLFW/glfw3.h>

#include <Miracle/Application/IMultimediaFramework.hpp>
#include <Miracle/Application/ILogger.hpp>

namespace Miracle::Infrastructure::Framework::Glfw {
	class MultimediaFramework : public Application::IMultimediaFramework {
	private:
		Application::ILogger& m_logger;

	public:
		MultimediaFramework(Application::ILogger& logger);

		~MultimediaFramework();

		virtual void processEvents() override;

		const GLFWvidmode* getGlfwCurrentVideoModeForPrimaryMonitor() const;
	};
}
