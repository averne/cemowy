#ifndef _CMW_PLATFORM_H
#define _CMW_PLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __SWITCH__
#   define CMW_SWITCH
#   include <switch.h>
#else
#   define CMW_PC // TODO: Complete with other os-dependent stuff
#endif

#ifdef DEBUG
#	define CMW_DEBUG  = 1
#else
#	define CMW_NDEBUG = 1
#endif

#define CMW_LOG_BACKEND_STDOUT 1
#define CMW_LOG_BACKEND_FILE   2
#define CMW_LOG_BACKEND_IMGUI  3

#ifndef CMW_LOG_BACKEND
#	define CMW_LOG_BACKEND CMW_LOG_BACKEND_STDOUT
#endif

#ifdef __cplusplus
}
#endif // extern "C"

#endif // _CMW_PLATFORM_H
