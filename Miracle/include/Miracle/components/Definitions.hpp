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

// ----- API definition -----
#if defined(MIRACLE_PLATFORM_WINDOWS) && defined(MIRACLE_LINKAGE_DYNAMIC)
#if defined(MIRACLE_BUILD)
#define MIRACLE_API __declspec(dllexport)
#else
#define MIRACLE_API __declspec(dllimport)
#endif
#else
#define MIRACLE_API
#endif
