#pragma once

/* ----- Platform definitions ----- */

#if defined(_WIN32)
#define MIRACLE_PLATFORM_WINDOWS
#elif defined(__APPLE__)
#define MIRACLE_PLATFORM_MACOS
#elif defined(__linux__)
#define MIRACLE_PLATFORM_LINUX
#else
#error Miracle does not support this platform! Platforms supported: Windows, macOS, Linux
#endif

/* ----- Configuration definitions ----- */

#ifdef NDEBUG
#define MIRACLE_CONFIG_RELEASE
#else
#define MIRACLE_CONFIG_DEBUG
#endif
