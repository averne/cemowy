#pragma once

#include <glad/glad.h>
#include <type_traits>
#include <utility>

#include "gl/shader_program.hpp"
#include "shapes/line.hpp"
#include "shapes/point.hpp"
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
        void begin_scene(T &&camera) {
            this->view_proj = const_cast<glm::mat4 *>(&camera.get_view_proj());
        }

        void end_scene() { }

        template <typename T>
        void submit(T &&element, const glm::mat4 &model, gl::ShaderProgram &program) const {
            element.on_draw();
            using Type = std::remove_cv_t<std::remove_reference_t<T>>;
            if constexpr (std::is_same_v<Type, shapes::Point>)
                render_mesh(GL_POINTS, element.get_mesh(), model, program);
            else if constexpr (std::is_same_v<Type, shapes::Line>)
                render_mesh(GL_LINES, element.get_mesh(), model, program);
            else
                render_mesh(GL_TRIANGLES, element.get_mesh(), model, program);
        }

        void render_mesh(GLenum mode, const Mesh &mesh, const glm::mat4 &model, gl::ShaderProgram &program) const;

        inline void set_clear_color(Colorf clear_color) { this->clear_color = clear_color; }
        inline Colorf &get_clear_color() { return this->clear_color; }

    protected:
        glm::mat4 *view_proj;
        Colorf clear_color = {0.0f, 0.0f, 0.0f, 1.0f};
        ResourceManager &resource_man;
};

} // namespace cmw
