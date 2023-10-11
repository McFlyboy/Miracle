#pragma once

#include "Definitions.hpp"

namespace Miracle {
	enum class Platform {
		windows,
		macos,
		linux
	};

	class Environment {
	public:
		Environment() = delete;

		static constexpr Platform getCurrentPlatform() {
#if defined(MIRACLE_PLATFORM_WINDOWS)
			return Platform::windows;
#elif defined(MIRACLE_PLATFORM_MACOS)
			return Platform::macos;
#elif defined(MIRACLE_PLATFORM_LINUX)
			return Platform::linux;
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
