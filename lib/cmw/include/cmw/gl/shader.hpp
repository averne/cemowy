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

#include <cstdio>
#include <string>
#include <iostream>
#include <stdexcept>
#include <glad/glad.h>

#include "cmw/core/log.hpp"
#include "cmw/core/resource_manager.hpp"
#include "cmw/gl/object.hpp"
#include "cmw/utils.hpp"

namespace cmw::gl {

template <GLenum Type>
class Shader: public GlObject {
    public:
        Shader(): GlObject(glCreateShader(get_type())) {
            CMW_TRACE("Creating shader object\n");
            CMW_TRY_THROW(get_handle(), std::runtime_error("Could not create shader object"));
        }

        Shader(const std::string &path): Shader() {
            auto src = ResourceManager::read_asset<std::string>(path);
            set_source(src);
            if (!compile()) {
                print_log();
                throw std::runtime_error("Could not compile shader");
            }
        }

        ~Shader() {
            CMW_TRACE("Destructing shader object\n");
            glDeleteShader(get_handle());
        }

        void set_source(const std::string &src) const {
            const char *dat = src.c_str();
            glShaderSource(get_handle(), 1, &dat, NULL);
        }

        GLint compile() const {
            GLint rc;
            glCompileShader(get_handle());
            glGetShaderiv(get_handle(), GL_COMPILE_STATUS, &rc);
            return rc;
        }

        std::string get_log() const {
            std::string str(0x200, 0);
            glGetShaderInfoLog(get_handle(), str.size(), nullptr, (char *)str.data());
            return str;
        }

        void print_log() const {
            CMW_ERROR("Failed to compile shader:\n%s", get_log().c_str());
        }

        static constexpr inline GLenum get_type() { return Type; }
};

class VertexShader: public Shader<GL_VERTEX_SHADER> {
    public:
        VertexShader() = default;
        VertexShader(const std::string &src): Shader(src) { }
};

class FragmentShader: public Shader<GL_FRAGMENT_SHADER> {
    public:
        FragmentShader() = default;
        FragmentShader(const std::string &src): Shader(src) { }
};

} // namespace cmw::gl
