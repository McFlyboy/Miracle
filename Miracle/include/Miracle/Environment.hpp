#pragma once

#include "Definitions.hpp"

namespace Miracle {
	enum class Platform {
		platformWindows,
		platformMacos,
		platformLinux
	};

	class Environment {
	public:
		Environment() = delete;

		static constexpr Platform getCurrentPlatform() {
#if defined(MIRACLE_PLATFORM_WINDOWS)
			return Platform::platformWindows;
#elif defined(MIRACLE_PLATFORM_MACOS)
			return Platform::platformMacos;
#elif defined(MIRACLE_PLATFORM_LINUX)
			return Platform::platformLinux;
#endif
		}

		static constexpr bool isDebug() {
#if defined(MIRACLE_CONFIG_DEBUG)
			return true;
#else
			return false;
#endif
		}
	};
}
