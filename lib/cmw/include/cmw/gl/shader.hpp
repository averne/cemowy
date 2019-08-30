#pragma once

#include <cstdio>
#include <string>
#include <iostream>
#include <stdexcept>
#include <glad/glad.h>

#include "object.hpp"
#include "../log.hpp"
#include "../utils.hpp"
#include "../platform.h"

namespace cmw {

template <GLenum Type>
class Shader: public GlObject {
    CMW_NON_COPYABLE(Shader);
    CMW_NON_MOVEABLE(Shader);

    public:
        Shader(): GlObject(glCreateShader(get_type())) {
            CMW_TRACE("Creating shader object\n");
            CMW_TRY_THROW(get_handle(), std::runtime_error("Could not create shader object"));
        }

        Shader(const std::string &path): Shader() {
            FILE *fp = open_asset(path);
            CMW_TRY_THROW(fp, std::runtime_error("Could not open shader file"));
            fseek(fp, 0, SEEK_END);
            std::size_t size = ftell(fp);
            fseek(fp, 0, SEEK_SET);
            std::string src(size + 1, '\0');
            fread(src.data(), size, 1, fp);
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

} // namespace cmw
