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
#include <stdexcept>
#include <initializer_list>
#include <glad/glad.h>

#include "cmw/core/log.hpp"
#include "cmw/gl/object.hpp"
#include "cmw/utils.hpp"

namespace cmw::gl {

struct BufferElement {
    enum Type: uint16_t {
        Invalid = 0,
        _1    = CMW_BIT(0), _2    = CMW_BIT(1), _3     = CMW_BIT(2), _4   = CMW_BIT(3),
        _bool = CMW_BIT(4), _Byte = CMW_BIT(5), _Ubyte = CMW_BIT(6),
        _Int  = CMW_BIT(7), _Uint = CMW_BIT(8), _Float = CMW_BIT(9), _Mat = CMW_BIT(10),
        _Nb = _1 | _2 | _3 | _4, _Type = _bool | _Byte | _Ubyte | _Int | _Uint | _Float | _Mat,
        Bool  = _bool  | _1,
        Byte  = _Byte  | _1, Byte2  = _Byte  | _2, Byte3  = _Byte  | _3, Byte4  = _Byte  | _4,
        Ubyte = _Ubyte | _1, Ubyte2 = _Ubyte | _2, Ubyte3 = _Ubyte | _3, Ubyte4 = _Ubyte | _4,
        Int   = _Int   | _1, Int2   = _Int   | _2, Int3   = _Int   | _3, Int4   = _Int   | _4,
        Uint  = _Uint  | _1, Uint2  = _Uint  | _2, Uint3  = _Uint  | _3, Uint4  = _Uint  | _4,
        Float = _Float | _1, Float2 = _Float | _2, Float3 = _Float | _3, Float4 = _Float | _4,
                             Mat2   = _Mat   | _2, Mat3   = _Mat   | _3, Mat4   = _Mat   | _4,
    };

    Type type;
    GLenum gl_type;
    std::size_t nb, size;
    bool normalized;

    constexpr BufferElement(): type(Type::Invalid), gl_type(0), nb(0), size(0), normalized(0) { }
    constexpr BufferElement(Type type, bool normalized = false):
        type(type), gl_type(get_gl_type(type)), nb(get_nb(type)), size(get_size(type)), normalized(normalized) { }

    static constexpr std::size_t get_size(Type type) {
        return ((type & (Type::_bool | Type::_Byte | Type::_Ubyte)) ? 1 : 4) * get_nb(type);
    }

    static constexpr std::size_t get_nb(Type type) {
        if (type & Type::_Mat)
            return __builtin_ffs(type & Type::_Nb) * __builtin_ffs(type & Type::_Nb);
        return __builtin_ffs(type & Type::_Nb);
    }

    static constexpr GLenum get_gl_type(Type type) {
        if      (type & Type::_bool)  return GL_BOOL;
        else if (type & Type::_Byte)  return GL_BYTE;
        else if (type & Type::_Ubyte) return GL_UNSIGNED_BYTE;
        else if (type & Type::_Int)   return GL_INT;
        else if (type & Type::_Uint)  return GL_UNSIGNED_INT;
        else return GL_FLOAT;
    }
};

struct BufferLayout {
    constexpr BufferLayout(std::initializer_list<BufferElement> &&elements):
        elements(elements), stride(get_stride(std::forward<std::initializer_list<BufferElement>>(elements))) { }

    static constexpr std::size_t get_stride(std::initializer_list<BufferElement> &&elements) {
        std::size_t stride = 0;
        for (const auto &element: elements)
            stride += element.size;
        return stride;
    }

    std::initializer_list<BufferElement> elements;
    std::size_t stride;
};

template <GLenum Type, std::size_t N = 1>
class BufferN: public GlObject {
    public:
        BufferN() {
            CMW_TRACE("Creating buffer object\n");
            glGenBuffers(get_nb(), &this->handle);
            CMW_TRY_THROW(get_handle(), std::runtime_error("Could not create buffer object"));
            bind();
        }

        ~BufferN() {
            CMW_TRACE("Destructing buffer object\n");
            glDeleteBuffers(get_nb(), &this->handle);
        }

        void set_data(const void *data, std::size_t size, GLenum draw_type = GL_STATIC_DRAW) {
            this->size = size;
            glBufferData(get_type(), size, data, draw_type);
        }

        void set_sub_data(const void *data, std::size_t size, void *off = 0) const {
            glBufferSubData(get_type(), (GLintptr)off, size, data);
        }

        void bind() const {
            glBindBuffer(get_type(), get_handle());
        }

        static void bind(GLuint handle) {
            glBindBuffer(get_type(), handle);
        }

        static void unbind() {
            glBindBuffer(get_type(), 0);
        }

        static inline std::size_t get_nb()         { return N; }
        static inline GLenum      get_type()       { return Type; }
        inline std::size_t        get_size() const { return this->size; }

    protected:
        std::size_t size;
};

template <std::size_t N = 1>
class VertexBufferN: public BufferN<GL_ARRAY_BUFFER, N> {
    public:
        static void set_layout(BufferLayout &&layout) {
            std::size_t i = 0, off = 0;
            for (const auto &element: layout.elements) {
                if (element.type & (BufferElement::_Byte | BufferElement::_Ubyte | BufferElement::_Int | BufferElement::_Uint))
                    set_attrib_iptr(i, element.nb, element.gl_type, layout.stride, (GLvoid *)off);
                else
                    set_attrib_ptr(i, element.nb, element.gl_type, layout.stride, (GLvoid *)off, element.normalized);
                off += element.size; ++i;
            }
        }

        static void set_attrib_ptr(GLuint pos, GLuint size, GLenum type, GLuint stride = 0, GLvoid *off = nullptr, bool normalize = false) {
            glVertexAttribPointer(pos, size, type, normalize, stride, off);
            enable_attrib_arr(pos);
        }

        static void set_attrib_iptr(GLuint pos, GLuint size, GLenum type, GLuint stride = 0, GLvoid *off = nullptr) {
            glVertexAttribIPointer(pos, size, type, stride, off);
            enable_attrib_arr(pos);
        }

        static void enable_attrib_arr(GLuint pos) {
            glEnableVertexAttribArray(pos);
        }

        static void disable_attrib_arr(GLuint pos) {
            glDisableVertexAttribArray(pos);
        }
};

template <std::size_t N = 1>
class ElementBufferN: public BufferN<GL_ELEMENT_ARRAY_BUFFER, N> { };

template <GLenum Type>
using Buffer        = BufferN<Type, 1>;
using VertexBuffer  = VertexBufferN<1>;
using ElementBuffer = ElementBufferN<1>;

} // namespace cmw::gl
