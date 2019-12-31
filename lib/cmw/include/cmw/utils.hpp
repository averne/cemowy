// Copyright (C) 2019 averne
//
// This file is part of cemowy.
//
// cemowy is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// cemowy is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with cemowy.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <cstdint>
#include <cstdlib>
#include <functional>
#include <utility>

#include "cmw/core/log.hpp"
#include "cmw/platform.h"

#define CMW_STRING_IMPL(x)   #x
#define CMW_STRING(x)        CMW_STRING_IMPL(x)
#define CMW_CAT_IMPL(x1, x2) x1##x2
#define CMW_CAT(x1, x2)      CMW_CAT_IMPL(x1, x2)

#define CMW_ANONYMOUS_VAR CMW_CAT(var_, __COUNTER__)

#define CMW_MAX(x, y) (((x) > (y)) ? (x) :  (y))
#define CMW_MIN(x, y) (((x) < (y)) ? (x) :  (y))
#define CMW_ABS(x)    (((x) > 0) ?   (x) : -(x))

#define CMW_BIT(n) (1 << (n))

#define CMW_VEC_SIZE(x) (sizeof((x)) / sizeof(*(x)))

#define CMW_PACKED     __attribute((packed))
#define CMW_ALIGNED(n) __attribute__((aligned(n)))

#define CMW_TRY_IMPL(x, cb) ({                                                                      \
    if (!(x)) {                                                                                     \
        CMW_ERROR(CMW_STRING(x) " failed\n");                                                       \
        ({cb;});                                                                                    \
    }                                                                                               \
})

#define CMW_TRY_RC_IMPL(x, cb) ({                                                                   \
    if (::cmw::Result rc = (::cmw::Result)(x); rc.failed()) {                                       \
        CMW_ERROR(CMW_STRING(x) " failed: %#x\n", rc);                                              \
        ({cb;});                                                                                    \
    }                                                                                               \
})

#define CMW_TRY(x)             CMW_TRY_IMPL(x, )
#define CMW_TRY_GOTO(x, l)     CMW_TRY_IMPL(x, goto l)
#define CMW_TRY_THROW(x, e)    CMW_TRY_IMPL(x, throw e)
#define CMW_TRY_RETURN(x, v)   CMW_TRY_IMPL(x, return v)
#define CMW_TRY_FATAL(x, v)    CMW_TRY_IMPL(x, CMW_FATAL(v))
#define CMW_TRY_RC(x)          CMW_TRY_RC_IMPL(x, )
#define CMW_TRY_RC_THROW(x, e) CMW_TRY_RC_IMPL(x, throw e)
#define CMW_TRY_RC_RETURN(x)   CMW_TRY_RC_IMPL(x, return (uint32_t)rc)
#define CMW_TRY_RC_FATAL(x)    CMW_TRY_RC_IMPL(x, CMW_FATAL(rc))

#define CMW_ASSERT_SIZE(x, sz)        static_assert(sizeof(x) == (sz), "Wrong size in " CMW_STRING(x))
#define CMW_ASSERT_STANDARD_LAYOUT(x) static_assert(std::is_standard_layout_v<x>, CMW_STRING(x) " is not standard layout")

#define CMW_SERV_INIT(s, ...) CMW_TRY_FATAL(CMW_CAT(s, Initialize)(__VA_ARGS__))
#define CMW_SERV_EXIT(s, ...) CMW_CAT(s, Exit)(__VA_ARGS__)

#ifdef CMW_SWITCH
#   define CMW_EXIT(x) fatalSimple(x)
#else
#   define CMW_EXIT(x) _Exit(x)
#endif

#define CMW_NON_COPYABLE(T)                 \
    public:                                 \
        T(const T &)            = delete;   \
        T &operator=(const T &) = delete

#define CMW_NON_MOVEABLE(T)                 \
    public:                                 \
        T(T &&)            = delete;        \
        T &operator=(T &&) = delete

namespace cmw {

template <typename ...Args>
static inline void bind_all(Args &&...args) {
    (args.bind(), ...);
}

template <typename ...Args>
static inline void unbind_all(Args &&...args) {
    (args.unbind(), ...);
}

} // namespace cmw

#include "cmw/utils/area.hpp"
#include "cmw/utils/asset.hpp"
#include "cmw/utils/color.hpp"
#include "cmw/utils/error.hpp"
#include "cmw/utils/position.hpp"
#include "cmw/utils/result.hpp"
#include "cmw/utils/scope_guard.hpp"
#include "cmw/utils/time.hpp"
