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

#include "cmw/gl/texture.hpp"
#include "cmw/gl/shader_program.hpp"

#include "cmw/core/resource_manager.hpp"

namespace cmw {

ResourceManager::ResourceManager() {
    auto pair = this->textures.try_emplace(WhiteTexture);
    this->white_texture = &pair.first->second;
    this->white_texture->set_blank_data(10, 10);
    this->white_texture->generate_mipmap();
}

gl::Texture2d &ResourceManager::get_texture(const std::string &path) {
    auto it = this->textures.find(path);
    if (it != this->textures.end())
        return it->second;
    auto pair = this->textures.try_emplace(path, path);
    return pair.first->second;
}

gl::ShaderProgram &ResourceManager::get_shader(const std::string &vert_path, const std::string &frag_path) {
    std::string key = vert_path + frag_path;
    auto it = this->shader_programs.find(key);
    if (it != this->shader_programs.end())
        return it->second;
    auto pair = this->shader_programs.try_emplace(key, gl::VertexShader{vert_path}, gl::FragmentShader{frag_path});
    return pair.first->second;
}

} // namespace cmw
