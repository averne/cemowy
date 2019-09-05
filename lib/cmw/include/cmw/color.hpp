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

constexpr Colorf White      { 1.0f, 1.0f, 1.0f, 1.0f };
constexpr Colorf Black      { 0.0f, 0.0f, 0.0f, 1.0f };
constexpr Colorf Gray       { 0.5f, 0.5f, 0.5f, 1.0f };
constexpr Colorf Red        { 1.0f, 0.0f, 0.0f, 1.0f };
constexpr Colorf Green      { 0.0f, 1.0f, 0.0f, 1.0f };
constexpr Colorf Blue       { 0.0f, 0.0f, 1.0f, 1.0f };
constexpr Colorf Yellow     { 1.0f, 1.0f, 0.0f, 1.0f };
constexpr Colorf Magenta    { 1.0f, 0.0f, 1.0f, 1.0f };
constexpr Colorf Cyan       { 0.0f, 1.0f, 1.0f, 1.0f };

} // namespace colors

} // namespace cmw
