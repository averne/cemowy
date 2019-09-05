#pragma once

#include <glad/glad.h>
#include <type_traits>
#include <utility>

#include "gl/shader_program.hpp"
#include "elements/line.hpp"
#include "elements/point.hpp"
#include "color.hpp"
#include "mesh.hpp"
#include "resource_manager.hpp"

namespace cmw {

class Renderer {
    public:
        Renderer(ResourceManager &resource_man): resource_man(resource_man) {
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            // glEnable(GL_CULL_FACE);
        };

        ~Renderer() = default;

        inline void clear(int flags) const {
            glClearColor(this->clear_color.r, this->clear_color.g, this->clear_color.b, this->clear_color.a);
            glClear(flags);
        }

        template <typename T>
        void submit(T &&element, const glm::mat4 &model, gl::ShaderProgram &program) const {
            element.on_draw();
            using Type = std::remove_cv_t<std::remove_reference_t<T>>;
            if constexpr (std::is_same_v<Type, elements::Point>)
                render_mesh(GL_POINTS, element.get_mesh(), model, program);
            else if constexpr (std::is_same_v<Type, elements::Line>)
                render_mesh(GL_LINES, element.get_mesh(), model, program);
            else
                render_mesh(GL_TRIANGLES, element.get_mesh(), model, program);
        }

        void render_mesh(GLenum mode, const Mesh &mesh, const glm::mat4 &model, gl::ShaderProgram &program) const;

        inline void set_clear_color(Colorf clear_color) { this->clear_color = clear_color; }
        inline Colorf &get_clear_color() { return this->clear_color; }

        inline void set_view_matrix(glm::mat4 view) { this->view = view; }
        inline void set_proj_matrix(glm::mat4 proj) { this->proj = proj; }

    protected:
        glm::mat4 view{1.0f}, proj{1.0f};
        Colorf clear_color = {0.0f, 0.0f, 0.0f, 1.0f};
        ResourceManager &resource_man;
};

} // namespace cmw
