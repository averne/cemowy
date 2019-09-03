#pragma once

#include <cstdint>
#include <limits>

#include "platform.h"

namespace cmw {

template <typename T>
struct ColorX {
    T r = std::numeric_limits<T>::max();
    T g = std::numeric_limits<T>::max();
    T b = std::numeric_limits<T>::max();
    T a = std::numeric_limits<T>::max();

    inline constexpr ColorX() = default;
    inline constexpr ColorX(T r, T g, T b):      r(r), g(g), b(b)       { }
    inline constexpr ColorX(T r, T g, T b, T a): r(r), g(g), b(b), a(a) { }

    inline bool operator==(const ColorX<T>& rhs) {
        return ((r == rhs.r) && (g == rhs.g) && (b == rhs.b) && (a == rhs.a));
    }

    inline bool operator!=(const ColorX<T> &rhs) {
        return !(*this == rhs);
    }
};

using Colori = ColorX<int8_t>;
using Coloru = ColorX<uint8_t>;
using Colorf = ColorX<float>;
using Colord = ColorX<double>;
using Color  = Colorf;

namespace colors {

constexpr Colorf White  {255, 255, 255, 255};
constexpr Colorf Black  {0,   0,   0,   255};
constexpr Colorf Gray   {127, 127, 127, 255};
constexpr Colorf Red    {255, 0,   0,   255};
constexpr Colorf Green  {0,   255, 0,   255};
constexpr Colorf Blue   {0,   0,   255, 255};
constexpr Colorf Yellow {255, 255, 0,   255};
constexpr Colorf Magenta{255, 0,   255, 255};
constexpr Colorf Cyan   {0,   255, 255, 255};

} // namespace colors

} // namespace cmw
