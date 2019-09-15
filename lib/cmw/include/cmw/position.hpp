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
#include <glm/glm.hpp>

#include "utils.hpp"

namespace cmw {

template <typename T = float>
class Position2X {
    public:
        T x = 0, y = 0;

    public:
        constexpr inline Position2X() = default;
        constexpr inline Position2X(T x, T y = 0): x(x), y(y) { }
        constexpr inline Position2X(const glm::vec<2, T, glm::defaultp> &vec): x(vec.x), y(vec.y) { }
        constexpr inline Position2X(std::initializer_list<T> list) {
            std::copy_n(list.begin(), 2, &this->x);
        }

        constexpr inline operator glm::vec<2, T, glm::defaultp>() const {
            return *reinterpret_cast<const glm::vec<2, T, glm::defaultp> *>(this);
        }

        template <typename U>
        constexpr inline operator Position2X<U>() {
            return Position2X<U>(
                static_cast<U>(this->x),
                static_cast<U>(this->y)
            );
        }

        constexpr inline T get_length() const {
            return glm::sqrt(this->x * this->x + this->y * this->y);
        }

        constexpr inline T &operator[](std::size_t idx) {
            return *(reinterpret_cast<T *>(this) + idx);
        }

#define DECL_BINARY_OPERATOR(op)                                                        \
        template <typename U>                                                           \
        constexpr inline Position2X<T> operator op(const Position2X<U> &rhs) const {    \
            return Position2X<T>(                                                       \
                this->x op static_cast<T>(rhs.x),                                       \
                this->y op static_cast<T>(rhs.y)                                        \
            );                                                                          \
        }                                                                               \
                                                                                        \
        template <typename U>                                                           \
        constexpr inline Position2X<T> operator op(const U &scalar) const {             \
            return Position2X<T>(                                                       \
                this->x op static_cast<T>(scalar),                                      \
                this->y op static_cast<T>(scalar)                                       \
            );                                                                          \
        }


#define DECL_UNARY_OPERATOR(op)                                                         \
        template <typename U = T>                                                       \
        constexpr inline Position2X<T> &operator op(const Position2X<U> &rhs) {         \
            this->x op static_cast<T>(rhs.x);                                           \
            this->y op static_cast<T>(rhs.y);                                           \
            return *this;                                                               \
        }                                                                               \
                                                                                        \
        template <typename U>                                                           \
        constexpr inline Position2X<T> operator op(const U &scalar) {                   \
            this->x op static_cast<T>(scalar);                                          \
            this->y op static_cast<T>(scalar);                                          \
            return *this;                                                               \
        }

#define DECL_COMP_OPERATOR(op)                                                          \
        template <typename U = T>                                                       \
        constexpr inline bool operator op(const Position2X<U> &rhs) const {             \
            return this->x op rhs.x &&                                                  \
                this->y op rhs.y;                                                       \
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
CMW_ASSERT_STANDARD_LAYOUT(Position2X<>);

template <typename T = float>
class Position3X {
    public:
        T x = 0, y = 0, z = 0;

    public:
        constexpr inline Position3X() = default;
        constexpr inline Position3X(T x, T y, T z = 0): x(x), y(y), z(z) { }
        constexpr inline Position3X(const glm::vec<3, T, glm::defaultp> &vec): x(vec.x), y(vec.y), z(vec.z) { }
        constexpr inline Position3X(std::initializer_list<T> list) {
            std::copy_n(list.begin(), 3, &this->x);
        }

        constexpr inline operator glm::vec<3, T, glm::defaultp>() const {
            return *reinterpret_cast<const glm::vec<3, T, glm::defaultp> *>(this);
        }

        template <typename U>
        constexpr inline operator Position3X<U>() {
            return Position3X<U>(
                static_cast<U>(this->x),
                static_cast<U>(this->y),
                static_cast<U>(this->z)
            );
        }

        constexpr inline T get_length() const {
            return glm::sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
        }

        constexpr inline T &operator[](std::size_t idx) {
            return *(reinterpret_cast<T *>(this) + idx);
        }

#define DECL_BINARY_OPERATOR(op)                                                        \
        template <typename U>                                                           \
        constexpr inline Position3X<T> operator op(const Position3X<U> &rhs) const {    \
            return Position3X<T>(                                                       \
                this->x op static_cast<T>(rhs.x),                                       \
                this->y op static_cast<T>(rhs.y),                                       \
                this->z op static_cast<T>(rhs.z)                                        \
            );                                                                          \
        }                                                                               \
                                                                                        \
        template <typename U>                                                           \
        constexpr inline Position3X<T> operator op(const U &scalar) const {             \
            return Position3X<T>(                                                       \
                this->x op static_cast<T>(scalar),                                      \
                this->y op static_cast<T>(scalar),                                      \
                this->z op static_cast<T>(scalar)                                       \
            );                                                                          \
        }


#define DECL_UNARY_OPERATOR(op)                                                         \
        template <typename U = T>                                                       \
        constexpr inline Position3X<T> &operator op(const Position3X<U> &rhs) {         \
            this->x op static_cast<T>(rhs.x);                                           \
            this->y op static_cast<T>(rhs.y);                                           \
            this->z op static_cast<T>(rhs.z);                                           \
            return *this;                                                               \
        }                                                                               \
                                                                                        \
        template <typename U>                                                           \
        constexpr inline Position3X<T> operator op(const U &scalar) {                   \
            this->x op static_cast<T>(scalar);                                          \
            this->y op static_cast<T>(scalar);                                          \
            this->z op static_cast<T>(scalar);                                          \
            return *this;                                                               \
        }

#define DECL_COMP_OPERATOR(op)                                                          \
        template <typename U = T>                                                       \
        constexpr inline bool operator op(const Position3X<U> &rhs) const {             \
            return this->x op rhs.x &&                                                  \
                this->y op rhs.y &&                                                     \
                this->z op rhs.z;                                                       \
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
CMW_ASSERT_STANDARD_LAYOUT(Position3X<>);

template <typename T = float>
class Position4X {
    public:
        T x = 0, y = 0, z = 0, w = 0;

    public:
        constexpr inline Position4X() = default;
        constexpr inline Position4X(T x, T y, T z, T w = 0): x(x), y(y), z(z), w(w) { }
        constexpr inline Position4X(const glm::vec<4, T, glm::defaultp> &vec): x(vec.x), y(vec.y), z(vec.z), w(vec.w) { }
        constexpr inline Position4X(std::initializer_list<T> list) {
            std::copy_n(list.begin(), 4, &this->x);
        }

        constexpr inline operator glm::vec<3, T, glm::defaultp>() const {
            return *reinterpret_cast<const glm::vec<4, T, glm::defaultp> *>(this);
        }

        template <typename U>
        constexpr inline operator Position4X<U>() {
            return Position4X<U>(
                static_cast<U>(this->x),
                static_cast<U>(this->y),
                static_cast<U>(this->z),
                static_cast<U>(this->w)
            );
        }

        constexpr inline T get_length() const {
            return glm::sqrt(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w);
        }

        constexpr inline T &operator[](std::size_t idx) {
            return *(reinterpret_cast<T *>(this) + idx);
        }

#define DECL_BINARY_OPERATOR(op)                                                        \
        template <typename U>                                                           \
        constexpr inline Position4X<T> operator op(const Position4X<U> &rhs) const {    \
            return Position4X<T>(                                                       \
                this->x op static_cast<T>(rhs.x),                                       \
                this->y op static_cast<T>(rhs.y),                                       \
                this->z op static_cast<T>(rhs.z),                                       \
                this->w op static_cast<T>(rhs.w)                                        \
            );                                                                          \
        }                                                                               \
                                                                                        \
        template <typename U>                                                           \
        constexpr inline Position4X<T> operator op(const U &scalar) const {             \
            return Position4X<T>(                                                       \
                this->x op static_cast<T>(scalar),                                      \
                this->y op static_cast<T>(scalar),                                      \
                this->z op static_cast<T>(scalar),                                      \
                this->w op static_cast<T>(scalar)                                       \
            );                                                                          \
        }


#define DECL_UNARY_OPERATOR(op)                                                         \
        template <typename U = T>                                                       \
        constexpr inline Position4X<T> &operator op(const Position4X<U> &rhs) {         \
            this->x op static_cast<T>(rhs.x);                                           \
            this->y op static_cast<T>(rhs.y);                                           \
            this->z op static_cast<T>(rhs.z);                                           \
            this->w op static_cast<T>(rhs.w);                                           \
            return *this;                                                               \
        }                                                                               \
                                                                                        \
        template <typename U>                                                           \
        constexpr inline Position4X<T> operator op(const U &scalar) {                   \
            this->x op static_cast<T>(scalar);                                          \
            this->y op static_cast<T>(scalar);                                          \
            this->z op static_cast<T>(scalar);                                          \
            this->w op static_cast<T>(scalar);                                          \
            return *this;                                                               \
        }

#define DECL_COMP_OPERATOR(op)                                                          \
        template <typename U = T>                                                       \
        constexpr inline bool operator op(const Position4X<U> &rhs) const {             \
            return this->x op rhs.x &&                                                  \
                this->y op rhs.y &&                                                     \
                this->z op rhs.z &&                                                     \
                this->w op rhs.w;                                                       \
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
CMW_ASSERT_STANDARD_LAYOUT(Position4X<>);

template <typename T>
struct Area { // TODO: More area classes?
    constexpr Area() = default;
    constexpr Area(T w, T h): w(w), h(h) { }

    constexpr inline T get_w() const { return this->w; }
    constexpr inline T get_h() const { return this->h; }
    constexpr inline std::pair<T, T> get_dims() const { return {this->w, this->h}; }

    T w = 0, h = 0;
};

using Position2i = Position2X<std::int32_t>;
using Position2u = Position2X<std::uint32_t>;
using Position2f = Position2X<float>;
using Position2d = Position2X<double>;

using Position3i = Position3X<std::int32_t>;
using Position3u = Position3X<std::uint32_t>;
using Position3f = Position3X<float>;
using Position3d = Position3X<double>;

using Position4i = Position4X<std::int32_t>;
using Position4u = Position4X<std::uint32_t>;
using Position4f = Position4X<float>;
using Position4d = Position4X<double>;

using Position   = Position3f;

} // namespace cmw

