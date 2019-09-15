/**
 * Copyright (C) 2019 averne
 *
 * This file is part of cemowy.
 *
 * cemowy is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * cemowy is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with cemowy.  If not, see <http://www.gnu.org/licenses/>.
 */

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
#    define CMW_DEBUG  = 1
#else
#    define CMW_NDEBUG = 1
#endif

#define CMW_LOG_BACKEND_STDOUT 1
#define CMW_LOG_BACKEND_FILE   2
#define CMW_LOG_BACKEND_IMGUI  3

#ifndef CMW_LOG_BACKEND
#    define CMW_LOG_BACKEND CMW_LOG_BACKEND_STDOUT
#endif

#ifdef __cplusplus
}
#endif // extern "C"

#endif // _CMW_PLATFORM_H
