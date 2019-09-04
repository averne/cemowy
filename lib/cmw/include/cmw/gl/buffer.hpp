#pragma once

#include <cstdint>
#include <stdexcept>
#include <initializer_list>
#include <glad/glad.h>

#include "object.hpp"
#include "../log.hpp"
#include "../utils.hpp"
#include "../platform.h"

namespace cmw::gl {

struct BufferElement {
    enum Type: uint8_t {
        _1    = CMW_BIT(0), _2   = CMW_BIT(1), _3     = CMW_BIT(2), _4   = CMW_BIT(3),
        _bool = CMW_BIT(4), _Int = CMW_BIT(5), _Float = CMW_BIT(6), _Mat = CMW_BIT(7),
        _Nb = _1 | _2 | _3 | _4, _Type = _bool | _Int | _Float | _Mat,
        Bool  = _bool  | _1,
        Int   = _Int   | _1, Int2   = _Int   | _2, Int3   = _Int   | _3, Int4   = _Int   | _4,
        Float = _Float | _1, Float2 = _Float | _2, Float3 = _Float | _3, Float4 = _Float | _4,
                             Mat2  =  _Mat   | _2, Mat3   = _Mat   | _3, Mat4   = _Mat   | _4,
    };

    GLenum gl_type;
    std::size_t nb, size;
    bool normalized;

    constexpr BufferElement(): gl_type(0), nb(0), size(0), normalized(0) { }
    constexpr BufferElement(Type type, bool normalized = false):
        gl_type(get_gl_type(type)), nb(get_nb(type)), size(get_size(type)), normalized(normalized) { }

    static constexpr std::size_t get_size(Type type) {
        return ((type & Type::_bool) ? 1 : 4) * get_nb(type);
    }

    static constexpr std::size_t get_nb(Type type) {
        if (type & Type::_Mat)
            return (type & Type::_Nb) * (type & Type::_Nb);
        return __builtin_ffs(type & Type::_Nb);
    }

    static constexpr GLenum get_gl_type(Type type) {
        if      (type & Type::_bool)                 return GL_BOOL;
        else if (type & Type::_Int)                  return GL_INT;
        else if (type & (Type::_Float | Type::_Mat)) return GL_FLOAT;
    }
};

struct BufferLayout {
    constexpr BufferLayout(std::initializer_list<BufferElement> &&elements):
        elements(elements), stride(get_stride(std::forward<std::initializer_list<BufferElement>>(elements))) { }

    static constexpr std::size_t get_stride(std::initializer_list<BufferElement> &&elements) {
        std::size_t stride = 0;
        for (auto &element: elements)
            stride += element.size;
        return stride;
    }

    std::initializer_list<BufferElement> elements;
    std::size_t stride;
};

template <GLenum Type, std::size_t N = 1>
class BufferN: public GlObject {
    CMW_NON_COPYABLE(BufferN);
    CMW_NON_MOVEABLE(BufferN);

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

        void set_sub_data(const void *data, std::size_t size, void *off = 0) {
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
            for (auto &element: layout.elements) {
                set_attrib_ptr(i, element.nb, element.gl_type, layout.stride, (GLvoid *)off, element.normalized);
                off += element.size; ++i;
            }
        }

        static void set_attrib_ptr(GLuint pos, GLuint size, GLenum type, GLuint stride = 0, GLvoid *off = nullptr, bool normalize = false) {
            glVertexAttribPointer(pos, size, type, normalize, stride, off);
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

} // namespace cmw
