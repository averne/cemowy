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
#include <map>

#include "cmw/core/log.hpp"
#include "cmw/gl/shader_program.hpp"
#include "cmw/gl/texture.hpp"

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
