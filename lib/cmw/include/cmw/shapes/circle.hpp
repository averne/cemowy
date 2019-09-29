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
#include <glm/glm.hpp>

#include "cmw/core/mesh.hpp"
#include "cmw/gl/texture.hpp"
#include "cmw/shapes/shape.hpp"
#include "cmw/utils/color.hpp"
#include "cmw/utils/position.hpp"

namespace cmw::shapes {

class Circle: public Shape {
    public:
        Circle(gl::Texture2d &texture, Colorf color = colors::White);
        Circle(const std::vector<Mesh::Vertex> &vertices, const std::vector<Mesh::Index> &indices,
            gl::Texture2d &texture, Colorf color = colors::White);
        Circle(const Position &position, float radius, gl::Texture2d &texture, Colorf color = colors::White, 
            std::size_t segments = 24);

        Circle(Colorf color = colors::White):
            Circle(Application::get_instance().get_resource_manager().get_white_texture(), color) { }
        Circle(const Position &position, float radius, Colorf color = colors::White, std::size_t segments = 24):
            Circle(position, radius, Application::get_instance().get_resource_manager().get_white_texture(), color, segments) { }
        Circle(const std::vector<Mesh::Vertex> &vertices, const std::vector<Mesh::Index> &indices, Colorf color = colors::White):
            Circle(vertices, indices, Application::get_instance().get_resource_manager().get_white_texture(), color) { }

        void set_position(const Position &position) {
            auto &vertices = this->get_mesh().get_vertices();
            Position delta = position - vertices[0].position;
            for (auto &vertex: vertices)
                vertex.position += delta;
        }

        void set_radius(float radius) {
            auto &vertices = this->get_mesh().get_vertices();
            Position &center = vertices[0].position;
            float ratio = radius / glm::distance((glm::vec3)vertices[0].position, (glm::vec3)vertices[1].position);
            for (auto &vertex: vertices) {
                vertex.position.x -= (vertex.position.x - center.x) * (1.0f - ratio);
                vertex.position.y -= (vertex.position.y - center.y) * (1.0f - ratio);
            }
        }

        virtual void on_draw(Renderer &renderer, float dt) override { }
};

} // namespace cmw::shapes
