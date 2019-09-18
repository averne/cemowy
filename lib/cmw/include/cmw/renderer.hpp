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

#include <cstdint>
#include <type_traits>
#include <utility>
#include <glad/glad.h>

#include "gl/shader_program.hpp"
#include "shapes/line.hpp"
#include "shapes/point.hpp"
#include "color.hpp"
#include "mesh.hpp"
#include "position.hpp"
#include "resource_manager.hpp"
#include "text.hpp"

namespace cmw {

class Renderer {
    public:
        enum class RenderingMode: std::uint8_t {
            Default,
            AlphaMap,
        };

        struct Vertex {
            Mesh::Vertex vertex;
            Colorf blend_color;
            int tex_idx;
            std::uint8_t mode;
        };

        struct Index {
            Mesh::Index index;
        };

    public:
        static constexpr std::size_t max_vertices  = 1000;
        static constexpr std::size_t max_indices   = 10000;
        static constexpr std::size_t max_textures  = 30;

        Renderer(ResourceManager &resource_man);

        inline void clear(int flags) const {
            glClearColor(this->clear_color.r, this->clear_color.g, this->clear_color.b, this->clear_color.a);
            glClear(flags);
        }

        template <typename T>
        void begin(T &&camera) {
            this->view_proj = &camera.get_view_proj();
            bind_all(this->vbo, this->ebo);
            this->vertex_buffer = (Vertex *)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
            this->index_buffer  = (Index  *)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_READ_ONLY);
        }

        void end(gl::ShaderProgram &program, GLenum mode = GL_TRIANGLES);
        void end(GLenum mode = GL_TRIANGLES) { end(this->get_default_mesh_shader(), mode); }

        template <typename T>
        inline void submit(T &&element, const glm::mat4 &model, RenderingMode mode = RenderingMode::Default) {
            element.on_draw();
            add_mesh(element.get_mesh(), model, mode);
        }

        void add_mesh(Mesh &mesh, const glm::mat4 &model, RenderingMode mode = RenderingMode::Default);

        void draw_string(cmw::Font &font, gl::ShaderProgram &program, const std::u16string &str,
            Position pos = {0, 0, 0}, float scale = 1.0f, const Colorf &color = {1.0f, 1.0f, 1.0f});

        inline void draw_string(cmw::Font &font, const std::u16string &str,
                Position pos = {0, 0, 0}, float scale = 1.0f, const Colorf &color = {1.0f, 1.0f, 1.0f}) {
            draw_string(font, this->glyph_program, str, pos, scale, color);
        }

        inline void set_clear_color(Colorf clear_color) { this->clear_color = clear_color; }
        inline Colorf &get_clear_color() { return this->clear_color; }

        inline gl::ShaderProgram &get_default_mesh_shader()  { return this->mesh_program; }
        inline gl::ShaderProgram &get_default_glyph_shader() { return this->glyph_program; }

    protected:
        ResourceManager &resource_man;
        gl::ShaderProgram &mesh_program;
        gl::ShaderProgram &glyph_program;

        gl::VertexArray   vao;
        gl::VertexBuffer  vbo;
        gl::ElementBuffer ebo;

        GLenum mode;
        Colorf clear_color = {0.0f, 0.0f, 0.0f, 1.0f};
        const glm::mat4 *view_proj;

        Vertex *vertex_buffer;
        Index *index_buffer;
        std::size_t num_vertices = 0, num_indices = 0;
        std::vector<gl::Texture2d *> textures;
};

} // namespace cmw
