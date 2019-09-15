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

#include <glad/glad.h>

#include <cmw/gl/shader_program.hpp>
#include <cmw/gl/texture.hpp>
#include <cmw/mesh.hpp>

#include <cmw/renderer.hpp>
#include <cmw/color.hpp>
#include <cmw/text.hpp>

namespace cmw {

void Renderer::render_mesh(GLenum mode, const Mesh &mesh, const glm::mat4 &model, gl::ShaderProgram &program) const {
    program.bind();
    program.set_value("u_mvp_mat", *this->view_proj * model);
    program.set_value("u_texture", 0);
    program.set_value("u_blend_color", mesh.get_blend_color().r, mesh.get_blend_color().g, mesh.get_blend_color().b, mesh.get_blend_color().a);
    gl::Texture2d::active(0);
    mesh.bind();
    if (mesh.is_indexed())
        glDrawElements(mode, mesh.get_size(), GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(mode, 0, mesh.get_size());
}

void Renderer::draw_string(cmw::Font &font, gl::ShaderProgram &program, const std::u16string &str,
        float x, float y, float z, float scale, Colorf color) {
    program.bind();
    program.set_value("u_blend_color", color.r, color.g, color.b);
    program.set_value("u_view_proj_mat", *this->view_proj);

    font.bind();
    gl::Texture2d::active(0);

    int last_codepoint = -1;
    float xpos = x, ypos = y;
    for (char16_t chr: str) {
        if (chr == u'\n') {
            y -= (font.get_ascender() + font.get_descender() + 40.0f) * scale; // ??? font.linegap == 0
            xpos = x;
            continue;
        }

        const auto &glyph = font.get_glyph(chr);
        glyph.bind();

        float w = (float)glyph.get_width() * scale, h = (float)glyph.get_height() * scale;
        xpos += glyph.get_bearing() * scale;
        ypos = y - h - glyph.get_bitmap_top() * scale;

        GLfloat vertices[6][5] = {
            { xpos,     ypos + h, z, 0.0, 0.0 },
            { xpos,     ypos,     z, 0.0, 1.0 },
            { xpos + w, ypos,     z, 1.0, 1.0 },
            { xpos,     ypos + h, z, 0.0, 0.0 },
            { xpos + w, ypos,     z, 1.0, 1.0 },
            { xpos + w, ypos + h, z, 1.0, 0.0 },
        };

        font.set_vertex_data(vertices, sizeof(vertices));
        glDrawArrays(GL_TRIANGLES, 0, 6);

        xpos += glyph.get_advance() * scale;
        if (last_codepoint)
            xpos += stbtt_GetCodepointKernAdvance(font.get_ctx(), last_codepoint, glyph.get_codepoint()) * scale;
        last_codepoint = glyph.get_codepoint();
    }
}

} // namespace cmw
