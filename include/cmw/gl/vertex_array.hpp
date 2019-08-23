#pragma once

#include <glad/glad.h>

#include "object.hpp"

namespace cmw {

template <std::size_t N = 1>
class VertexArrayN: public GlObject {
    public:
        VertexArrayN() {
            CMW_TRACE("Creating vertex array object\n");
            glGenVertexArrays(get_nb(), &this->handle);
            bind();
        }

        ~VertexArrayN() {
            CMW_TRACE("Destructing vertex array object\n");
            glDeleteVertexArrays(get_nb(), &this->handle);
        }

        void bind() const {
            glBindVertexArray(get_handle());
        }

        static void bind(GLuint handle) {
            glBindVertexArray(handle);
        }

        static void unbind() {
            glBindVertexArray(0);
        }

        static inline std::size_t get_nb() { return N; }
};

using VertexArray = VertexArrayN<1>;

} // namespace cmw
