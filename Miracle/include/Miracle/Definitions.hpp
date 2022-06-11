#pragma once

// ----- Platform definitions -----
#if defined(_WIN32)
#define MIRACLE_PLATFORM_WINDOWS 1
#elif defined(__APPLE__)
#define MIRACLE_PLATFORM_MACOS 1
#elif defined(__linux__)
#define MIRACLE_PLATFORM_LINUX 1
#else
#error Miracle does not support this platform! Platforms supported: Windows, MacOS, Linux
#endif

// ----- Configuration definitions -----
#ifdef NDEBUG
#define MIRACLE_CONFIG_RELEASE 1
#else
#define MIRACLE_CONFIG_DEBUG 1
#endif
