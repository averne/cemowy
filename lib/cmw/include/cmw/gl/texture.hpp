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

#include <stdexcept>
#include <algorithm>
#include <utility>
#include <glad/glad.h>
#include <stb_image.h>

#include "cmw/core/log.hpp"
#include "cmw/gl/object.hpp"
#include "cmw/utils.hpp"

namespace cmw::gl {

template <GLenum Type, std::size_t N = 1>
class TextureN: public GlObject {
    public:
        inline TextureN() {
            CMW_TRACE("Creating texture object\n");
            glGenTextures(get_nb(), &this->handle);
            CMW_TRY_THROW(get_handle(), std::runtime_error("Could not create texture object"));
            bind();
        }

        inline TextureN(int idx) {
            CMW_TRACE("Creating texture object\n");
            if (idx >= 0) this->active(idx);
            glGenTextures(get_nb(), &this->handle);
            CMW_TRY_THROW(get_handle(), std::runtime_error("Could not create texture object"));
            bind();
        }

        inline ~TextureN() {
            CMW_TRACE("Destructing texture object\n");
            glDeleteTextures(get_nb(), &this->handle);
        }

        static inline void active(GLuint idx) {
            glActiveTexture(GL_TEXTURE0 + idx);
        }

        static inline void deactive(GLuint idx) {
            glActiveTexture(GL_TEXTURE0);
        }

        static inline void generate_mipmap() {
            glGenerateMipmap(get_type());
        }

        template <typename ...Params>
        static inline void set_parameters(Params &&...params) {
            (glTexParameteri(get_type(), params.first, params.second), ...);
        }

        static inline void set_default_parameters() {
            set_parameters(std::pair{GL_TEXTURE_WRAP_S, GL_REPEAT}, std::pair{GL_TEXTURE_WRAP_T, GL_REPEAT},
                std::pair{GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR}, std::pair{GL_TEXTURE_MAG_FILTER, GL_LINEAR});
        }

        void inline bind() const {
            glBindTexture(get_type(), get_handle());
        }

        static inline void bind(GLuint handle) {
            glBindTexture(get_type(), handle);
        }

        static inline void unbind() {
            glBindTexture(get_type(), 0);
        }

        static constexpr inline std::size_t get_nb()   { return N; }
        static constexpr inline GLenum      get_type() { return Type; }
};

template <std::size_t N = 1>
class Texture1dN: public TextureN<GL_TEXTURE_1D, N> {
    public:
        inline Texture1dN() = default;
        inline Texture1dN(int idx): TextureN<GL_TEXTURE_1D, N>(idx) { }

        inline void set_data(void *data, GLuint width, GLuint mipmap_lvl = 0,
                GLenum store_fmt = GL_RGB, GLenum load_fmt = GL_RGB, GLenum load_data_fmt = GL_UNSIGNED_BYTE, GLuint leg = 0) {
            glTexImage1D(this->get_type(), mipmap_lvl, store_fmt, width, leg, load_fmt, load_data_fmt, data);
        }
};

template <std::size_t N = 1>
class Texture2dN: public TextureN<GL_TEXTURE_2D, N> {
    public:
        inline Texture2dN() = default;
        inline Texture2dN(int idx): TextureN<GL_TEXTURE_2D, N>(idx) { }

        Texture2dN(const std::string &path, GLint idx = -1,
                GLenum load_data_fmt = GL_UNSIGNED_BYTE, GLuint mipmap_lvl = 0): Texture2dN(idx) {
            int w, h, nchan, fmt;
            stbi_set_flip_vertically_on_load(true);
            stbi_uc *data = stbi_load_from_file(open_asset(path), &w, &h, &nchan, 0);
            if (!data) {
                CMW_ERROR("%s", stbi_failure_reason());
                throw std::runtime_error("Could not load texture file");
            }

            if      (nchan == 3) fmt = GL_RGB;
            else if (nchan == 4) fmt = GL_RGBA;
            else                 fmt = GL_RED;
            this->set_data(data, w, h, fmt, fmt, load_data_fmt, mipmap_lvl);
            this->generate_mipmap();
            this->set_default_parameters();

            stbi_image_free(data);
        }

        inline void set_data(void *data, GLuint width, GLuint height, GLenum store_fmt = GL_RGB, GLenum load_fmt = GL_RGB,
                GLenum load_data_fmt = GL_UNSIGNED_BYTE, GLuint mipmap_lvl = 0, GLuint leg = 0) {
            glTexImage2D(this->get_type(), mipmap_lvl, store_fmt, width, height, leg, load_fmt, load_data_fmt, data);
        }

        inline void set_blank_data(GLuint width, GLint height, GLenum store_fmt = GL_RGB, GLenum load_fmt = GL_RGB,
                GLenum load_data_fmt = GL_UNSIGNED_BYTE, GLuint mipmap_lvl = 0, GLuint leg = 0) {
            std::vector<uint8_t> blank_data(width * height * 4, 255);
            set_data(blank_data.data(), width, height, store_fmt, load_fmt, load_data_fmt, mipmap_lvl, leg);
        }
};

template <std::size_t N = 1>
class Texture3dN: public TextureN<GL_TEXTURE_3D, N> {
    public:
        inline Texture3dN() = default;
        inline Texture3dN(int idx): TextureN<GL_TEXTURE_3D, N>(idx) { }

        inline void set_data(void *data, GLuint width, GLuint height, GLuint depth, GLuint mipmap_lvl = 0,
                GLenum store_fmt = GL_RGB, GLenum load_fmt = GL_RGB, GLenum load_data_fmt = GL_UNSIGNED_BYTE, GLuint leg = 0) {
            glTexImage3D(this->get_type(), mipmap_lvl, store_fmt, width, height, depth, leg, load_fmt, load_data_fmt, data);
        }
};

template <GLenum Type>
using Texture   = TextureN<Type, 1>;
using Texture1d = Texture1dN<1>;
using Texture2d = Texture2dN<1>;
using Texture3d = Texture3dN<1>;

} // namespace cmw::gl
