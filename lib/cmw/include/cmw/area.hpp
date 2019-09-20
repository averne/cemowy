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

#include "position.hpp"

namespace cmw {

template <typename T = float>
struct AreaX {
    union {
        T x, y;
        Position2X<T> pos;
    };
    T w = 0, h = 0;

    constexpr inline AreaX(): pos(0, 0) { }
    constexpr inline AreaX(Position2X<T> pos, T w, T h): pos(pos), w(w), h(h) { }
    constexpr inline AreaX(T w, T h): w(w), h(h) { }
    constexpr inline AreaX(T x, T y, T w, T h): AreaX({x, y}, w, h) { }

    template <typename U>
    constexpr inline operator AreaX<U>() {
        return AreaX<U>(
            static_cast<Position2X<U>>(this->pos),
            static_cast<U>(this->w),
            static_cast<U>(this->h)
        );
    }

    constexpr inline T get_area() const { return this->w * this->h; }

#define DECL_BINARY_OPERATOR(op)                                                    \
    template <typename U>                                                           \
    constexpr inline AreaX<T> operator op(const AreaX<U> &rhs) const {              \
        return AreaX<T>(                                                            \
            this->pos op static_cast<Position2X<T>>(rhs.pos),                       \
            this->w op static_cast<T>(rhs.w),                                       \
            this->h op static_cast<T>(rhs.h)                                        \
        );                                                                          \
    }                                                                               \
                                                                                    \
    template <typename U>                                                           \
    constexpr inline AreaX<T> operator op(const U &scalar) const {                  \
        return AreaX<T>(                                                            \
            this->pos op static_cast<Position2X<T>>(scalar),                        \
            this->w op static_cast<T>(scalar),                                      \
            this->h op static_cast<T>(scalar)                                       \
        );                                                                          \
    }

#define DECL_UNARY_OPERATOR(op)                                                     \
    template <typename U = T>                                                       \
    constexpr inline AreaX<T> &operator op(const AreaX<U> &rhs) {                   \
        this->pos op static_cast<Position2X<T>>(rhs.pos);                           \
        this->w op static_cast<T>(rhs.w);                                           \
        this->h op static_cast<T>(rhs.h);                                           \
        return *this;                                                               \
    }                                                                               \
                                                                                    \
    template <typename U>                                                           \
    constexpr inline AreaX<T> operator op(const U &scalar) {                        \
        this->pos op static_cast<Position2X<T>>(scalar);                            \
        this->w op static_cast<T>(scalar);                                          \
        this->h op static_cast<T>(scalar);                                          \
        return *this;                                                               \
    }

#define DECL_COMP_OPERATOR(op)                                                      \
    template <typename U = T>                                                       \
    constexpr inline bool operator op(const AreaX<U> &rhs) const {                  \
        return this->pos op rhs.pos &&                                              \
            this->w op rhs.w &&                                                     \
            this->h op rhs.h;                                                       \
    }

    DECL_BINARY_OPERATOR(+)
    DECL_BINARY_OPERATOR(-)
    DECL_BINARY_OPERATOR(*)
    DECL_BINARY_OPERATOR(/)
    DECL_BINARY_OPERATOR(&)
    DECL_BINARY_OPERATOR(|)
    DECL_BINARY_OPERATOR(^)
    DECL_BINARY_OPERATOR(>>)
    DECL_BINARY_OPERATOR(<<)

    DECL_UNARY_OPERATOR(=)
    DECL_UNARY_OPERATOR(+=)
    DECL_UNARY_OPERATOR(-=)
    DECL_UNARY_OPERATOR(/=)
    DECL_UNARY_OPERATOR(*=)
    DECL_UNARY_OPERATOR(&=)
    DECL_UNARY_OPERATOR(|=)
    DECL_UNARY_OPERATOR(^=)
    DECL_UNARY_OPERATOR(>>=)
    DECL_UNARY_OPERATOR(<<=)

    DECL_COMP_OPERATOR(==)
    DECL_COMP_OPERATOR(!=)
    DECL_COMP_OPERATOR(>)
    DECL_COMP_OPERATOR(<)
    DECL_COMP_OPERATOR(>=)
    DECL_COMP_OPERATOR(<=)

#undef DECL_BINARY_OPERATOR
#undef DECL_UNARY_OPERATOR
#undef DECL_COMP_OPERATOR
};
CMW_ASSERT_STANDARD_LAYOUT(AreaX<>);

using Areai = AreaX<std::int32_t>;
using Areau = AreaX<std::uint32_t>;
using Areaf = AreaX<float>;
using Aread = AreaX<double>;
using Area  = Areaf;

} // namespace cmw
