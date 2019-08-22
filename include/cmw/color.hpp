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

constexpr Color white  {255, 255, 255, 255};
constexpr Color black  {0,   0,   0,   255};
constexpr Color gray   {127, 127, 127, 255};
constexpr Color red    {255, 0,   0,   255};
constexpr Color green  {0,   255, 0,   255};
constexpr Color blue   {0,   0,   255, 255};
constexpr Color yellow {255, 255, 0,   255};
constexpr Color magenta{255, 0,   255, 255};
constexpr Color cyan   {0,   255, 255, 255};

} // namespace cmw
