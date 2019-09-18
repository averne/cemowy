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

#include <cmw/gl/shader_program.hpp>
#include <cmw/gl/texture.hpp>
#include <cmw/mesh.hpp>

#include <cmw/renderer.hpp>
#include <cmw/color.hpp>
#include <cmw/position.hpp>
#include <cmw/text.hpp>

namespace cmw {

Renderer::Renderer(ResourceManager &resource_man): resource_man(resource_man),
        mesh_program(resource_man.get_shader("shaders/mesh.vert", "shaders/mesh.frag")),
        glyph_program(resource_man.get_shader("shaders/glyph.vert", "shaders/glyph.frag")) {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // glEnable(GL_CULL_FACE);

    bind_all(this->vao, this->vbo, this->ebo);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (const void *)(offsetof(Vertex, vertex) + offsetof(Mesh::Vertex, position)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(Vertex), (const void *)(offsetof(Vertex, vertex) + offsetof(Mesh::Vertex, uv)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 4, GL_FLOAT, false, sizeof(Vertex), (const void *)(offsetof(Vertex, blend_color)));
    glEnableVertexAttribArray(2);
    glVertexAttribIPointer(3, 1, GL_INT, sizeof(Vertex), (const void *)offsetof(Vertex, tex_idx));
    glEnableVertexAttribArray(3);
    glVertexAttribIPointer(4, 1, GL_UNSIGNED_BYTE, sizeof(Vertex), (const void *)offsetof(Vertex, mode));
    glEnableVertexAttribArray(4);
    this->vbo.set_data(nullptr, sizeof(Vertex) * this->max_indices, GL_DYNAMIC_DRAW);
    this->ebo.set_data(nullptr, sizeof(Index)  * this->max_vertices,  GL_DYNAMIC_DRAW);

    this->textures.reserve(this->max_textures);
}

void Renderer::end(gl::ShaderProgram &program, GLenum mode) {
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

    bind_all(this->vao, program);

    for (std::size_t i = 0; i < this->textures.size(); ++i) {
        this->mesh_program.set_value("u_textures[" + std::to_string(i) + "]", (int)i);
        gl::Texture2d::active(i);
        this->textures[i]->bind();
    }

    program.set_value("u_view_proj", *this->view_proj);
    glDrawElements(mode, this->num_indices, GL_UNSIGNED_INT, 0);

    this->textures.clear();
    this->num_vertices = this->num_indices = 0;
}

void Renderer::add_mesh(Mesh &mesh, const glm::mat4 &model, RenderingMode mode) {
    if (this->textures.size() > this->max_textures) // Force flush. We assume vertices and indices will stays in bounds
        end(GL_TRIANGLES);

    int tex_idx;
    auto it = std::find(this->textures.begin(), this->textures.end(), &mesh.get_texture());
    if (it != this->textures.end()) {
        tex_idx = it - this->textures.begin();
    } else {
        this->textures.push_back(&mesh.get_texture());
        tex_idx = this->textures.size() - 1;
    }

    for (const auto &index: mesh.get_indices()) {
        this->index_buffer[this->num_indices].index = (std::uint32_t)(this->num_vertices + index);
        this->num_indices++;
    }

    for (const auto &vertex: mesh.get_vertices()) {
        glm::vec3 position = glm::vec3(model * glm::vec4((glm::vec3)vertex.position, 1.0f)); // Apply model matrix
        this->vertex_buffer[this->num_vertices].vertex       = {position, vertex.uv};
        this->vertex_buffer[this->num_vertices].blend_color  = mesh.get_blend_color();
        this->vertex_buffer[this->num_vertices].tex_idx      = tex_idx;
        this->vertex_buffer[this->num_vertices].mode         = (std::uint8_t)mode;
        this->num_vertices++;
    }
}

void Renderer::draw_string(cmw::Font &font, gl::ShaderProgram &program, const std::u16string &str,
        Position pos, float scale, const Colorf &color) {
    program.bind();
    program.set_value("u_blend_color", color.r, color.g, color.b);
    program.set_value("u_view_proj_mat", *this->view_proj);

    int last_codepoint = -1;
    float xpos = pos.x, ypos = pos.y;
    for (char16_t chr: str) {
        if (chr == u'\n') {
            pos.y -= (font.get_ascender() + font.get_descender() + 40.0f) * scale; // ??? font.linegap == 0
            xpos = pos.x;
            continue;
        }

        auto &glyph = font.get_glyph(chr);

        float w = (float)glyph.get_width() * scale, h = (float)glyph.get_height() * scale;
        xpos += glyph.get_bearing() * scale;
        ypos = pos.y - h - glyph.get_bitmap_top() * scale;

        std::vector<Mesh::Vertex> vertices = {
            { {xpos,     ypos + h, pos.z}, {0.0, 0.0} },
            { {xpos + w, ypos + h, pos.z}, {1.0, 0.0} },
            { {xpos + w, ypos,     pos.z}, {1.0, 1.0} },
            { {xpos,     ypos,     pos.z}, {0.0, 1.0} },
        };

        auto mesh = Mesh(vertices, std::vector<Mesh::Index>{0, 1, 2, 2, 3, 0}, glyph.get_texture(), color);
        add_mesh(mesh, glm::mat4(1.0f), RenderingMode::AlphaMap);

        xpos += glyph.get_advance() * scale;
        if (last_codepoint)
            xpos += stbtt_GetCodepointKernAdvance(font.get_ctx(), last_codepoint, glyph.get_codepoint()) * scale;
        last_codepoint = glyph.get_codepoint();
    }
}

} // namespace cmw
