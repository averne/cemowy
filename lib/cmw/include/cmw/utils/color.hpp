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
#include <limits>

namespace cmw {

template <typename T>
struct ColorX {
    T r = std::numeric_limits<T>::max();
    T g = std::numeric_limits<T>::max();
    T b = std::numeric_limits<T>::max();
    T a = std::numeric_limits<T>::max();

    constexpr inline ColorX() = default;
    constexpr inline ColorX(T r, T g, T b):      r(r), g(g), b(b)       { }
    constexpr inline ColorX(T r, T g, T b, T a): r(r), g(g), b(b), a(a) { }

    inline bool operator==(const ColorX<T>& rhs) {
        return ((r == rhs.r) && (g == rhs.g) && (b == rhs.b) && (a == rhs.a));
    }

    inline bool operator!=(const ColorX<T> &rhs) {
        return !(*this == rhs);
    }
};

using Colori = ColorX<std::int8_t>;
using Coloru = ColorX<std::uint8_t>;
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
