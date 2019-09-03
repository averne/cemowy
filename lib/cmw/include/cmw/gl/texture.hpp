#pragma once

#include <stdexcept>
#include <algorithm>
#include <utility>
#include <glad/glad.h>
#include <stb_image.h>

#include "object.hpp"
#include "../log.hpp"
#include "../utils.hpp"
#include "../platform.h"

namespace cmw {

enum class TextureType {
    Diffuse,
    Specular,
    Normal,
};

template <GLenum Type, std::size_t N = 1>
class TextureN: public GlObject {
    CMW_NON_COPYABLE(TextureN);
    CMW_NON_MOVEABLE(TextureN);

    public:
        TextureN() {
            CMW_TRACE("Creating texture object\n");
            glGenTextures(get_nb(), &this->handle);
            CMW_TRY_THROW(get_handle(), std::runtime_error("Could not create texture object"));
            bind();
        }

        TextureN(int idx) {
            CMW_TRACE("Creating texture object\n");
            if (idx >= 0) this->active(idx);
            glGenTextures(get_nb(), &this->handle);
            CMW_TRY_THROW(get_handle(), std::runtime_error("Could not create texture object"));
            bind();
        }

        ~TextureN() {
            CMW_TRACE("Destructing texture object\n");
            glDeleteTextures(get_nb(), &this->handle);
        }

        static void active(GLuint idx) {
            glActiveTexture(GL_TEXTURE0 + idx);
        }

        static void deactive(GLuint idx) {
            glActiveTexture(GL_TEXTURE0);
        }

        static void generate_mipmap() {
            glGenerateMipmap(get_type());
        }

        template <typename ...Params>
        static void set_parameters(Params &&...params) {
            (glTexParameteri(get_type(), params.first, params.second), ...);
        }

        static void set_default_parameters() {
            set_parameters(std::pair{GL_TEXTURE_WRAP_S, GL_REPEAT}, std::pair{GL_TEXTURE_WRAP_T, GL_REPEAT},
                std::pair{GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR}, std::pair{GL_TEXTURE_MAG_FILTER, GL_LINEAR});
        }

        void bind() const {
            glBindTexture(get_type(), get_handle());
        }

        static void bind(GLuint handle) {
            glBindTexture(get_type(), handle);
        }

        static void unbind() {
            glBindTexture(get_type(), 0);
        }

        static inline std::size_t get_nb()   { return N; }
        static inline GLenum      get_type() { return Type; }
};

template <std::size_t N = 1>
class Texture1dN: public TextureN<GL_TEXTURE_1D, N> {
    public:
        Texture1dN() = default;
        Texture1dN(int idx): TextureN<GL_TEXTURE_1D, N>(idx) { }

        void set_data(void *data, GLuint width, GLuint mipmap_lvl = 0,
                GLenum store_fmt = GL_RGB, GLenum load_fmt = GL_RGB, GLenum load_data_fmt = GL_UNSIGNED_BYTE, GLuint leg = 0) {
            glTexImage1D(this->get_type(), mipmap_lvl, store_fmt, width, leg, load_fmt, load_data_fmt, data);
        }
};

template <std::size_t N = 1>
class Texture2dN: public TextureN<GL_TEXTURE_2D, N> {
    public:
        Texture2dN() = default;
        Texture2dN(int idx): TextureN<GL_TEXTURE_2D, N>(idx) { }

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

        void set_data(void *data, GLuint width, GLuint height, GLenum store_fmt = GL_RGB, GLenum load_fmt = GL_RGB,
                GLenum load_data_fmt = GL_UNSIGNED_BYTE, GLuint mipmap_lvl = 0, GLuint leg = 0) {
            glTexImage2D(this->get_type(), mipmap_lvl, store_fmt, width, height, leg, load_fmt, load_data_fmt, data);
        }

        void set_blank_data(GLuint width, GLint height, GLenum store_fmt = GL_RGB, GLenum load_fmt = GL_RGB,
                GLenum load_data_fmt = GL_UNSIGNED_BYTE, GLuint mipmap_lvl = 0, GLuint leg = 0) {
            std::vector<uint8_t> blank_data(width * height * 3, 255);
            set_data(blank_data.data(), width, height, store_fmt, load_fmt, load_data_fmt, mipmap_lvl, leg);
        }
};

template <std::size_t N = 1>
class Texture3dN: public TextureN<GL_TEXTURE_3D, N> {
    public:
        Texture3dN() = default;
        Texture3dN(int idx): TextureN<GL_TEXTURE_3D, N>(idx) { }

        void set_data(void *data, GLuint width, GLuint height, GLuint depth, GLuint mipmap_lvl = 0,
                GLenum store_fmt = GL_RGB, GLenum load_fmt = GL_RGB, GLenum load_data_fmt = GL_UNSIGNED_BYTE, GLuint leg = 0) {
            glTexImage3D(this->get_type(), mipmap_lvl, store_fmt, width, height, depth, leg, load_fmt, load_data_fmt, data);
        }
};

template <GLenum Type>
using Texture   = TextureN<Type, 1>;
using Texture1d = Texture1dN<1>;
using Texture2d = Texture2dN<1>;
using Texture3d = Texture3dN<1>;

} // namespace cmw
