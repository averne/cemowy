#pragma once

#include "platform.h"

#define CMW_STRING_IMPL(x)   #x
#define CMW_STRING(x)        CMW_STRING_IMPL(x)
#define CMW_CAT_IMPL(x1, x2) x1##x2
#define CMW_CAT(x1, x2)      CMW_CAT_IMPL(x1, x2)

#define CMW_MAX(x, y) (((x) > (y)) ? (x) :  (y))
#define CMW_MIN(x, y) (((x) < (y)) ? (x) :  (y))
#define CMW_ABS(x)    (((x) > 0) ?   (x) : -(x))

#define CMW_VEC_SIZE(x) (sizeof((x)) / sizeof(*(x)))

#define CMW_TRY_IMPL(x, cb) ({                                                                      \
    if (!(x)) {                                                                                     \
        ::cmw::Logger::enqueue(::cmw::Logger::Level::Error, CMW_STRING(x) " failed\n");             \
        ({cb;});                                                                                    \
    }                                                                                               \
})

#define CMW_TRY_RC_IMPL(x, cb) ({                                                                   \
    if (Result rc = (x); R_FAILED(rc)) {                                                            \
        ::cmw::Logger::enqueue(::cmw::Logger::Level::Error, CMW_STRING(x) " failed: %#x\n", rc);    \
        ({cb;});                                                                                    \
    }                                                                                               \
})

#define CMW_TRY(x)           CMW_TRY_IMPL(x, )
#define CMW_ASSERT(x)        CMW_TRY_IMPL(x, )
#define CMW_TRY_GOTO(x, l)   CMW_TRY_IMPL(x, goto l)
#define CMW_TRY_RETURN(x, v) CMW_TRY_IMPL(x, return v)
#define CMW_TRY_FATAL(x, v)  CMW_TRY_IMPL(x, CMW_FATAL(v))
#define CMW_TRY_RC(x)        CMW_TRY_RC_IMPL(x, )
#define CMW_TRY_RC_RETURN(x) CMW_TRY_RC_IMPL(x, return rc)
#define CMW_TRY_RC_FATAL(x)  CMW_TRY_RC_IMPL(x, CMW_FATAL(rc))

#define CMW_ASSERT_SIZE(x, sz)        static_assert(sizeof(x) == (sz), "Wrong size in " CMW_STRING(x))
#define CMW_ASSERT_STANDARD_LAYOUT(x) static_assert(std::is_standard_layout_v<x>, CMW_STRING(x) " is not standard layout")

#define CMW_SERV_INIT(s, ...) CMW_TRY_FATAL(CONCATENATE(s, Initialize)(__VA_ARGS__))
#define CMW_SERV_EXIT(s, ...) CONCATENATE(s, Exit)(__VA_ARGS__)

#ifdef CMW_SWITCH
#   define CMW_EXIT(x) fatalSimple(x)
#else
#   define CMW_EXIT(x) _Exit(x)
#endif

#define CMW_NON_COPYABLE(T)                 \
    public:                                 \
        T(const T &) = delete;              \
        T &operator=(const T &) = delete

#define CMW_NON_MOVEABLE(T)                 \
    public:                                 \
        T(T &&) = delete;                   \
        T &operator=(T &&) = delete

namespace cmw {

class Result {
    public:
        constexpr Result() = default;
        constexpr Result(uint32_t res):                   res(res)                           { }
        constexpr Result(uint32_t module, uint32_t desc): res((module - 2000) | (desc << 9)) { }

        explicit operator uint32_t() const { return this->res; }

        inline uint32_t code()   const { return  this->res;}
        inline uint32_t module() const { return  this->res & 0x1ff; }
        inline uint32_t desc()   const { return (this->res >> 9) & 0x3fff; }

        inline bool operator==(const Result &rhs) const {
            return this->res == rhs.code();
        }

        inline bool operator!=(const Result &rhs) const {
            return (*this == rhs);
        }

    protected:
        uint32_t res = 0;
};

} // namespace cmw
