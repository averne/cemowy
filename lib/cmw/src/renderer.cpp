#include <glad/glad.h>

#include <cmw/gl/shader_program.hpp>
#include <cmw/gl/texture.hpp>
#include <cmw/mesh.hpp>

#include <cmw/renderer.hpp>
#include <cmw/color.hpp>

namespace cmw {

void Renderer::render_mesh(GLenum mode, gl::ShaderProgram &program, const glm::mat4 model, const Mesh &mesh, Colorf color) const {
    program.bind();
    program.set_value("u_mvp_mat", this->proj * this->view * model);
    program.set_value("u_texture", 0);
    program.set_value("u_blend_color", color.r, color.g, color.b, color.a);
    gl::Texture2d::active(0);
    this->resource_man.get_white_texture().bind();
    mesh.bind();
    glDrawArrays(mode, 0, mesh.get_size());
}

} // namespace cmw
