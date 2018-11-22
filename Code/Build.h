#pragma once

// Build Settings - BUILD_DEBUG, BUILD_RELEASE
#define BUILD_DEBUG
//#define BUILD_RELEASE
//~Build Settings

#ifdef BUILD_DEBUG
#define DEBUG_CODE(Code) Code
#else
#define DEBUG_CODE(Code)
#endif

#ifdef BUILD_RELEASE
#define RELEASE_CODE(Code) Code
#else
#define RELEASE_CODE(Code)
#endif

