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
#include "cmw/utils/position.hpp"

namespace cmw::shapes {

class Point: public Shape {
    public:
        Point(gl::Texture2d &texture, Colorf color = colors::White, GLfloat width = 1.0f): Shape(texture, color), width(width) { }

        Point(const Position &position, gl::Texture2d &texture, Colorf color = colors::White, GLfloat width = 1.0f):
                Point(texture, color, width) {
            set_position(position);
        }

        void set_position(const Position &position) {
            this->get_mesh().set_data({(Mesh::Vertex){position}});
        }

        virtual void on_draw(Renderer &renderer, float dt) override {
            glPointSize(this->width);
        }

        inline GLfloat get_width() const { return this->width; }
        inline void set_width(GLfloat width) { this->width = width; }

    private:
        GLfloat width;
};

} // namespace cmw::shapes
