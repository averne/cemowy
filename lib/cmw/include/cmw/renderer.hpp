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
#include "text.hpp"

namespace cmw {

class Renderer {
    public:
        Renderer(ResourceManager &resource_man): resource_man(resource_man),
                mesh_program(resource_man.get_shader("shaders/mesh.vert", "shaders/mesh.frag")),
                glyph_program(resource_man.get_shader("shaders/glyph.vert", "shaders/glyph.frag")) {
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            // glEnable(GL_CULL_FACE);
        };

        inline void clear(int flags) const {
            glClearColor(this->clear_color.r, this->clear_color.g, this->clear_color.b, this->clear_color.a);
            glClear(flags);
        }

        template <typename T>
        void begin_scene(T &&camera) {
            this->view_proj = &camera.get_view_proj();
        }

        void end_scene() { }

        template <typename T>
        inline void submit(T &&element, const glm::mat4 &model, gl::ShaderProgram &program) const {
            element.on_draw();
            using Type = std::remove_cv_t<std::remove_reference_t<T>>;
            if constexpr (std::is_same_v<Type, shapes::Point>)
                render_mesh(GL_POINTS, element.get_mesh(), model, program);
            else if constexpr (std::is_same_v<Type, shapes::Line>)
                render_mesh(GL_LINES, element.get_mesh(), model, program);
            else
                render_mesh(GL_TRIANGLES, element.get_mesh(), model, program);
        }

        template <typename T>
        inline void submit(T &&element, const glm::mat4 &model) const {
            submit(std::forward<T>(element), model, this->mesh_program);
        }

        void render_mesh(GLenum mode, const Mesh &mesh, const glm::mat4 &model, gl::ShaderProgram &program) const;

        void draw_string(cmw::Font &font, gl::ShaderProgram &program, const std::u16string &str,
            float x = 0.0f, float y = 0.0f, float z = 0.0f, float scale = 1.0f, Colorf color = {1.0f, 1.0f, 1.0f});

        inline void draw_string(cmw::Font &font, const std::u16string &str,
                float x = 0.0f, float y = 0.0f, float z = 0.0f, float scale = 1.0f, Colorf color = {1.0f, 1.0f, 1.0f}) {
            draw_string(font, this->glyph_program, str, x, y, z, scale, color);
        }

        inline void set_clear_color(Colorf clear_color) { this->clear_color = clear_color; }
        inline Colorf &get_clear_color() { return this->clear_color; }

        inline gl::ShaderProgram &get_default_mesh_shader()  { return this->mesh_program; }
        inline gl::ShaderProgram &get_default_glyph_shader() { return this->glyph_program; }

    protected:
        ResourceManager &resource_man;
        gl::ShaderProgram &mesh_program;
        gl::ShaderProgram &glyph_program;

        Colorf clear_color = {0.0f, 0.0f, 0.0f, 1.0f};
        const glm::mat4 *view_proj;
};

} // namespace cmw
