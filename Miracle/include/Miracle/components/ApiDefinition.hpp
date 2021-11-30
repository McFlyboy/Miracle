#pragma once

#if defined(MIRACLE_PLATFORM_WINDOWS) && defined(MIRACLE_LINKAGE_SHARED)
#if defined(MIRACLE_USAGE_BUILD)
#define MIRACLE_API __declspec(dllexport)
#else
#define MIRACLE_API __declspec(dllimport)
#endif
#else
#define MIRACLE_API
#endif
