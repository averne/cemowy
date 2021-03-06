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

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "cmw/core/mesh.hpp"
#include "cmw/gl/texture.hpp"
#include "cmw/shapes/shape.hpp"
#include "cmw/utils/color.hpp"

namespace cmw::shapes {

class Line: public Shape {
    public:
        Line(gl::Texture2d &texture, Colorf color = colors::White, GLfloat width = 1.0f);
        Line(const std::vector<Position> &positions, gl::Texture2d &texture, Colorf color = colors::White, GLfloat width = 1.0f);

        template <typename ...Args>
        void add_points(Args &&...positions) {
            auto &vertices = this->mesh.get_vertices();
            auto &indices  = this->mesh.get_indices();
            Mesh::Index idx = vertices.size();

            vertices.reserve(sizeof...(Args));
            (vertices.emplace_back(positions), ...);

            indices.reserve(2 * sizeof...(Args));
            indices.emplace_back(idx - 1);
            for (; idx < vertices.size() - 1; ++idx)
                indices.emplace_back(idx), indices.emplace_back(idx);
            indices.emplace_back(idx);
        }

        void edit_point(std::size_t idx, const Position &position) {
            this->get_mesh().get_vertices()[idx].position = position;
        }

        virtual void on_draw(Renderer &renderer, float dt) override {
            glLineWidth(this->width); // Doesn't seem to work on Switch
        }

        inline GLfloat get_width() const { return this->width; }
        inline void set_width(GLfloat width) { this->width = width; }

    protected:
        GLfloat width;
};

} // namespace cmw::shapes
