#include <string>
#include <map>

#include <cmw/gl/texture.hpp>
#include <cmw/gl/shader.hpp>
#include <cmw/gl/shader_program.hpp>

#include <cmw/resource_manager.hpp>

namespace cmw {

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
