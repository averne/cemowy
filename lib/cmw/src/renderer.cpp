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

#include <cstdint>
#include <glad/glad.h>

#include "cmw/core/mesh.hpp"
#include "cmw/core/text.hpp"
#include "cmw/gl/shader_program.hpp"
#include "cmw/gl/texture.hpp"
#include "cmw/utils/color.hpp"
#include "cmw/utils/position.hpp"

#include "cmw/core/renderer.hpp"

namespace cmw {

Renderer::Renderer(ResourceManager &resource_man): resource_man(resource_man),
        mesh_program(resource_man.get_shader("shaders/mesh.vert", "shaders/mesh.frag")) {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // glEnable(GL_CULL_FACE);

    glEnable(GL_MULTISAMPLE);

    bind_all(this->vao, this->vbo, this->ebo);
    this->vbo.set_layout({
        gl::BufferElement::Float3,
        gl::BufferElement::Float2,
        gl::BufferElement::Float4,
        gl::BufferElement::Int,
        gl::BufferElement::Uint, // Ubyte padded to 4 bytes
    });
    this->vbo.set_data(nullptr, sizeof(Vertex) * this->max_vertices, GL_DYNAMIC_DRAW);
    this->ebo.set_data(nullptr, sizeof(Index)  * this->max_indices,  GL_DYNAMIC_DRAW);

    this->textures.reserve(this->max_textures);
}

void Renderer::end(gl::ShaderProgram &program, GLenum mode) {
    bind_all(this->vao, this->ebo, this->vbo, program);
    this->vbo.set_sub_data(this->vertex_buffer.data(), this->vertex_buffer.size() * sizeof(Vertex));
    this->ebo.set_sub_data(this->index_buffer.data(),  this->index_buffer.size()  * sizeof(Index));

    for (std::size_t i = 0; i < this->textures.size(); ++i) {
        this->mesh_program.set_value("u_textures[" + std::to_string(i) + "]", (int)i);
        gl::Texture2d::active(i);
        this->textures[i]->bind();
    }

    program.set_value("u_view_proj", *this->view_proj);
    glDrawElements(mode, this->index_buffer.size(), GL_UNSIGNED_INT, 0);

    this->vertex_buffer.clear();
    this->index_buffer.clear();
    this->textures.clear();
}

void Renderer::add_mesh(Mesh &mesh, const glm::mat4 &model, RenderingMode mode) {
    if ((this->textures.size() >= this->max_textures)
            || (this->vertex_buffer.size() >= this->max_vertices)
            || (this->index_buffer.size() >= this->max_indices)) // Don't accept new data
        return;

    int tex_idx;
    auto it = std::find(this->textures.begin(), this->textures.end(), &mesh.get_texture());
    if (it != this->textures.end()) {
        tex_idx = it - this->textures.begin();
    } else {
        this->textures.push_back(&mesh.get_texture());
        tex_idx = this->textures.size() - 1;
    }

    const auto &vertices = mesh.get_vertices();
    const auto &indices  = mesh.get_indices();
    auto vbo_sz = this->vertex_buffer.size();
    auto ebo_sz = this->index_buffer.size();

    this->index_buffer.reserve(ebo_sz + indices.size());
    for (const auto &index: indices)
        this->index_buffer.emplace_back(vbo_sz + index);

    this->vertex_buffer.reserve(vbo_sz + vertices.size());
    for (const auto &vertex: vertices) {
        glm::vec3 position = glm::vec3(model * glm::vec4((glm::vec3)vertex.position, 1.0f)); // Apply model matrix
        Vertex vert{Mesh::Vertex{position, vertex.uv}, mesh.get_blend_color(), tex_idx, 0};
        vert.mode = (std::uint8_t)mode; // FIXME: for some reason the mode is not correctly set when using emplace_back, or when directly passing it to the constructor
        this->vertex_buffer.push_back(vert);
    }
}

void Renderer::draw_glyph(Glyph &glyph, const Position &pos, float scale, const Colorf &color) {
        float chr_w = (float)glyph.get_width() * scale, chr_h = (float)glyph.get_height() * scale;
        float chr_x = pos.x + glyph.get_bearing() * scale;
        float chr_y = pos.y - chr_h - glyph.get_bitmap_top() * scale;

        std::vector<Mesh::Vertex> vertices = {
            { {chr_x,         chr_y + chr_h, pos.z}, {0.0, 0.0} },
            { {chr_x + chr_w, chr_y + chr_h, pos.z}, {1.0, 0.0} },
            { {chr_x + chr_w, chr_y,         pos.z}, {1.0, 1.0} },
            { {chr_x,         chr_y,         pos.z}, {0.0, 1.0} },
        };

        auto mesh = Mesh(vertices, std::vector<Mesh::Index>{0, 1, 2, 2, 3, 0}, glyph.get_texture(), color);
        add_mesh(mesh, glm::mat4(1.0f), RenderingMode::AlphaMap);
}

void Renderer::draw_string(const std::u16string &str, const Position &pos, float scale, const Colorf &color) {
    int last_codepoint = 0;
    Position cur_pos = pos;
    Font *font = nullptr;
    int ascender = 0, descender = 0;

    for (char16_t chr: str) {
        if (chr == u'\n') {
            cur_pos.y -= (ascender + descender + 40.0f) * scale; // ??? font->linegap == 0
            cur_pos.x = pos.x;
            continue;
        }

        if (!(font = find_font(chr)))
            continue;

        ascender  = std::max(ascender,  font->get_ascender());
        descender = std::max(descender, font->get_descender());

        auto &glyph = font->get_glyph(chr);
        draw_glyph(glyph, cur_pos, scale, color);

        cur_pos.x += glyph.get_advance() * scale;
        if (last_codepoint)
            cur_pos.x += font->get_kerning(last_codepoint, glyph.get_codepoint()) * scale;
        last_codepoint = glyph.get_codepoint();
    }
}

} // namespace cmw
