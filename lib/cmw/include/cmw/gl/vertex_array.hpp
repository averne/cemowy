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

#include <glad/glad.h>

#include "cmw/gl/object.hpp"

namespace cmw::gl {

template <std::size_t N = 1>
class VertexArrayN: public GlObject {
    public:
        inline VertexArrayN() {
            CMW_TRACE("Creating vertex array object\n");
            glGenVertexArrays(get_nb(), &this->handle);
            bind();
        }

        inline ~VertexArrayN() {
            CMW_TRACE("Destructing vertex array object\n");
            glDeleteVertexArrays(get_nb(), &this->handle);
        }

        inline void bind() const {
            glBindVertexArray(get_handle());
        }

        static inline void bind(GLuint handle) {
            glBindVertexArray(handle);
        }

        static inline void unbind() {
            glBindVertexArray(0);
        }

        static constexpr inline std::size_t get_nb() { return N; }
};

using VertexArray = VertexArrayN<1>;

} // namespace cmw::gl
