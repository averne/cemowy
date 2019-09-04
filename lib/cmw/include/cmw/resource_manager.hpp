#pragma once

#include <cstdio>
#include <string>
#include <map>

#include "gl/shader_program.hpp"
#include "gl/texture.hpp"
#include "log.hpp"

namespace cmw {

class ResourceManager {
    public:
        static inline std::string WhiteTexture = "";

        ResourceManager() {
            auto pair = this->textures.try_emplace(WhiteTexture);
            this->white_texture = &pair.first->second;
            this->white_texture->set_blank_data(10, 10);
            this->white_texture->generate_mipmap();
        }

        ~ResourceManager() = default;

        gl::Texture2d &get_texture(const std::string &path) {
            auto it = this->textures.find(path);
            if (it != this->textures.end())
                return it->second;
            auto pair = this->textures.try_emplace(path, path);
            return pair.first->second;
        }

        gl::Texture2d &get_white_texture() const { return *this->white_texture; }

        gl::ShaderProgram &get_shader(const std::string &vert_path, const std::string &frag_path) {
            std::string key = vert_path + frag_path;
            auto it = this->shader_programs.find(key);
            if (it != this->shader_programs.end())
                return it->second;
            auto pair = this->shader_programs.try_emplace(key, gl::VertexShader{vert_path}, gl::FragmentShader{frag_path});
            return pair.first->second;
        }

    private:
        gl::Texture2d *white_texture;
        std::map<std::string, gl::Texture2d> textures;
        std::map<std::string, gl::ShaderProgram> shader_programs;
};

} // namespace cmw
