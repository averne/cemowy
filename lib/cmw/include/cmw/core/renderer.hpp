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

#include "cmw/core/mesh.hpp"
#include "cmw/core/resource_manager.hpp"
#include "cmw/core/text.hpp"
#include "cmw/gl/shader_program.hpp"
#include "cmw/shapes/shape.hpp"
#include "cmw/utils/color.hpp"
#include "cmw/utils/position.hpp"
#include "cmw/widgets/widget.hpp"

namespace cmw {

class Renderer {
    public:
        enum class RenderingMode: std::uint8_t {
            Default,
            AlphaMap,
        };

    public:
        static constexpr std::size_t max_vertices  = 1000;
        static constexpr std::size_t max_indices   = 10000;
        static constexpr std::size_t max_textures  = 30;

    private:
        inline Font *find_font(char16_t chr) {
            for (auto &font: this->resource_man.get_fonts())
                if (font->has_glyph(chr))
                    return font.get();
            CMW_ERROR("Failed to find glyph %c (%#x)\n", chr, chr);
            return nullptr;
        }

    public:
        Renderer(ResourceManager &resource_man);

        inline void clear(int flags) const {
            glClearColor(this->clear_color.r, this->clear_color.g, this->clear_color.b, this->clear_color.a);
            glClear(flags);
        }

        template <typename T>
        void begin(T &&camera, float dt) {
            this->view_proj = &camera.get_view_proj();
            this->dt = dt;
        }

        void end(gl::ShaderProgram &program, GLenum mode = GL_TRIANGLES);
        void end(GLenum mode = GL_TRIANGLES) { end(this->get_default_mesh_shader(), mode); }

        template <typename T>
        inline void submit(T &&element, const glm::mat4 &model, RenderingMode mode = RenderingMode::Default) {
            element.on_draw(*this, this->dt);
            using Type = std::remove_cv_t<std::remove_reference_t<T>>;
            if constexpr (std::is_base_of_v<shapes::Shape, Type>)
                add_mesh(element.get_mesh(), model, mode);
            else if constexpr (std::is_base_of_v<widgets::Widget, Type>)
                element.draw(*this, this->dt);
            else
                throw std::runtime_error("Wrong type for Renderer::submit");
        }

        void add_mesh(Mesh &mesh, const glm::mat4 &model, RenderingMode mode = RenderingMode::Default);

        void draw_glyph(Glyph &glyph, const Position &pos = {0, 0, 0}, float scale = 1.0f,
            const Colorf &color = {1.0f, 1.0f, 1.0f});

        inline void draw_char(Font *font, char16_t chr, const Position &pos = {0, 0, 0}, float scale = 1.0f,
                const Colorf &color = {1.0f, 1.0f, 1.0f}) {
            if (font->has_glyph(chr))
                draw_glyph(font->get_glyph(chr), pos, scale, color);
        }
        inline void draw_char(char16_t chr, const Position &pos = {0, 0, 0}, float scale = 1.0f,
                const Colorf &color = {1.0f, 1.0f, 1.0f}) {
            if (auto *font = find_font(chr); font)
                draw_char(font, chr, pos, scale, color);
        }

        // Uses specified font preferentially and falls back to others otherwise
        void draw_string(Font *font, const std::u16string &str, const Position &pos = {0, 0, 0}, float scale = 1.0f,
            const Colorf &color = {1.0f, 1.0f, 1.0f});
        void draw_string(const std::u16string &str, const Position &pos = {0, 0, 0}, float scale = 1.0f,
            const Colorf &color = {1.0f, 1.0f, 1.0f});

        inline void set_clear_color(Colorf clear_color) { this->clear_color = clear_color; }
        inline       Colorf &get_clear_color()       { return this->clear_color; }
        inline const Colorf &get_clear_color() const { return this->clear_color; }

        inline       gl::ShaderProgram &get_default_mesh_shader()       { return this->mesh_program; }
        inline const gl::ShaderProgram &get_default_mesh_shader() const { return this->mesh_program; }

    protected:
        struct Vertex {
            Mesh::Vertex vertex;
            Colorf blend_color;
            int tex_idx;
            std::uint8_t mode;
        };

        struct Index {
            Mesh::Index index;
            constexpr inline Index(const Mesh::Index &i): index(i) { }
        };

    protected:
        ResourceManager &resource_man;
        gl::ShaderProgram &mesh_program;

        gl::VertexArray   vao;
        gl::VertexBuffer  vbo;
        gl::ElementBuffer ebo;

        Colorf clear_color = {0.0f, 0.0f, 0.0f, 1.0f};
        const glm::mat4 *view_proj;
        float dt;

        std::vector<Vertex>          vertex_buffer;
        std::vector<Index>           index_buffer;
        std::vector<gl::Texture2d *> textures;
};

} // namespace cmw
