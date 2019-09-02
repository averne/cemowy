#include <glad/glad.h>

#include <cmw/gl/shader_program.hpp>
#include <cmw/mesh.hpp>

#include <cmw/renderer.hpp>

namespace cmw {

void Renderer::render_mesh(GLenum mode, ShaderProgram &program, const glm::mat4 model, const Mesh &mesh) const {
    program.bind();
    program.set_value("mvp_mat", this->proj * this->view * model);
    mesh.bind();
    glDrawArrays(mode, 0, mesh.get_size());
}

} // namespace cmw
