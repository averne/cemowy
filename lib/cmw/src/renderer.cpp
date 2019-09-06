#include <glad/glad.h>

#include <cmw/gl/shader_program.hpp>
#include <cmw/gl/texture.hpp>
#include <cmw/mesh.hpp>

#include <cmw/renderer.hpp>
#include <cmw/color.hpp>

namespace cmw {

void Renderer::render_mesh(GLenum mode, const Mesh &mesh, const glm::mat4 &model, gl::ShaderProgram &program) const {
    program.bind();
    program.set_value("u_mvp_mat", *this->view_proj * model);
    program.set_value("u_texture", 0);
    program.set_value("u_blend_color", mesh.get_blend_color().r, mesh.get_blend_color().g, mesh.get_blend_color().b, mesh.get_blend_color().a);
    gl::Texture2d::active(0);
    mesh.bind();
    glDrawArrays(mode, 0, mesh.get_size());
}

} // namespace cmw
