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

#include <string>
#include <iostream>
#include <algorithm>
#include <initializer_list>
#include <type_traits>
#include <utility>
#include <stdexcept>
#include <map>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "object.hpp"
#include "../position.hpp"
#include "../platform.h"

namespace cmw::gl {

class ShaderProgram: public GlObject {
    public:
        ShaderProgram(): GlObject(glCreateProgram()) {
            CMW_TRACE("Creating shader program object\n");
            CMW_TRY_THROW(get_handle(), std::runtime_error("Could not create shader program object"));
        }

        template <typename ...Shaders>
        ShaderProgram(Shaders &&...shaders): ShaderProgram() {
            attach_shaders(std::forward<Shaders>(shaders)...);
            if (!link()) {
                print_log();
                throw std::runtime_error("Could not link shader program");
            }
            detach_shaders(std::forward<Shaders>(shaders)...);
        }

        ~ShaderProgram() {
            CMW_TRACE("Destructing shader program object\n");
            glDeleteProgram(get_handle());
        }

        template <typename ...Shaders>
        void attach_shaders(Shaders &&...shaders) const {
            (glAttachShader(get_handle(), shaders.get_handle()), ...);
        }

        template <typename ...Shaders>
        void detach_shaders(Shaders &&...shaders) const {
            (glDetachShader(get_handle(), shaders.get_handle()), ...);
        }

        GLint link() const {
            GLint rc;
            glLinkProgram(get_handle());
            glGetProgramiv(get_handle(), GL_LINK_STATUS, &rc);
            if (rc); // delete shaders
            return rc;
        }

        void use() const {
            glUseProgram(get_handle());
        }

        static void unuse() {
            glUseProgram(0);
        }

        inline void bind() const { use(); }
        inline static void unbind() { unuse(); }

        inline GLint get_uniform_loc(const std::string &name) {
            auto it = this->uniform_loc_cache.find(name);
            if (it != this->uniform_loc_cache.end())
                return it->second;
            GLint loc = glGetUniformLocation(get_handle(), name.c_str());
            if (loc == -1)
                CMW_ERROR("Could not find uniform %s\n", name.c_str());
            this->uniform_loc_cache[name] = loc;
            return loc;
        }

        template <typename T>
        void set_value(GLint loc, T &&val) const {
            using Type = std::remove_cv_t<std::remove_reference_t<T>>;
            if constexpr (std::is_same_v<Type, GLboolean> || std::is_same_v<Type, GLint>)
                glUniform1i(loc, (int)val);
            else if constexpr (std::is_same_v<Type, GLfloat>)
                glUniform1f(loc, val);
            else if constexpr (std::is_same_v<Type, glm::vec2> || std::is_same_v<Type, Position2f>)
                glUniform2fv(loc, 1, glm::value_ptr(val));
            else if constexpr (std::is_same_v<Type, glm::vec3> || std::is_same_v<Type, Position3f>)
                glUniform3fv(loc, 1, glm::value_ptr(val));
            else if constexpr (std::is_same_v<Type, glm::vec4> || std::is_same_v<Type, Position4f>)
                glUniform4fv(loc, 1, glm::value_ptr(val));
            else if constexpr (std::is_same_v<Type, glm::mat2>)
                glUniformMatrix2fv(loc, 1, GL_FALSE, glm::value_ptr(val));
            else if constexpr (std::is_same_v<Type, glm::mat3>)
                glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(val));
            else if constexpr (std::is_same_v<Type, glm::mat4>)
                glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(val));
            else
                throw std::invalid_argument("Invalid argument for ShaderProgram::set_value");
        }

        void set_value(GLint loc, float val_1, float val_2) const {
            glUniform2f(loc, val_1, val_2);
        }

        void set_value(GLint loc, float val_1, float val_2, float val_3) const {
            glUniform3f(loc, val_1, val_2, val_3);
        }

        void set_value(GLint loc, float val_1, float val_2, float val_3, float val_4) const {
            glUniform4f(loc, val_1, val_2, val_3, val_4);
        }

        template <typename ...Args>
        void set_value(const std::string &name, Args &&...args) {
            set_value(get_uniform_loc(name), std::forward<Args>(args)...);
        }

        std::string get_log() const {
            std::string str(0x200, 0);
            glGetProgramInfoLog(get_handle(), str.size(), nullptr, (char *)str.data());
            return str;
        }

        void print_log() const {
            CMW_ERROR("%s\n", get_log().c_str());
        }

    private:
        struct Comp {
            bool operator()(const std::string &s1, const std::string &s2) const {
                return strcmp(s1.c_str(), s2.c_str()) < 0;
            }
        };
        std::map<std::string, GLint, Comp> uniform_loc_cache;
};

} // namespace cmw::gl
